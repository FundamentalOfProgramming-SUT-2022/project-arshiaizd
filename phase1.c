#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
char ReferenceFileName[1000];
void CreateFile(char FileName[]);
void Cat(char FileName[]);
void Removestr(char FileName[] ,int LineNum , int CharNum , int size , int fb);
void Insertstr(char FileName[] , char String[] , int LineNumber , int CharNum);
void Copy(char FileName[] ,int LineNum , int CharNum , int size , int fb);
void Cut(char FileName[] ,int LineNum , int CharNum , int size , int fb);
void Paste(char FileName[] ,int LineNum , int CharNum);
FILE *Reference(char FileName[]);


int main(int argc , char *argv[])
{
    int i = 0;
    while(i < argc) {
        i++;
        if (!strcmp(argv[i], "createfile"))
        {
            if(!strcmp(argv[++i] , "-file")) {
                printf("Invalid Format! --file");
                continue;
            }
            CreateFile(argv[++i]);
        }
        else if(!strcmp(argv[i] , "cat")) {
            if (!strcmp(argv[++i], "-file")) {
                printf("Invalid Format! --file");
                continue;
            }

            Cat(argv[++i]);
        }
        else if(!(strcmp(argv[i] , "removestr")))
        {
            if (!(strcmp(argv[++i], "-file"))) {
                printf("Invalid Format! --file");
                continue;
            }
            int FilePointer = ++i;
            if (!strcmp(argv[++i], "-pos")) {
                printf("Invalid Format! --pos");
                continue;
            }
            int LineNum , CharNum ,size;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            if (!strcmp(argv[++i], "size")) {
                printf("Invalid Format! -size\n");
                continue;
            }
            sscanf(argv[++i] , "%d" , &size);
            int fb = argv[++i][1];
            Removestr(argv[FilePointer] , LineNum , CharNum , size , fb);
        }
        else if(!(strcmp(argv[i] , "insertstr")))
        {
            if (!(strcmp(argv[++i], "-file"))) {
                printf("Invalid Format! --file");
                continue;
            }
            int FilePointer = ++i;
//            printf("%s\n" , argv[FilePointer]);
            if (!strcmp(argv[++i], "-str")) {
                printf("Invalid Format! --str");
                continue;
            }
            int StringPointer = ++i;
//            puts(argv[StringPointer]);
            int LineNum , CharNum;
            i++;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            Insertstr(argv[FilePointer] , argv[StringPointer] , LineNum , CharNum);
        }
        else if(!(strcmp(argv[i] , "copystr")))
        {
            if (!(strcmp(argv[++i], "-file"))) {
                printf("Invalid Format! --file");
                continue;
            }
            int FilePointer = ++i;
            if (!strcmp(argv[++i], "-pos")) {
                printf("Invalid Format! --pos");
                continue;
            }
            int LineNum , CharNum ,size;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            if (!strcmp(argv[++i], "size")) {
                printf("Invalid Format! -size\n");
                continue;
            }
            sscanf(argv[++i] , "%d" , &size);
            int fb = argv[++i][1];
            Copy(argv[FilePointer] , LineNum , CharNum , size , fb);
        }
        else if(!(strcmp(argv[i] , "cutstr")))
        {
            if (!(strcmp(argv[++i], "-file"))) {
                printf("Invalid Format! --file");
                continue;
            }
            int FilePointer = ++i;
            if (!strcmp(argv[++i], "-pos")) {
                printf("Invalid Format! --pos");
                continue;
            }
            int LineNum , CharNum ,size;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            if (!strcmp(argv[++i], "size")) {
                printf("Invalid Format! -size\n");
                continue;
            }
            sscanf(argv[++i] , "%d" , &size);
            int fb = argv[++i][1];
            Cut(argv[FilePointer] , LineNum , CharNum , size , fb);
        }
        else if(!(strcmp(argv[i] , "pastestr")))
        {
            if (!(strcmp(argv[++i], "-file"))) {
                printf("Invalid Format! --file");
                continue;
            }
            int FilePointer = ++i;
            if (!strcmp(argv[++i], "-pos")) {
                printf("Invalid Format! --pos");
                continue;
            }
            int LineNum , CharNum;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            Paste(argv[FilePointer] , LineNum , CharNum);
        }
    }
    return 0;
}

