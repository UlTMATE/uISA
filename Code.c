#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void putData();
int checkRegInsFile(char code[],int i);
int checkImmInsFile(char code[],int i);
int checkIndInsFile(char code[],int i);
void checkForLabel();
void passOne();
void passTwo();
char insString[25], insBin[17];
int lineNum=1;
char labelFileName[15]={'l','a','b','e','l','F','i','l','e','.','t','x','t','\0'};
char outFileName[50];//={'o','u','t','p','u','t','.','t','x','t','\0'};
char programFileName[50];//={'p','r','o','g','r','a','m','.','t','x','t','\0'};
char symbolFileName[16]={'s','y','m','b','o','l','F','i','l','e','.','t','x','t','\0'};
FILE *progFile, *outFile;
FILE *symbolFile, *labFile;
int main(){
    printf("*********************\n\tISA\n*********************\n\n");
    printf("16 Bit Address Bus\n16 Bit Instruction Code\n14 General Purpose Registers (R01-R14)\n");
    printf("2 Special Purpose Registers SPR and FLG Register\n");
    printf("3 Addressing Mode Used : Register, Indirect and Immediate (0-f)\n\n");
    printf("***********************************\n\n");
 //   printf()
    printf("Enter the Source file name: ");
    scanf("%s",programFileName);
    printf("Enter the Designation file name: ");
    scanf("%s",outFileName);
    passOne();
    passTwo();
}

void passOne(){
 //   printf("In Pass One\n");
    int inserted=0, comma=0, space=0,  currLoc=0;
    char c;
    symbolFile=fopen(symbolFileName, "w+");
    labFile=fopen(labelFileName,"w+");
    if((progFile = fopen(programFileName, "r+")) == NULL){
        printf("Error! No Program File Found\n");
        exit(1);
    }
    int i;
    while((c=fgetc(progFile))!=-1){
        if(c=='\n'){
            checkForLabel();
            lineNum++;
            fputs(insString,symbolFile);
            putc(c,symbolFile);
            space=0; comma=0; inserted=0; currLoc=0;
            int j=0;
            for(j=0;j<20;j++){
                insString[j]='\0';
            }
            continue;
        }
        if((space!=0 && c==' ') || (space==0 && c==' ' && inserted==0)){ //Checking extra spaces
            continue;
        }
        if((comma!=0 && c==',') || (comma==0 && c==',' && inserted==0)){ //Checking extra commas
            printf("\n62Syntax Error in Program File: \nExtra ',' at line %d\n",lineNum);
            fclose(symbolFile);
            remove(symbolFileName);
            exit(1);
        }
        if(space==0 && inserted!=0 && c==' '){ //Inserting space
            insString[currLoc]=c;
            currLoc++;
            space++;
            continue;
        }
        if(inserted==0 && c=='\0'){
            continue;
        }
        if(inserted!=0 && c==','){ //Inserting space by replacing comma
  /*          if(currLoc==16){
                printf("\nSyntax Error in Program File: \nExtra ',' at line %d\n",lineNum);
                exit(1);
            }
*/
            insString[currLoc]=' ';
            currLoc++;
            comma++;
            space++;
            continue;
        }
        if(c=='\t'){
            insString[currLoc]=' ';
            currLoc++;
            space++;
            continue;
        }
 /*       if(currLoc==19){
            printf("\n88Syntax Error in Program File: \nExtra instruction at line %d\n",lineNum);
 //           remove("mySymbolFile.isa");
            exit(1);
        }
*/
        insString[currLoc]=c;
        inserted++;
        currLoc++;
        space=0;
        comma=0;
    }
    fclose(progFile);
    fclose(symbolFile);
    fclose(labFile);
 //   printf("End Pass One\n");
}

