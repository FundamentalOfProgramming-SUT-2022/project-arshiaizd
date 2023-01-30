#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fileapi.h>
#include <unistd.h>
char ReferenceFileName[1000] , Filename2[1000] ,Filename3[1000];
void CreateFile2(char FileName[] , int handle);
void Cat(char FileName[]);
void Removestr(char FileName[] ,int LineNum , int CharNum , int size , int fb ,int handle);
void Insertstr(char FileName[] , FILE* tempfile , int LineNumber , int CharNum , int handle);
void Copy(char FileName[] ,int LineNum , int CharNum , int size , int fb , int handle);
void Cut(char FileName[] ,int LineNum , int CharNum , int size , int fb);
void Paste(char FileName[] ,int LineNum , int CharNum);
void UndoHelper(FILE *file);
void Undo(char FileName[]);
void TextComparator(char File1[] , char File2[]);
void StringCheck(char *String);
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
            CreateFile2(argv[++i] , 0);
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
        else if (!strcmp(argv[i], "undo"))
        {
            if(!strcmp(argv[++i] , "-file")) {
                printf("Invalid Format! --file");
                continue;
            }
            Undo(argv[++i]);
        }
        else if (!strcmp(argv[i], "compare"))
        {
            if(!strcmp(argv[++i] , "-file")) {
                printf("Invalid Format! --file");
                continue;
            }
            TextComparator(argv[i+1] , argv[i+2]);
        }
    }
    return 0;
}