void CreateFile(char FileName[])
{
    char *token = strtok(FileName , "/");
    while(token != NULL)
    {
        if(strchr(token , '.')) {
            break;
        }
        if(chdir(token) == -1)
        {
            mkdir(token);
            chdir(token);
        }
        token = strtok(NULL , "/");

    }
    FILE *temp;
    temp = fopen(token , "r");
    if(temp) {
        printf("The File Already Exists!\n");
        fclose(temp);
    }
    else {
        temp = fopen(token, "w");
        if(!temp)
        {
            printf("The File Could Not Be Created!\n");
            exit(1);
        }
        fclose(temp);
        printf("File Successfully created\n");
    }
    return;
}

void Cat(char FileName[])
{
    FILE *file = Reference(FileName);
    int pointer;
    while(1)
    {
        pointer = fgetc(file);
        if(feof(file))
            break;
        printf("%c" , pointer);
    }
    fclose(file);

    return;
}

void Removestr(char FileName[] ,int LineNum , int CharNum , int size , int fb)
{
 int temp;
    FILE *file = Reference(FileName);
    FILE *destination = fopen("destination.txt", "wb+");
    fseek(file , 0 , SEEK_SET);
    for(int j = 1 ; j < LineNum ; j++) {
        temp = 'A';
        while (temp != '\r') {
            temp = fgetc(file);
            if(temp == EOF)
            {
                printf("That location Does Not Exist!!\n");
                exit(1);
            }
            fprintf(destination, "%c", temp);
        }
    }
    for(int j = 0; j < CharNum;j++) {
        temp = fgetc(file);
        fprintf(destination, "%c", temp);
    }
    if(fb == 'b') {
        fseek(destination , -size  , SEEK_CUR);
    }
    else
    {
        int j;
        for( j = 0; j < size ; j++)
            fgetc(file);
    }
    while(1) {
        temp = fgetc(file);
        if (feof(file))
            break;
        fprintf(destination, "%c", temp);
    }
    fclose(file);
    fclose(destination);
    remove(ReferenceFileName);
    rename("destination.txt" , ReferenceFileName);
    printf("File Successfully Edited!\n");
    return;
}


void Insertstr(char FileName[] , char String[] , int LineNum , int CharNum)
{
    int temp , flag = 0;
    FILE *file = Reference(FileName);
    FILE *destination = fopen("destination.txt", "wb+");
    fseek(file , 0 , SEEK_SET);
    for(int j = 1 ; j < LineNum ; j++) {
        temp = 'A';
        while (temp != '\r') {
            temp = fgetc(file);
            if(temp == EOF)
            {
                printf("That line does not exist yet!!\n");
                exit(-1);
            }

            fputc(temp, destination);
        }
    }
    for(int j = 0; j <= CharNum-1;j++) {
        temp = fgetc(file);
        if(temp == EOF)
        {
            printf("That place does not exist yet!!\n");
            exit(-1);
        }
        if(temp == '\r')
        {
            printf("%d " , j);
            for(int k = 0; k < CharNum - j;k++)
                fputs(" ", destination);
            break;
        }
        fprintf(destination, "%c", temp);
    }
    for(int k = 0;k < strlen(String);k++) {
        temp = String[k];
        if(temp == '\\' && !flag)
            {
                flag = 1;
                continue;
            }
        else if(flag && temp == '\\')
        {
            fprintf(destination , "\\");
            flag = 0;
            continue;
        }
        else if(flag && temp == 'n')
        {
            fprintf(destination , "\n");
            flag = 0;
            continue;
        }
        else if(temp == '\'') {
            fputc('\"', destination);
            continue;
        }
        fputc(temp , destination);
    }
    while(1)
    {
        int temp2 = fgetc(file);
        if(feof(file))
            break;
        fprintf(destination, "%c", temp2);
    }
    fclose(file);
    fclose(destination);
    remove(ReferenceFileName);
    rename("destination.txt" , ReferenceFileName);
    printf("String Successfully Inserted!\n");
    return;
}












FILE* Reference(char FileName[])
{
    char *token = strtok( FileName, "/");
    while(token != NULL)
    {
        if(strchr(token , '.')) {
            break;
        }
        if(chdir(token) == -1)
        {
            printf("Invalid File Path!\n");
            exit(-1);
        }
        token = strtok(NULL , "/");

    }
    FILE *temp;
    sprintf(ReferenceFileName,"%s", token);
    temp = fopen(token , "r");
    if(temp) {
        fclose(temp);
        temp = fopen(token , "ab+");
        return temp;
    }
    else {
        fclose(temp);
        printf("Invalid File Name\n");
        exit(-1);
    }
}