void passTwo(){
//    printf("In Pass Two\n");
    lineNum=1;
	int inserted=0, comma=0, space=0,  currLoc=0;
    char c;
    outFile = fopen(outFileName, "w+");
    if((progFile = fopen(symbolFileName, "r+")) == NULL){
        printf("Error! No Symbol File Found\n");
        fclose(labFile);
        fclose(symbolFile);
        fclose(outFile);
        remove(symbolFileName);
        remove(outFileName);
        remove(labelFileName);
        exit(1);
    }
    int i;
    for(i=0; i<16;i++){
        insBin[i]='2';
    }
    for(i=0; i<25; i++){
        insString[i]='\0';
    }
    while((c=fgetc(progFile))!=-1){
        if(c=='\n'){
            if(insString[0]=='\0'){
                continue;
            }
            putData();
            int status=2;
            int j;
            for(j=0; j<16; j++){
                if(insBin[j]=='2'){
                    if(status==1){
                        status=-1;
                        break;
                    }
                    status=0;
                } else{
                    if(status==0){
                        status=-1;
                        break;
                    } else{
                        status=1;
                    }
                }
            }
 //           printf("\nstat=%d \n",status);
            if(status==-1){
                printf("\n(137) Syntax Error in Program File: \nIncomplete code (not 32 bit) at line %d\n",lineNum);
                fclose(labFile);
                fclose(symbolFile);
                fclose(outFile);
                remove(symbolFileName);
                remove(outFileName);
                remove(labelFileName);
                exit(1);
            } else if(status==1){
                fputs(insBin,outFile);
                for(j=0;j<16;j++){
                    insBin[j]='2';
                }
                for(j=0;j<25;j++){
                    insString[j]='\0';
                }
                lineNum++;
            } else if(status==0){
                continue;
            }
            putc(c,outFile);
            space=0; comma=0; inserted=0; currLoc=0;
            continue;
        }
        if((space!=0 && c==' ') || (space==0 && c==' ' && inserted==0)){ //Checking extra spaces
            continue;
        }
        if((comma!=0 && c==',') || (comma==0 && c==',' && inserted==0)){ //Checking extra commas
            printf("\n(159) Syntax Error in Program File: \nExtra ',' at line %d\n",lineNum);
            fclose(labFile);
            fclose(symbolFile);
            fclose(outFile);
            remove(symbolFileName);
            remove(outFileName);
            remove(labelFileName);
            exit(1);
        }
        if(space==0 && inserted!=0 && c==' '){ //Inserting space
            insString[currLoc]=c;
            currLoc++;
            space++;
            continue;
        }
        if(inserted!=0 && c==','){ //Inserting space by replacing comma
  /*          if(currLoc==16){
                printf("\nSyntax Error in Program File: \nExtra ',' at line %d\n",lineNum);
                exit(1);
            }
*/
            insString[currLoc]=' ';
            currLoc++;
            comma++;
            space++;
            continue;
        }
 /*       if(currLoc==16){
            printf("\n182Syntax Error in Program File: \nExtra instruction at line %d\n",lineNum);
            remove(outFileName);
            exit(1);
        }
*/
        insString[currLoc]=c;
        inserted++;
        currLoc++;
        space=0;
        comma=0;
    }
    fclose(progFile);
    fclose(outFile);
    fclose(labFile);
    remove(labelFileName);
    remove(symbolFileName);
    exit(0);
//    printf("End Pass Two\n");
}
void checkForLabel(){
    int i=0, j=0; char code[20];
    for(i; insString[i]!=' '; i++, j++){
        if(insString[i]=='\0' || insString[i]=='\n' || insString[i]=='\r'){
            break;
        }
        code[j]=insString[i];
   //     printf("code[%d]: %c\n",j,code[j]);
    }
    if(code[j-1]==':'){
        char labCode[20];
//        printf("code[j-2]= %c\n",code[j-2]);
        int f=0;
        for(f; f<j-1; f++){
            labCode[f]=code[f];
  //          printf("labCode[%d]=%c\n",f,labCode[f]);
        }
        labCode[f]='\0';
        fprintf(labFile,"%s ",labCode);
        int z=7;
        int quotient = lineNum;
 //       printf("line is %d",lineNum);
        char binLab[9];
        if(quotient>256){
            printf("Error! Label index out of bounds at line %d",lineNum);
            fclose(labFile);
            fclose(symbolFile);
            remove(symbolFileName);
            remove(labelFileName);
            exit(1);
        }
        while(quotient!=0){
            if((quotient % 2)==0){
                binLab[z--]='0';
            } else {
                binLab[z--]='1';
  //              printf("Here it is %d : %c\n",z+1,binLab[z+1]);
            }
            quotient = quotient / 2;
        }
        for( ;z>=0;z--){
            binLab[z]='0';
        }
        binLab[8]='\0';
        fprintf(labFile,"%s\n", binLab);
        return;
    }
}
void putData(){
    int i=0, j=0; char code[20];
    for(i; insString[i]!=' '; i++, j++){
        if(insString[i]=='\0' || insString[i]=='\n' || insString[i]=='\r'){
            break;
        }
        code[j]=insString[i];
 //       printf("code%d %c \n",j,code[j]);
    }
    if(code[j-1]==':'){
        i++, j=0;
    	for(i; insString[i]!=' '; i++, j++){
        if(insString[i]=='\0' || insString[i]=='\n' || insString[i]=='\r'){
            break;
        }
        code[j]=insString[i];
 //       printf("code%d %c \n",j,code[j]);
    	}
    }
        if(insString[0]=='\0'){
            return;
        }
        if(j>4){
            printf("Error! Invalid OPCODE at line : %d",lineNum);
            fclose(labFile);
            fclose(symbolFile);
            fclose(outFile);
            remove(symbolFileName);
            remove(outFileName);
            remove(labelFileName);
            exit(1);
        }
        if(checkRegInsFile(code,i+1)==1){
 //           printf("Found in RegInsFile\n");
            return;
        }
        else if(checkIndInsFile(code,i+1)==1){
 //           printf("Found in IndInsFile\n");
            return;
        }
        else if(checkImmInsFile(code,i+1)==1){
 //           printf("Found in ImmInsFile\n");
            return;
        }
        else{
            printf("(329) Syntax Error in code at line %d :- Invalid Opcode",lineNum);
            fclose(labFile);
            fclose(symbolFile);
            fclose(outFile);
            remove(symbolFileName);
            remove(outFileName);
            remove(labelFileName);
            exit(1);
        }
}
int checkRegInsFile(char ins[],int i){
//    printf("Here in Reg\n");
    FILE *regInsFile, *regFile;
    if((regInsFile = fopen("RinstructionFile.txt", "r+")) == NULL){
        printf("Error! No RinstructionFile File Found\n");
        fclose(labFile);
        fclose(symbolFile);
        fclose(outFile);
        remove(symbolFileName);
        remove(outFileName);
        remove(labelFileName);
        exit(1);
    }
    char opcode[5], bin[17];
    int currLoc=0;

    while(feof(regInsFile)==0){
        fscanf(regInsFile,"%s %s",opcode,bin);
        int as=0, flag=0;
  //      for(as; as<i-1; as++){
          for(as; opcode[as]!='\0'; as++){
  //              printf("Its %d op:%c ins:%c \n",as,opcode[as],ins[as]);
            if(opcode[as]==ins[as]){
                flag=1;
            } else{
                flag=0;
                break;
            }
        }
        if(flag==1){
            int j=0;
            for(j; j<17 && bin[j]!='\0'; j++){
                insBin[j]=bin[j];
 //               printf("insBin[%d]=%c \n",j,insBin[j]);
            }
            currLoc=j;
            if(currLoc==4){
                char r[3][4];
                int z;
                for(z=0; z<3; z++){
                    for(i,j=0; i<20 && insString[i]!=' ' && insString[i]!='\0'; i++,j++){
  //                          printf("its %d ",j);
                        if(j>3){
                            printf("Error! Invalid register name at line %d\nCheck Manual for Register Addressing Mode's 3 Addressible Instructions",lineNum);
                            fclose(regInsFile);
                            fclose(labFile);
                            fclose(symbolFile);
                            fclose(outFile);
                            remove(symbolFileName);
                            remove(outFileName);
                            remove(labelFileName);
                            exit(1);
                        }
                        r[z][j]=insString[i];
  //                  printf("rzj: %c ",r[z][j]);
  //                  printf("\n");
                    }
                    i++;
                }
                if((regFile = fopen("registerFile.txt", "r+")) == NULL){
                    printf("Error! No Register File Found\n");
                    fclose(regInsFile);
                    fclose(labFile);
                    fclose(symbolFile);
                    fclose(outFile);
                    remove(symbolFileName);
                    remove(outFileName);
                    remove(labelFileName);
                    exit(1);
                }
                int flagi=0,myFlag=0;
                for(z=0;z<3;z++){
                    while(feof(regFile)==0){
                        char regChar[4], binRegChar[5];
                        fscanf(regFile,"%s %s",regChar,binRegChar);
                        int temp=0;
                        for(temp; temp<3; temp++){
  //                          printf("reg=%c rz=%c\n",regChar[temp],r[z][temp]);
                            if(regChar[temp]==r[z][temp]){
                                flagi++;
                            } else{
                                flagi=0;
                                break;
                            }
                        }
                        if(flagi>0){
  //                          printf("yoooo\n");
                            myFlag++;
                            int k;
                            for(k=0; k<4 && binRegChar[j]!='\0'; k++,currLoc++){
                                insBin[currLoc]=binRegChar[k];
                            }
                        }
                }
                rewind(regFile);
            }
                if(myFlag==3){
                        fclose(regInsFile);
                        fclose(regFile);
                    return 1;
                }
            } else if(currLoc==8){
                char r[2][4];
                int z;
                for(z=0; z<2; z++){
                    for(i,j=0; i<20 && insString[i]!=' ' && insString[i]!='\0'; i++,j++){
   //                         printf("its %d ",j);
                        if(j>3){
                            printf("Error! Invalid register name at line %d\nCheck Manual for Register Addressing Mode's 2 Addressible Instructions",lineNum);
                            fclose(regInsFile);
                            fclose(labFile);
                            fclose(symbolFile);
                            fclose(outFile);
                            remove(symbolFileName);
                            remove(outFileName);
                            remove(labelFileName);
                            exit(1);
                        }
                        r[z][j]=insString[i];
  //                  printf("rzj: %c ",r[z][j]);
  //                  printf("\n");
                    }
                    i++;
                }
                if((regFile = fopen("registerFile.txt", "r+")) == NULL){
                    printf("Error! No Register File Found\n");
                    fclose(regInsFile);
                    fclose(labFile);
                    fclose(symbolFile);
                    fclose(outFile);
                    remove(symbolFileName);
                    remove(outFileName);
                    remove(labelFileName);
                    exit(1);
                }
                int flagi=0,myFlag=0;
                for(z=0;z<2;z++){
                    while(feof(regFile)==0){
                        char regChar[4], binRegChar[5];
                        fscanf(regFile,"%s %s",regChar,binRegChar);
                        int temp=0;
                        for(temp; temp<3; temp++){
 //                           printf("reg=%c rz=%c\n",regChar[temp],r[z][temp]);
                            if(regChar[temp]==r[z][temp]){
                                flagi++;
                            } else{
                                flagi=0;
                                break;
                            }
                        }
                        if(flagi>0){
  //                          printf("yoooo\n");
                            myFlag++;
                            int k;
                            for(k=0; k<4 && binRegChar[j]!='\0'; k++,currLoc++){
                                insBin[currLoc]=binRegChar[k];
                            }
                        }
                }
                rewind(regFile);
            }
                if(myFlag==2){
                        fclose(regInsFile);
                        fclose(regFile);
                    return 1;
                }
            } else if(currLoc==12){
                char r[1][4];
                int z, temp=i;
                for(z=0; z<1; z++){
                    for(i,j=0; i<20 && insString[i]!=' ' && insString[i]!='\0'; i++,j++){
  //                      printf("its %d ",j);
                        if(j>3){
                            printf("Error! Invalid register name at line %d\nCheck Manual for Register Addressing Mode's 1 Addressible Instructions",lineNum);
                            fclose(regInsFile);
                            fclose(labFile);
                            fclose(symbolFile);
                            fclose(outFile);
                            remove(symbolFileName);
                            remove(outFileName);
                            remove(labelFileName);
                            exit(1);
                        }
                        r[z][j]=insString[i];
  //                      printf("rzj: %c ",r[z][j]);
    //                    printf("\n");
                    }
                    i++;
                }
                if((regFile = fopen("registerFile.txt", "r+")) == NULL){
                    printf("Error! No Register File Found\n");
                    fclose(regInsFile);
                    fclose(labFile);
                    fclose(symbolFile);
                    fclose(outFile);
                    remove(symbolFileName);
                    remove(outFileName);
                    remove(labelFileName);
                    exit(1);
                }
                int flagi=0,myFlag=0;
                for(z=0;z<1;z++){
                    while(feof(regFile)==0){
                        char regChar[4], binRegChar[5];
                        fscanf(regFile,"%s %s",regChar,binRegChar);
                        int temp=0;
                        for(temp; temp<3; temp++){
  //                          printf("reg=%c rz=%c\n",regChar[temp],r[z][temp]);
                            if(regChar[temp]==r[z][temp]){
                                flagi++;
                            } else{
                                flagi=0;
                                break;
                            }
                        }
                        if(flagi>0){
   //                         printf("yoooo\n");
                            myFlag++;
                            int k;
                            for(k=0; k<4 && binRegChar[j]!='\0'; k++,currLoc++){
                                insBin[currLoc]=binRegChar[k];
                            }
                        }
                    }
                    rewind(regFile);
                }
                if(myFlag==1){
                    fclose(regInsFile);
                    fclose(regFile);
                    return 1;
                }

            } else if(currLoc==16){
                fclose(regInsFile);
                return 1;
            } else{
                return -1;
            }
        }
    }
    fclose(regInsFile);
    return -1;
}

