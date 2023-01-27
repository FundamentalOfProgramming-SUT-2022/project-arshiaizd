#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fileapi.h>
char ReferenceFileName[1000];
void CreateFile2(char FileName[]);
void Cat(char FileName[]);
void Removestr(char FileName[] ,int LineNum , int CharNum , int size , int fb ,int handle);
void Insertstr(char FileName[] , FILE* tempfile , int LineNumber , int CharNum , int handle);
void Copy(char FileName[] ,int LineNum , int CharNum , int size , int fb , int handle);
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
            CreateFile2(argv[++i]);
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
            Removestr(argv[FilePointer] , LineNum , CharNum , size , fb , 0);
        }
        else if(!(strcmp(argv[i] , "insertstr")))
        {
            if (!(strcmp(argv[++i], "-file"))) {
                printf("Invalid Format! --file");
                continue;
            }
            int FilePointer = ++i;
            if (!strcmp(argv[++i], "-str")) {
                printf("Invalid Format! --str");
                continue;
            }
            FILE *tempfile = fopen("tempfile.txt" , "wb+");
            fprintf(tempfile , "%s" , argv[++i]);
            int LineNum , CharNum;
            i++;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            Insertstr(argv[FilePointer] , tempfile , LineNum , CharNum , 0);
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
            Copy(argv[FilePointer] , LineNum , CharNum , size , fb , 0);
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

void CreateFile2(char FileName[])
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

void Removestr(char FileName[] ,int LineNum , int CharNum , int size , int fb , int handle)
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
    if(handle)
        return;
    printf("File Successfully Edited!\n");
    return;
}


void Insertstr(char FileName[] , FILE *tempfile , int LineNum , int CharNum ,int handle)
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
    fseek(tempfile , 0 , SEEK_SET);
    while(1) {
        temp = fgetc(tempfile);
        if(temp == EOF) break;
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
    fclose(tempfile);
    DeleteFileA("tempfile.txt");
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
    if(handle) return;
    printf("String Successfully Edited!\n");
    return;
}

void Copy(char FileName[] ,int LineNum , int CharNum , int size , int fb ,int handle)
{
    char temp;
    FILE *file = Reference(FileName);
    chdir("system");
    DeleteFileA("clipboard.txt");
    FILE *clipboard = fopen("clipboard.txt", "wb+");
    fseek(file , 0 , SEEK_SET);
    for(int j = 1 ; j < LineNum ; j++) {
        temp = 'A';
        while (temp != '\r') {
            temp = fgetc(file);
            fflush(stdout);
            if(temp == EOF)
            {
                printf("That line does not exist!!\n");
                exit(-1);
            }

        }
    }
    for(int j = 0; j <= CharNum-1;j++) {
        temp = fgetc(file);
        if(temp == EOF)
        {
            printf("That place does not exist yet!!\n");
            exit(-1);
        }
        if(temp == '\n')
        {
            printf("There Is Not Enough Character In This Line\n");
            exit(1);
        }
    }
    if(fb == 'b') {
        fseek(file , -size  , SEEK_CUR);
    }
    for(int k = 0; k < size; k++)
    {
        char temp2;
        temp2 = fgetc(file);
        if(temp == EOF)
        {
            printf("Pointer Reached End of File\n");
            break;
        }
        fputc(temp2 , clipboard);
    }
    fclose(file);
    fclose(clipboard);
    SetFileAttributesA("clipboard.txt" , FILE_ATTRIBUTE_HIDDEN);
    chdir("..");
    if(handle) return;
    printf("String Successfully Copied\n");
}

void Cut(char FileName[] ,int LineNum , int CharNum , int size , int fb)
{
    Copy(FileName , LineNum , CharNum , size ,fb ,1);
    Removestr(FileName , LineNum , CharNum , size , fb ,1);
    printf("String Successfully Cut\n");
    return;
}

void Paste(char FileName[] ,int LineNum , int CharNum)
{
    chdir("system");
    FILE *tempfile = fopen("clipboard.txt" , "rb+");
    chdir("..");
    Insertstr(FileName , tempfile , LineNum , CharNum , 1);
    printf("String Successfully Pasted!\n");
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
        mkdir("BYEEe");
        printf("Invalid File Name! %s\n" , token);
        exit(-1);
    }
}