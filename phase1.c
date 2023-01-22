#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#define UserInsanityCheck() {                           \
    strcpy(Path , argv[++i]) ;                          \
    if(argv[i++][0] == '"')                             \
    {                                                   \
        int LastChar = strlen(argv[i]);                 \
        while(!strchr(argv[i++] , '"'))                 \
        {                                               \
            Path[LastChar] = ' ';                       \
            strcat(Path , argv[i]);                     \
            LastChar += 1 + strlen(argv[i]);            \
        }                                               \
        Path[LastChar-1] = '\0';                        \
                                                        \
    }                                                   \
    else i--;                                           \
}

#define UserInsanityCheck2() {                          \
    strcpy(SS , argv[++i]) ;                            \
    if(argv[i++][0] == '"')                             \
    {                                                   \
        int LastChar = strlen(argv[i]);                 \
        while(argv[++i][0] != '-')                      \
        {                                               \
            SS[LastChar] = ' ';                         \
            strcat(SS , argv[i]);                       \
            LastChar += 1 + strlen(argv[i]);            \
        }                                               \
        SS[LastChar-1] = '\0';                          \
    }                                                   \
    else i--;                                           \
}
char ReferenceFileName[100];
char Path[1000] , SS[1000 * 1000];
void CreateFile(char FIleName[]);
void Cat(char FileName[]);
void Removestr(char FileName[] ,int LineNum , int CharNum , int size , char fb);
void Insertstr(char FileName[] , char String[] , int LineNumber , int CharNum);
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
            UserInsanityCheck();
            Cat(Path);
        }
        else if(!(strcmp(argv[i] , "removestr")))
        {
            if (!(strcmp(argv[++i], "-file"))) {
                printf("Invalid Format! --file");
                continue;
            }
            UserInsanityCheck();
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
            Removestr(Path , LineNum , CharNum , size , argv[++i][1]);
        }
        else if(!(strcmp(argv[i] , "insertstr")))
        {
            if (!(strcmp(argv[++i], "-file"))) {
                printf("Invalid Format! --file");
                continue;
            }
            UserInsanityCheck();
            if (!strcmp(argv[++i], "-str")) {
                printf("Invalid Format! --str");
                continue;
            }
            UserInsanityCheck2();
            int LineNum , CharNum;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            Insertstr(Path , SS , LineNum , CharNum);
        }
    }
    return 0;
}

void CreateFile(char FIleName[])
{
    char *token = strtok(FIleName , "/");
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

void Removestr(char FileName[] ,int LineNum , int CharNum , int size , char fb)
{
    int temp;
    FILE *file = Reference(FileName);
    FILE *destination = fopen("destination.txt", "wb+");
    fseek(file , 0 , SEEK_SET);
    for(int j = 1 ; j < LineNum ; j++) {
        temp = 'A';
        while (temp != '\n') {
            temp = fgetc(file);
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
    int temp;
    FILE *file = Reference(FileName);
    FILE *destination = fopen("destination.txt", "wb+");
    fputs(String , file);
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
    temp = fopen(token , "r+");
    if(temp) {
        fclose(temp);
        temp = fopen(token , "a+");
        if(!temp)
        {
            printf("There Is NO Such File\n");
            exit(1);
        }
        return temp;
    }
    else {
        fclose(temp);
        printf("Invalid File Name\n");
        exit(-1);
    }
}