int checkIndInsFile(char ins[],int i){
//    printf("Here in Ind\n");
    FILE *indInsFile, *regFile;
    if((indInsFile = fopen("XinstructionFile.txt", "r+")) == NULL){
        printf("Error! No XinstructionFile File Found\n");
        fclose(labFile);
        fclose(symbolFile);
        fclose(outFile);
        remove(symbolFileName);
        remove(outFileName);
        remove(labelFileName);
        exit(1);
    }
    char opcode[5], bin[17];
    int currLoc=0;
    while(feof(indInsFile)==0){
        fscanf(indInsFile,"%s %s",opcode,bin);
        int as=0, flag=0;
 //       for(as; as<i-1; as++){
        for(as; opcode[as]!='\0'; as++){
            if(opcode[as]==ins[as]){
                flag=1;
            } else{
                flag=0;
                break;
            }
        }
        if(flag==1){
            int j=0;
            for(j; j<17 && bin[j]!='\0'; j++){
                insBin[j]=bin[j];
            }
            currLoc=j;
            if(currLoc==4){
                char r[3][4];
                int z;
                for(z=0; z<3; z++){
                    for(i,j=0; i<20 && insString[i]!=' ' && insString[i]!='\0'; i++,j++){
    //                        printf("its %d ",j);
                        if(j>3){
                            printf("Error! Invalid register name at line %d\nCheck manual for Indirect Addressing Mode's 3 Addressable Instructions",lineNum);
                            fclose(indInsFile);
                            fclose(labFile);
                            fclose(symbolFile);
                            fclose(outFile);
                            remove(symbolFileName);
                            remove(outFileName);
                            remove(labelFileName);
                            exit(1);
                        }
                        r[z][j]=insString[i];
  //                  printf("rzj: %c ",r[z][j]);
    //                printf("\n");
                    }
                    i++;
                }
                if((regFile = fopen("registerFile.txt", "r+")) == NULL){
                    printf("Error! No Register File Found\n");
                    fclose(indInsFile);
                    fclose(labFile);
                    fclose(symbolFile);
                    fclose(outFile);
                    remove(symbolFileName);
                    remove(outFileName);
                    remove(labelFileName);
                    exit(1);
                }
                int flagi=0,myFlag=0;
                for(z=0;z<3;z++){
                    while(feof(regFile)==0){
                        char regChar[4], binRegChar[5];
                        fscanf(regFile,"%s %s",regChar,binRegChar);
                        int temp=0;
                        for(temp; temp<3; temp++){
      //                      printf("reg=%c rz=%c\n",regChar[temp],r[z][temp]);
                            if(regChar[temp]==r[z][temp]){
                                flagi++;
                            } else{
                                flagi=0;
                                break;
                            }
                        }
                        if(flagi>0){
     //                       printf("yoooo\n");
                            myFlag++;
                            int k;
                            for(k=0; k<4 && binRegChar[j]!='\0'; k++,currLoc++){
                                insBin[currLoc]=binRegChar[k];
                            }
                        }
                }
                rewind(regFile);
            }
                if(myFlag==3){
                        fclose(indInsFile);
                        fclose(regFile);
                    return 1;
                }
            } else if(currLoc==8){
  //              printf("At 8th loc");
                char r[2][4];
                int z;
                int temp=i;
                for(z=0; z<2; z++){
                    for(i,j=0; i<20 && insString[i]!=' ' && insString[i]!='\0'; i++,j++){
  //                          printf("its %d ",j);
                        if(j>3){
                            printf("Error! Invalid register name at line %d\nCheck manual for Indirect Addressing Mode's 2 Addressable Instructions",lineNum);
                            fclose(indInsFile);
                            fclose(labFile);
                            fclose(symbolFile);
                            fclose(outFile);
                            remove(symbolFileName);
                            remove(outFileName);
                            remove(labelFileName);
                            exit(1);
                        }
                        r[z][j]=insString[i];
   //                 printf("rzj: %c ",r[z][j]);
     //               printf("\n");
                    }
                    i++;
                }
                if((regFile = fopen("registerFile.txt", "r+")) == NULL){
                    printf("Error! No Register File Found\n");
                    fclose(indInsFile);
                    fclose(labFile);
                    fclose(symbolFile);
                    fclose(outFile);
                    remove(symbolFileName);
                    remove(outFileName);
                    remove(labelFileName);
                    exit(1);
                }
                int flagi=0,myFlag=0;
                for(z=0;z<2;z++){
                    while(feof(regFile)==0){
                        char regChar[4], binRegChar[5];
                        fscanf(regFile,"%s %s",regChar,binRegChar);
                        int tempo=0;
                        for(tempo; tempo<3; tempo++){
      //                      printf("reg=%c rz=%c\n",regChar[temp],r[z][temp]);
                            if(regChar[tempo]==r[z][tempo]){
                                flagi++;
                            } else{
                                flagi=0;
                                break;
                            }
                        }
                        if(flagi>0){
  //                          printf("yoooo\n");
                            myFlag++;
                            int k;
                            for(k=0; k<4 && binRegChar[j]!='\0'; k++,currLoc++){
                                insBin[currLoc]=binRegChar[k];
                            }
                        }
                }
                rewind(regFile);
            }
                if(myFlag==2){
                        fclose(indInsFile);
                        fclose(regFile);
                    return 1;
                } else{
                    char lab[20];
    //                printf("here %d\n",i);
                    for(temp,j=0; temp<20 && insString[temp]!=' ' && insString[temp]!='\0'; temp++,j++){
   //                     printf("Its lab %d ",j);
                        lab[j]=insString[temp];
   //                     printf("Lab_rzj: %c ",lab[j]);
   //                     printf("\n");
                    }
                    lab[j]='\0';
                    if((labFile = fopen(labelFileName, "r+")) == NULL){
                        fclose(indInsFile);
                        return -1;
                    }
                    char insLab[10], binLab[17];
                    int flagy=0;
                    while(feof(labFile)==0){
                        fscanf(labFile,"%s %s", insLab,binLab);
                        int g=0;
                        for(g; insLab[g]!='\0'; g++){
                            if(lab[g]==insLab[g]){
                                flagy=1;
  //                              printf("\n\nAlmost %d %d there\n\n",flagy,g);
                            } else{
                                flagy=0;
   //                             printf("\n\nelse Almost %d %d there\n\n",flagy,g);
 //                               printf("lab:%c insLab:%c\n",lab[g],insLab[g]);
                                break;
                            }
                        }
                        if(flagy==1){
   //                     printf("Label at %d\n\n",currLoc);
                        int tempor=currLoc;
                        for(flagy=0; binLab[flagy]!='\0'; flagy++){
                            insBin[tempor]=binLab[flagy];
                            tempor++;
   //                          printf("B:%c ",insBin[flagy]);
                        }
                        return 1;
                    }
                    }
                }
            } else{
                return -1;
            }
        }
    }
    return -1;
}

