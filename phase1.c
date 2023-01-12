#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

int i = 0;
void CreateFile(char path[]);

int main(int argc , char *argv[])
{
    while(i < argc) {
        if (!strcmp(argv[++i], "createfile"))
        {
            if(!strcmp(argv[++i] , "--file")) {
                printf("Invalid Format! %s\n" , argv[i]);
                continue;
            }
            CreateFile(argv[++i]);
        }
    }
    return 0;
}

void CreateFile(char path[])
{
    char *token = strtok(path , "/");
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
        fclose(temp);
        printf("File Successfully created\n");
    }
    exit(0);
}