void CreateFile2(char FileName[] , int handle)
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
        if(handle) return;
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
    strcpy(Filename2 , FileName);
    FILE *file = Reference(FileName);
    FILE *destination = fopen("destination.txt", "wb+");
    chdir("D:\\Project");
    UndoHelper(file);
    fseek(file , 0 , SEEK_SET);
    for(int j = 1 ; j < LineNum ; j++) {
        temp = 'A';
        while (temp != '\n') {
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
    file = Reference(Filename2);
    fclose(file);
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
    strcpy(Filename2 , FileName);
    FILE *file = Reference(FileName);
    FILE *destination = fopen("destination.txt", "wb+");
    chdir("D:\\Project");
    UndoHelper(file);
    fseek(file , 0 , SEEK_SET);
    for(int j = 1 ; j < LineNum ; j++) {
        temp = 'A';
        while (temp != '\n') {
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
    chdir(FileName);
    fclose(file);
    fclose(destination);
    file = Reference(Filename2);
    fclose(file);
    remove(ReferenceFileName);
    rename("destination.txt" , ReferenceFileName);
    if(handle) return;
    printf("String Successfully inserted!\n");
    return;
}

void Copy(char FileName[] ,int LineNum , int CharNum , int size , int fb ,int handle)
{
    char temp;
    FILE *file = Reference(FileName);
    chdir("D:\\Project");
    chdir("system");
    DeleteFileA("clipboard.txt");
    FILE *clipboard = fopen("clipboard.txt", "wb+");
    fseek(file , 0 , SEEK_SET);
    for(int j = 1 ; j < LineNum ; j++) {
        temp = 'A';
        while (temp != '\n') {
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
        if(temp2 == EOF)
        {
            printf("Pointer Reached End of File\n");
            exit(1);
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
    strcpy(Filename3 , FileName);
    Copy(FileName , LineNum , CharNum , size ,fb ,1);
    Removestr(Filename3 , LineNum , CharNum , size , fb ,1);
    printf("String Successfully Cut\n");
    return;
}

void Paste(char FileName[] ,int LineNum , int CharNum)
{
    chdir("D:\\Project");
    chdir("system");
    FILE *tempfile = fopen("clipboard.txt" , "rb+");
    chdir("..");
    Insertstr(FileName , tempfile , LineNum , CharNum , 1);
    printf("String Successfully Pasted!\n");
    return;
}

void Undo(char FileName[])
{
    FILE *file = Reference(FileName);
    file = freopen(ReferenceFileName , "wb+" , file);
    char temp[1000];
    char temp2[1000];
    sprintf(temp, "undo%s", ReferenceFileName);
    chdir("D:\\Project");
    chdir("system");
    if (chdir(temp) == -1) {
        printf("There Is No action To Undo!!\n");
    }
    char k;
    char *token = strtok(temp , ".");
    int j , Numpointer = strlen(token);
    for(j = 0;j < Numpointer;j++)
    {
        temp2[j] = token[j];
    }
    temp2[Numpointer] = '0';
    temp2[Numpointer+1] = '.';
    token =strtok(NULL , ".");
    for(j = 2;j <= strlen(token)+1;j++)
    {
        temp2[Numpointer + j] = token[j-2];
    }
    temp2[Numpointer + j] = '\0';
    for(k = 1;;k++)
    {
        temp2[Numpointer] = k + '0';
        if(access(temp2 , R_OK) == -1) {
            break;
        }
    }
    temp2[Numpointer] = k-1 + '0';
    FILE *ToBeDeleted = fopen(temp2 , "rb+");
    if(!ToBeDeleted)
    {
        printf("There is No action To Undo!\n");
        return;
    }
    fseek(ToBeDeleted , 0 ,SEEK_SET);
    while(1)
    {
        int tempchar = fgetc(ToBeDeleted);
        if(tempchar == EOF) break;
        fputc(tempchar , file);
    }
    fclose(ToBeDeleted);
    fclose(file);
    DeleteFileA(temp2);
    chdir("D:\\Project");
    printf("Undoed Successfully\n");
}

void TextComparator(char File1[] ,char File2[])
{
    FILE *file1 = Reference(File1) , *file2 = Reference(File2);
    fseek(file1 , 0 , SEEK_SET); fseek(file2 , 0 , SEEK_SET);
    int j = 1, flag = 0;
    char LOF1[10000] , LOF2[10000];
    while(1)
    {
        fgets(LOF1 , 10000, file1);
        fgets(LOF2 , 10000, file2);
        StringCheck(LOF2);
        StringCheck(LOF1);
        if(strcmp(LOF1 , LOF2))
        {
            printf("============ #%d ============\n%s\n%s\n" , j , LOF1 , LOF2);
        }
        j++;
        if(feof(file1) && !feof(file2)) {
            flag = 2;
            break;
        }
        if(!feof(file1) && feof(file2)) {
            flag = 1;
            break;
        }
        if(feof(file1) && feof(file2)) {
            flag = 1;
            break;
        }
    }
    if(flag == 1) {
        while (1) {
            if (feof(file1))
                return;
            fgets(LOF1, 10000, file1);
            StringCheck(LOF1);
            printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n%s\n", j, j, LOF1);
            j++;
        }
    }
    if(flag == 2) {
        while (1) {
            if (feof(file2))
                return;
            fgets(LOF2, 10000, file2);
            StringCheck(LOF2);
            printf("<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n%s\n", j, j, LOF2);
            j++;
        }
    }


}










void StringCheck(char *String)
{
    int length = strlen(String);
    if(String[length - 1] == '\n')
        String[length - 1] = '\0';
}
void UndoHelper(FILE *file) {
    char temp[1000];
    char temp2[1000];
    sprintf(temp, "undo%s", ReferenceFileName);
    chdir("system");
    if (chdir(temp) == -1) {
        mkdir(temp);
        chdir(temp);
    }
    char k;
    char *token = strtok(temp , ".");
    int j , Numpointer = strlen(token);
    for(j = 0;j < Numpointer;j++)
    {
        temp2[j] = token[j];
    }
    temp2[Numpointer] = '0';
    temp2[Numpointer+1] = '.';
    token =strtok(NULL , ".");
    for(j = 2;j <= strlen(token)+1;j++)
    {
        temp2[Numpointer + j] = token[j-2];
    }
    temp2[Numpointer + j] = '\0';
    for(k = 1;;k++)
    {
        temp2[Numpointer] = k + '0';
        if(access(temp2 , R_OK) == -1) {
            FILE *UndoTemp = fopen(temp2 ,"w");
            fseek(file , 0 ,SEEK_SET);
            while(1)
            {
                int tempchar = fgetc(file);
                if(tempchar == EOF) break;
//                printf("%c" , tempchar);
                fputc(tempchar , UndoTemp);
            }
            fclose(UndoTemp);
            fseek(file , 0 , SEEK_SET);
            SetFileAttributesA(temp2 , FILE_ATTRIBUTE_HIDDEN);
            chdir("..");
            sprintf(temp, "undo%s", ReferenceFileName);
            SetFileAttributesA(temp , FILE_ATTRIBUTE_HIDDEN);
            chdir("..");
            break;
        }
    }
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
        printf("Invalid File Name! %s\n" , token);
        exit(-1);
    }
}