int checkImmInsFile(char ins[],int i){
//    printf("Here in Imm\n");
    FILE *immInsFile, *regFile;
    if((immInsFile = fopen("IinstructionFile.txt", "r+")) == NULL){
        printf("Error! No Immediate Instructions File Found\n");
        fclose(labFile);
        fclose(symbolFile);
        fclose(outFile);
        remove(symbolFileName);
        remove(outFileName);
        remove(labelFileName);
        exit(1);
    }
    char opcode[5], bin[17];
    int currLoc=0;
    while(feof(immInsFile)==0){
   //         printf("Inside Loop\n");
        fscanf(immInsFile,"%s %s",opcode,bin);
        int as=0, flag=0;
 //       for(as; as<i-1; as++){
        for(as; opcode[as]!='\0'; as++){
  //          printf("opF:%c and code:%c \n",opcode[as],ins[as]);
            if(opcode[as]==ins[as]){
 //                   printf("here true\n");
                flag=1;
            } else{
                flag=0;
                break;
            }
        }
 //       printf("flag is %d\n",flag);
        if(flag==1){
  //          printf("In here");
            int j=0;
            for(j; j<17 && bin[j]!='\0'; j++){
                insBin[j]=bin[j];
            }
            currLoc=j;

            if(currLoc==8){
                char r[1][4];
                int z;
                for(z=0; z<1; z++){
                    for(i,j=0; i<20 && insString[i]!=' ' && insString[i]!='\0'; i++,j++){
   //                         printf("its %d ",j);
                        if(j>3){
                            printf("Error! Invalid register name at line %d\nCheck manual for Immediate Addressing Mode's 2 Addressable Instructions",lineNum);
                            fclose(immInsFile);
                            fclose(labFile);
                            fclose(symbolFile);
                            fclose(outFile);
                            remove(symbolFileName);
                            remove(outFileName);
                            remove(labelFileName);
                            exit(1);
                        }
                        r[z][j]=insString[i];
 //                   printf("rzj: %c ",r[z][j]);
   //                 printf("\n");
                    }
                    i++;
                }
                if((regFile = fopen("registerFile.txt", "r+")) == NULL){
                    printf("Error! No Register File Found\n");
                    fclose(immInsFile);
                    fclose(labFile);
                    fclose(symbolFile);
                    fclose(outFile);
                    remove(symbolFileName);
                    remove(outFileName);
                    remove(labelFileName);
                    exit(1);
                }
                int flagi=0,myFlag=0;
                for(z=0;z<1;z++){
                    while(feof(regFile)==0){
                        char regChar[4], binRegChar[5];
                        fscanf(regFile,"%s %s",regChar,binRegChar);
                        int temp=0;
                        for(temp; temp<3; temp++){
  //                          printf("reg=%c rz=%c\n",regChar[temp],r[z][temp]);
                            if(regChar[temp]==r[z][temp]){
                                flagi++;
                            } else{
                                flagi=0;
                                break;
                            }
                        }
                        if(flagi>0){
  //                          printf("yoooo\n");
                            myFlag++;
                            int k;
                            for(k=0; k<4 && binRegChar[j]!='\0'; k++,currLoc++){
                                insBin[currLoc]=binRegChar[k];
                            }
                        }
                }
                rewind(regFile);
            }
 //           printf("Value of myFlag=%d\n",myFlag);
                if(myFlag==1){
                        fclose(immInsFile);
                        fclose(regFile);
         //           return 1;
                }
                }
                int n=0;
                for(i,n; insString[i]!='\0'; i++,n++){
                    char ch=insString[i];
                    switch(ch){

                        case '0':
                            bin[0]='0';
                            bin[1]='0';
                            bin[2]='0';
                            bin[3]='0';
                            break;
                        case '1':
                            bin[0]='0';
                            bin[1]='0';
                            bin[2]='0';
                            bin[3]='1';
                            break;
                        case '2':
                            bin[0]='0';
                            bin[1]='0';
                            bin[2]='1';
                            bin[3]='0';
                            break;
                        case '3':
                            bin[0]='0';
                            bin[1]='0';
                            bin[2]='1';
                            bin[3]='1';
                            break;
                        case '4':
                            bin[0]='0';
                            bin[1]='1';
                            bin[2]='0';
                            bin[3]='0';
                            break;
                        case '5':
                            bin[0]='0';
                            bin[1]='1';
                            bin[2]='0';
                            bin[3]='1';
                            break;
                        case '6':
                            bin[0]='0';
                            bin[1]='1';
                            bin[2]='1';
                            bin[3]='0';
                            break;
                        case '7':
                            bin[0]='0';
                            bin[1]='1';
                            bin[2]='1';
                            bin[3]='1';
                            break;
                        case '8':
                            bin[0]='1';
                            bin[1]='0';
                            bin[2]='0';
                            bin[3]='0';
                            break;
                        case '9':
                            bin[0]='1';
                            bin[1]='0';
                            bin[2]='0';
                            bin[3]='1';
                            break;
                        case 'a':
                        case 'A':
                            bin[0]='1';
                            bin[1]='0';
                            bin[2]='1';
                            bin[3]='0';
                            break;
                        case 'b':
                        case 'B':
                            bin[0]='1';
                            bin[1]='0';
                            bin[2]='1';
                            bin[3]='1';
                            break;
                        case 'c':
                        case 'C':
                            bin[0]='1';
                            bin[1]='1';
                            bin[2]='0';
                            bin[3]='0';
                            break;
                        case 'd':
                        case 'D':
                            bin[0]='1';
                            bin[1]='1';
                            bin[2]='0';
                            bin[3]='1';
                            break;
                        case 'e':
                        case 'E':
                            bin[0]='1';
                            bin[1]='1';
                            bin[2]='1';
                            bin[3]='0';
                            break;
                        case 'f':
                        case 'F':
                            bin[0]='1';
                            bin[1]='1';
                            bin[2]='1';
                            bin[3]='1';
                            break;
                        default:
                            return -1;
                    }
                    int x;
                    for(currLoc,x=0; x<4; x++,currLoc++){
                        insBin[currLoc]=bin[x];
                    }
                }
                if(n>1){
                        printf("Syntax Error in code at line %d\nProvide Hexadecimal Number in range (0-f)",lineNum);
                        fclose(labFile);
                        fclose(symbolFile);
                        fclose(outFile);
                        remove(symbolFileName);
                        remove(outFileName);
                        remove(labelFileName);
                        exit(1);
                }
                return 1;
            }
    }
    return -1;
}
