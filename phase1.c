#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fileapi.h>
#include <conio.h>
char ReferenceFileName[1000] , Filename2[1000] ,Filename3[1000];
void CreateFile2(char FileName[] , int handle);
void Cat(char FileName[] ,FILE *output);
void Removestr(char FileName[] ,int LineNum , int CharNum , int size , int fb ,int handle);
void Insertstr(char FileName[] , FILE* tempfile , int LineNumber , int CharNum , int handle);
void Copy(char FileName[] ,int LineNum , int CharNum , int size , int fb , int handle);
void Cut(char FileName[] ,int LineNum , int CharNum , int size , int fb);
void Paste(char FileName[] ,int LineNum , int CharNum);
void UndoHelper(FILE *file);
void Undo(char FileName[]);
void TextComparator(char File1[] , char File2[] , FILE *output);
void StringCheck(char *String);
void AutoIndent(char FileName[]);
FILE *Reference(char FileName[]);
void DirectoryTree( char *FileName, int Spaces , int MaxDepth ,FILE *output);
void Arman(int argc , int i , char **argv ,FILE *file);
void grep(int n , char** FileNameAry , char option , char* pattern , FILE* output);
void find(char *FileName , char *pattern , int all , int at  , int byword , int count , FILE* output);
int  StringFormat(char *s , int size ,int handle);
void Replace(char String1[] , char String2[] , char FileName[] , int all , int at);
char Curr_Address[1000];
int main(int argc , char *argv[])
{
    FILE *file;
    getcwd(Curr_Address , 1000);
    int i = 0;
    if(i < argc) {
        i++;
        if (!strcmp(argv[i], "createfile"))
        {
            if(strcmp(argv[++i] , "--file") != 0) {
                printf("Invalid Format! --file");
            }
            CreateFile2(argv[++i] , 0);
        }
        else if(!strcmp(argv[i] , "cat")) {
            if (strcmp(argv[++i], "--file") != 0) {
                printf("Invalid Format! --file");
            }
            if(i + 2 < argc)
            {
                 FILE *tempfile = fopen("tempfile.txt" , "wb+");
                 Cat(argv[++i] , tempfile);
                 Arman(argc , i + 2 , argv ,tempfile);
                 fclose(tempfile);
                remove("tempfile.txt");
            }
            Cat(argv[++i],stdout);
        }
        else if(!(strcmp(argv[i] , "removestr")))
        {
            if ((strcmp(argv[++i], "--file")) != 0) {
                printf("Invalid Format! --file");
            }
            int FilePointer = ++i;
            if (strcmp(argv[++i], "--pos") != 0 ) {
                printf("Invalid Format! --pos");
            }
            int LineNum , CharNum ,size;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            if (strcmp(argv[++i], "-size") != 0) {
                printf("Invalid Format! -size\n");
            }
            sscanf(argv[++i] , "%d" , &size);
            int fb = argv[++i][1];
            Removestr(argv[FilePointer] , LineNum , CharNum , size , fb , 0);
        }
        else if(!(strcmp(argv[i] , "insertstr")))
        {
            if ((strcmp(argv[++i], "--file")) != 0) {
                printf("Invalid Format! --file");
            }
            int FilePointer = ++i;
            if (strcmp(argv[++i], "--str") != 0) {
                printf("Invalid Format! --str");
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
            if ((strcmp(argv[++i], "--file")) != 0) {
                printf("Invalid Format! --file");
            }
            int FilePointer = ++i;
            if (strcmp(argv[++i], "--pos") != 0) {
                printf("Invalid Format! --pos");
            }
            int LineNum , CharNum ,size;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            if (strcmp(argv[++i], "-size") != 0) {
                printf("Invalid Format! -size\n");
            }
            sscanf(argv[++i] , "%d" , &size);
            int fb = argv[++i][1];
            Copy(argv[FilePointer] , LineNum , CharNum , size , fb , 0);
        }
        else if(!(strcmp(argv[i] , "cutstr")))
        {
            if ((strcmp(argv[++i], "--file")) != 0) {
                printf("Invalid Format! --file");
            }
            int FilePointer = ++i;
            if (strcmp(argv[++i], "--pos") != 0) {
                printf("Invalid Format! --pos");
            }
            int LineNum , CharNum ,size;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            if (strcmp(argv[++i], "-size") != 0) {
                printf("Invalid Format! -size\n");
            }
            sscanf(argv[++i] , "%d" , &size);
            int fb = argv[++i][1];
            Cut(argv[FilePointer] , LineNum , CharNum , size , fb);
        }
        else if(!(strcmp(argv[i] , "pastestr")))
        {
            if ((strcmp(argv[++i], "--file")) != 0) {
                printf("Invalid Format! --file");
            }
            int FilePointer = ++i;
            if (strcmp(argv[++i], "--pos") != 0) {
                printf("Invalid Format! --pos");
            }
            int LineNum , CharNum;
            sscanf(argv[++i] , "%d:%d" , &LineNum , &CharNum);
            Paste(argv[FilePointer] , LineNum , CharNum);
        }
        else if (!strcmp(argv[i], "undo"))
        {
            if(strcmp(argv[++i] , "--file") != 0) {
                printf("Invalid Format! --file");
            }
            Undo(argv[++i]);
        }
        else if (!strcmp(argv[i], "compare"))
        {
            if(strcmp(argv[++i] , "--file") != 0) {
                printf("Invalid Format! --file");
            }
            if(i+3 < argc)
            {
                 FILE *tempfile = fopen("tempfile.txt" , "wb+");
                 TextComparator(argv[i + 1] , argv[i+2] , tempfile);
                 Arman(argc , i + 4  , argv ,tempfile);
                 fclose(tempfile);
                remove("tempfile.txt");
            }
            TextComparator(argv[i+1] , argv[i+2] ,stdout);
        }
        else if (!strcmp(argv[i], "tree"))
        {
            int depth;
            sscanf(argv[++i] , "%d" , &depth);
            if(depth < -1)
            {
                printf("Invalid Depth!\n");
            }
            if(i+1 < argc)
            {
                FILE *tempfile = fopen("tempfile.txt" , "wb+");
                DirectoryTree("root" , 0 , depth , tempfile);
                 Arman(argc , i + 2 , argv ,tempfile);
            }
            DirectoryTree("root" , 0 , depth ,stdout);
//            if(!strcmp(argv[i+1] , "=D"))
//            {
//                Arman(argc , i+2 , argv , file);
//            }
        }
        else if(!strcmp(argv[i] , "auto-indent"))
        {
            if(strcmp(argv[++i] , "--file") != 0)
            {
                printf("You Missed --file\n");

            }
            else AutoIndent(argv[++i]);
        }
        else if(!strcmp(argv[i] , "grep"))
        {
            char option = 0;
            if(strcmp(argv[++i] , "--str") != 0)
            {
                option = argv[i++][1];
            }
            i++;
            char *pattern = argv[i];
            i+=2;
            int n;
            n = argc - i;
            char *FileAry[n];
            for(int j = 0; j < n;j++)
            {
//                if(!strcmp(argv[i+j] , "=D"))
//                {
//                FILE *tempfile = fopen("tempfile.txt" , "wb+");
//                grep(j , FileAry , option , pattern , tempfile);
//                 Arman(argc , j+1 , argv ,tempfile);
//                }
                FileAry[j] = argv[j+i];
            }
//            printf("%s %s %s ")
            grep(n , FileAry , option , pattern , stdout);
        }
        else if(!strcmp(argv[i] , "find"))
        {
            int all = 0 , at = 0, byword = 0, count = 0;
            i++;

            char *s ,*FileName;
            for(;i<argc;i++)
            {
                if(!strcmp(argv[i] , "-at"))
                {
                    sscanf( argv[++i] , "%d"  ,&at);
                }
                if(!strcmp(argv[i] , "-all"))
                {
                    all = 1;
                }
                if(!strcmp(argv[i] , "-byword")) {
                    byword = 1;
                }
                if(!strcmp(argv[i] , "-count"))
                {
                    count = 1;
                }
                if(!strcmp(argv[i] , "--file"))
                    FileName = argv[++i];
                if(!strcmp(argv[i] , "--str"))
                    s = argv[++i];
                if(!strcmp(argv[i] , "=D"))
                {
                    FILE *tempfile = fopen("tempfile.txt" , "wb+");
                    find(FileName , s , all , at , byword , count , tempfile);
                    Arman(argc , i + 1 , argv ,tempfile);
                }

            }
            find(FileName , s , all , at , byword , count ,stdout);
        }
        else if(!strcmp(argv[i] , "replace"))
        {
            char *str1 , *str2 , *FileName;
            int all = 0 , at = 0;
            for( ; i < argc ; i++)
            {
                if(!strcmp(argv[i] , "--str1"))
                   str1 = argv[++i];
                if(!strcmp(argv[i] , "--str2"))
                    str2 = argv[++i];
                if(!strcmp(argv[i] , "--file"))
                    FileName = argv[++i];
                if(!strcmp(argv[i] , "-at"))
                    sscanf(argv[++i] , "%d" , &at);
                if(!strcmp(argv[i] , "-all"))
                    all = 1;
                if (all && at)
                {
                    printf("You Can Not Use At And All At Once!\n");
                    return 0;
                }
            }
            Replace(str1 , str2 , FileName , all , at);
        }
        else
        {
            printf("Invslid Command!\n");
            exit(-1);
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
    temp = fopen(token , "rb+");
    if(temp) {
        printf("The File Already Exists!\n");
        fclose(temp);
    }
    else {
        temp = fopen(token, "w+");
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

void Cat(char FileName[],FILE *output)
{
    FILE *file = Reference(FileName);
    int pointer;
    while(1)
    {
        pointer = fgetc(file);
        if(feof(file))
            break;
        fprintf(output,"%c" , pointer);
    }
    fclose(file);

    return;
}

void Removestr(char FileName[] ,int LineNum , int CharNum , int size , int fb , int handle)
{
    int temp;
    strcpy(Filename2 , FileName);
    char FileName4[10000];
    strcpy(FileName4 , "temp");
    strcat(FileName4 , ReferenceFileName);
    FILE *file = Reference(FileName);
    FILE *destination = fopen(FileName4, "wb+");
        chdir(Curr_Address);
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
        rename(FileName4 , ReferenceFileName);
        if(handle)
            return;
        printf("File Successfully Edited!\n");
        return;
    }

void Insertstr(char FileName[] , FILE *tempfile , int LineNum , int CharNum ,int handle)
{
    int temp , flag = 0;
    strcpy(Filename2 , FileName);
    char FileName4[10000];
    strcpy(FileName4 , "temp");
    strcat(FileName4, ReferenceFileName);
    chdir(Curr_Address);
    FILE *file = Reference(FileName);
    FILE *destination = fopen(FileName4 , "wb+");
    chdir(Curr_Address);
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
        if(temp == '\n')
        {
            printf("There is not enough character in that line!\n");
            return;
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
        else if(flag && temp == '\'')
        {
            fprintf(destination , "\'");
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
    remove("tempfile.txt");
    chdir(Curr_Address);
    while(1)
    {
        int temp2 = fgetc(file);
        if(feof(file))
            break;
        fprintf(destination, "%c", temp2);
    }
//    chdir(FileName);
    fclose(file);
    fclose(destination);
    file = Reference(Filename2);
    fclose(file);
    remove(ReferenceFileName);
    rename(FileName4 , ReferenceFileName);
    if(handle) return;
    printf("String Successfully inserted!\n");
    return;
}

void Copy(char FileName[] ,int LineNum , int CharNum , int size , int fb ,int handle)
{
    char temp;
    FILE *file = Reference(FileName);
    chdir(Curr_Address);
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
    chdir(Curr_Address);
    strcpy(Filename3 , FileName);
    Copy(FileName , LineNum , CharNum , size ,fb ,1);
    Removestr(Filename3 , LineNum , CharNum , size , fb ,1);
    printf("String Successfully Cut\n");
    return;
}

void Paste(char FileName[] ,int LineNum , int CharNum)
{
    chdir(Curr_Address);
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
    chdir(Curr_Address);
    chdir("system");
    if (chdir(temp) == -1) {
        printf("There Is No action To Undo!!\n");
        fclose(file);
        return;
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
        fclose(file);
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
    chdir(Curr_Address);
printf("Undoed Successfully\n");
}

void TextComparator(char File1[] ,char File2[] , FILE *output)
{
    chdir(Curr_Address);
    FILE *file1 = Reference(File1);
    chdir(Curr_Address);
    FILE *file2 = Reference(File2);
    fseek(file1 , 0 , SEEK_SET); fseek(file2 , 0 , SEEK_SET);
    int j = 1, flag = 0, pos ,counter = 0;
    char LOF1[10000] , LOF2[10000];
    while(1)
    {
        fgets(LOF1 , 10000, file1);
        fgets(LOF2 , 10000, file2);
        StringCheck(LOF2);
        StringCheck(LOF1);
        if(strcmp(LOF1 , LOF2))
        {
            fprintf(output , "============ #%d ============\n%s\n%s\n" , j , LOF1 , LOF2);
        }
        j++;
        if(feof(file1) && !feof(file2)) {
            flag = 2;
            pos = ftell(file2);
            break;
        }
        if(!feof(file1) && feof(file2)) {
            flag = 1;
            pos = ftell(file1);
            break;
        }
        if(feof(file1) && feof(file2)) {
            flag = 0;
            break;
        }
    }
    if(flag == 1) {
        while(1)
        {
            char c = fgetc(file1);
            if(c == EOF)
                break;
            if(c == '\n')
                counter++;
        }
        fseek(file1,pos , SEEK_SET);
        fprintf(output, ">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n" , j , j+counter);
        while (1) {
            if (feof(file1))
                return;
            fgets(LOF1, 10000, file1);
            StringCheck(LOF1);
            fprintf(output, "%s\n" , LOF1);
            j++;
        }
    }
    if(flag == 2) {
        while(1)
        {
            char c = fgetc(file2);
            if(c == EOF)
                break;
            if(c == '\n')
                counter++;
        }
        fseek(file2,pos , SEEK_SET);
        fprintf(output, "<<<<<<<<<<<< #%d - #%d <<<<<<<<<<<<\n" , j , j+counter);
        while (1) {
            if (feof(file2))
                return;
            fgets(LOF2, 10000, file2);
            StringCheck(LOF2);
            fprintf(output, "%s\n" , LOF2);
            j++;
        }
    }


}

void DirectoryTree( char *FileName, int Spaces , int MaxDepth ,FILE *output)
{
    if(Spaces / 3 > MaxDepth && MaxDepth != -1)
        return;
    DIR *folder;
    struct dirent *Files;

    if (!(folder = opendir(FileName)))
        return;

    while ((Files = readdir(folder)) != NULL) {
        if (!strchr(Files->d_name , '.')) {
            char path[1024];
            if (strcmp(Files->d_name, ".") == 0)
                continue;
            if (strcmp(Files->d_name, "..") == 0)
                continue;
            sprintf(path,  "%s/%s", FileName, Files->d_name);
            fprintf(output , "%*s[%s]\n", Spaces, "", Files->d_name);
            DirectoryTree(path, Spaces + 3 , MaxDepth , output);
        } else {
            if (strcmp(Files->d_name, ".") == 0)
                continue;
            if (strcmp(Files->d_name, "..") == 0)
                continue;
            fprintf(output , "%*s%s\n", Spaces, "-", Files->d_name);
        }
    }
    closedir(folder);
}

void Arman(int argc , int i , char **argv , FILE* tempfile)
{
    if(!(strcmp(argv[i] , "insertstr")))
    {
        int LineNum , CharNum;
        sscanf(argv[i+4] , "%d:%d" , &LineNum , &CharNum);
        Insertstr(argv[i+2] , tempfile , LineNum , CharNum , 0);

        exit(0);
    }
    if(!(strcmp(argv[i] , "find")))
    {
            int all = 0 , at = 0, byword = 0, count = 0;
            i++;

            char s[1000] ,*FileName;
            fseek(tempfile , 0 ,SEEK_SET);
            int k = 0;
           for(;;k++) {
               char ch = getc(tempfile);
               if (ch == EOF)
                   break;
               s[k] = ch;
           }
           fclose(tempfile);
           remove("tempfile.txt");
           s[k] = '\0';
            for(;i<argc;i++)
            {
                if(!strcmp(argv[i] , "-at"))
                {
                    sscanf( argv[++i] , "%d"  ,&at);
                }
                if(!strcmp(argv[i] , "-all"))
                {
                    all = 1;
                }
                if(!strcmp(argv[i] , "-byword")) {
                    byword = 1;
                }
                if(!strcmp(argv[i] , "-count"))
                {
                    count = 1;
                }
                if(!strcmp(argv[i] , "--file"))
                    FileName = argv[++i];

            }
            find(FileName , s , all , at , byword , count ,stdout);
        exit(0);
    }
    else if(!strcmp(argv[i] , "grep"))
    {
        printf("Sorry No Grep :(((\n");
        exit(-1);
    }
    else
    {
        printf("That command does not take an string!!\n");
        exit(-1);
    }
//    fclose(file);
//    char * tempname2;
//    sprintf(tempname2 , "Out%d.txt" , i);
//    remove(tempname2);
}

void AutoIndent(char FileName[])
{
    chdir(Curr_Address);
    strcpy(Filename2 , FileName);
    FILE *file = Reference(FileName) , *dest = fopen("Dest.txt" , "wb+");
    chdir(Curr_Address);
    UndoHelper(file);
    char Buffer[10000];
    fseek(file , 0 , SEEK_SET);
    fgets(Buffer , 10000 , file);
    int length = strlen(Buffer);
    char s[length + 10];
    int k = 0 , flag = 0 , counterspace = 0;
    for(int j = 0;j< length;j++)
    {
        if(Buffer[j] == '{' || Buffer[j] == '}')
        {
            flag = 0;
            s[k++] = Buffer[j];
            counterspace = 0;
        }
        else if (Buffer[j] == ' ')
        {
            counterspace++;
        }
        else
        {
            if(flag == 1)
            {
                for(int ff =0; ff<counterspace;ff++)
                    s[k++] = ' ';
            }
            else flag = 1;
            counterspace = 0;
            s[k++] = Buffer[j];
        }
    }
    int tabcounter = 0;
    s[k] = '\0';
    fputc(s[0] , dest);
    if(s[0] == '{')
    {
        fputs("\n" , dest);
        tabcounter++;
    }printf("%s\n" , s);
    for(int j = 1; j < k; j++)
    {
        if(s[j] == '{')
        {
            if(s[j-1] == '{')
            {
                fprintf(dest , "%*s%s" ,tabcounter * 4 , "" ,"{");
            }
            else
            {
//                fputc('\n' , dest);
                fprintf(dest , "%s"  , " {");
            }
            tabcounter++;
            fputc('\n' ,dest);
        }
        else if(s[j] == '}')
        {
            if(s[j-1] == '{' || s[j-1] == '}')
            {
                tabcounter--;
                fprintf(dest , "%*s%s" , tabcounter * 4 , "", "}");
            }
            else {
                fputc('\n', dest);
                tabcounter--;
                fprintf(dest, "%*s%s", tabcounter * 4 , "", "}");
            }
            fputc('\n' , dest);
        }
        else
        {
            fprintf(dest , "%*s%c" ,tabcounter * 4 , "" , s[j]);
            while(s[j+1] != '{' && s[j+1] != '}' && j < k)
            {
                fputc(s[++j] , dest);
            }
        }
    }
    fclose(dest);
    fclose(file);
    file = Reference(Filename2);
    fclose(file);;
    remove(ReferenceFileName);
    rename("Dest.txt" , ReferenceFileName);
    printf("Auto-Indented\n");
}

int GrepHepler(FILE *file , char* FileName , char* pattern , char option , FILE *output)
{
    fseek(file , 0 , SEEK_SET);
    int counter = 0;
    int flag = 0;
    char Buffer[1000];
    while(1)
    {
        fgets(Buffer , 1000 , file);
        if(strstr(Buffer , pattern))
        {
//            printf("%s" , Buffer);
            if(option == 'c')
                counter++;
            else if(option == 'l')
                flag = 1;
            else
            {
                fprintf(output , "%s: %s" , FileName , Buffer);
            }
        }
        if(feof(file))
        {
            printf("\n");
            break;
        }
    }
    if(flag)
        fprintf(output , "%s\n" , FileName);
    return counter;
}
void grep(int n , char** FileNameAry , char option , char* pattern , FILE *output)
{
    FILE *FileAry[n];
    char temp[1000];
    char FileNames[n][100];
    StringFormat(pattern , strlen(pattern) , 0);
    for(int i = 0;i < n;i++)
    {
        strcpy(temp , FileNameAry[i]);
        chdir(Curr_Address);
        FileAry[i] = Reference(temp);
        fseek(FileAry[i] , -1 , SEEK_END);
        if(fgetc(FileAry[i]) == '\n')
        {
            fseek(FileAry[i] , -1 , SEEK_END);
            fputs(" ", FileAry[i]);
        }
        strcpy(FileNames[i] , ReferenceFileName);
    }
    int sum = 0;
    for(int i = 0;i < n;i++)
    {
        sum += GrepHepler(FileAry[i] , FileNames[i] , pattern , option , output);
//        printf("%d " , sum);
    }
    if(option == 'c')
        fprintf(output , "%d\n" , sum);
    for(int i = 0; i < n;i++)
        fclose(FileAry[i]);

}

long long FindFirstWord(FILE *file , int at , char FirstPattern)
{
//    fseek(file , 0 , SEEK_SET);
    int counter = 0;
    while(1)
    {
        char c = getc(file);
        if(c == EOF)
        {
            return -1;
        }
        if(c == FirstPattern)
            counter++;
        if(counter == at)
            return ftell(file);
    }
}
long long BackStar(FILE *file , long long index)
{
    fseek(file , 0 , SEEK_SET);
    long long int pointer;
    while(1)
    {
        char cc = getc(file);
        int temp = ftell(file);
        if(cc == ' ' || temp == 1 || cc == '\n')
        {
            pointer = ftell(file);
        }
        if(temp == index)
            break;
        if(cc == EOF)
            break;
    }
    if(pointer == 1) return 1;
    return pointer + 1;
}
long long GetWord(FILE *file , long long index)
{
    fseek(file , 0 , SEEK_SET);
    long long WordCounter = 0;
    while(1)
    {
        if(feof(file))
            return -1;
        char c = 'a';
        while(c != '\n' && c != ' ' && c != EOF)
            c =getc(file);
        WordCounter++;
        if(ftell(file) >= index)
        {
            return WordCounter;
        }
    }
}

void find(char *FileName , char *pattern , int all , int at  , int byword , int count , FILE *output)
{
    chdir(Curr_Address);
    FILE *file = Reference(FileName);
    if(!at && !all && !count)
        at = 1;
    int result = StringFormat(pattern , strlen(pattern) , 1);
    if((all & count) || (all & at) || (count & at))
    {
        printf("Wrong Combination Of Switches!\n");
        exit(-1);
    }
    int counter = 0;
    if(count)
    {
        int i;
        for(i = 1; 1;i++)
        {
            long long int pointer = FindFirstWord(file , 1 , pattern[0]);
            if(pointer == -1)
            {
                i--;
                break;
            }
            int flag = 1;
            for(int j = 1; j < strlen(pattern) ;j++)
            {
                if(pattern[j] != getc(file))
                {
                    flag = 0;
                    break;
                }
            }
            if(flag) {
                counter++;
                char c = 'A';
                if(byword)
                {
                    while(c != '\n' && c !=EOF && c != ' ')
                    {
                        c = getc(file);
                    }
                }
            }
        }
        fprintf(output , "%d\n" , counter);
    }
    if(at)
    {
        int i;
        for(i = 1; 1;i++)
        {
            long long int pointer = FindFirstWord(file , 1 , pattern[0]);
            if(pointer == -1)
            {
                i--;
                fprintf(output , "%d" , -1);
                break;
            }
            int flag = 1;
            for(int j = 1; j < strlen(pattern) ;j++)
            {
                if(pattern[j] != getc(file))
                {
                    flag = 0;
                    break;
                }
            }
            if(flag) {
                counter++;
                char c = 'A';
                if(byword)
                {
                    while(c != '\n' && c !=EOF && c != ' ')
                    {
                        c = getc(file);
                    }
                }
            }
            if(counter == at)
            {
                if(byword) fprintf(output , "%lld\n" , GetWord(file , pointer));
                else if(result == 1 || result == 3)
                {
                    fprintf(output , "%lld\n" , BackStar(file , pointer));
                }
                else fprintf(output ,"%lld\n" , pointer);
                return;
            }
        }
    }
    if(all)
    {
        int i , check = 0;
        for(i = 1; 1;i++)
        {
            long long int pointer = FindFirstWord(file , 1 , pattern[0]);
            if(pointer == -1)
            {
                i--;
                break;
            }
            int flag = 1;
            for(int j = 1; j < strlen(pattern) ;j++)
            {
                if(pattern[j] != getc(file))
                {
                    flag = 0;
                    break;
                }
            }
            if(flag) {
                counter++;
                char c = 'A';
                if(byword)
                {
                    fprintf(output , "%lld " , GetWord(file , pointer));
                    check++;
                    fseek(file , (int)pointer , SEEK_SET);
                    while(c != '\n' && c !=EOF && c != ' ')
                    {
                        c = getc(file);
                    }
                }
                else if(result == 1 || result == 3)
                {
                    fprintf(output , "%lld " , BackStar(file , pointer));
                    check++;
                    fseek(file ,(int)pointer , SEEK_SET);
                }
                else {
                    fprintf(output , "%lld " , pointer);
                    check++;
                }
            }
        }
        if(!check) fprintf( output , "-1");
        fprintf(output , "\n");
    }
    fclose(file);
}
char FileName4[10000];
long long ReplaceHelper(FILE *file , char String1[] , char String2[] , long long int StartPos , long long int EndPos , int result)
{
    fseek(file , 0 , SEEK_SET);
    FILE *temp = fopen(FileName4 , "wb+");
    int j = 0 , returnvalue ;
    for(;j < StartPos - 1;j++)
    {
        int c = fgetc(file);
        fputc(c , temp);
    }
    int cc = 0;
    if(result == 2 || result == 3)
    {
        while(1)
        {
            int c = fgetc(file);
            if (c == EOF)
                break;
            if(c == ' ')
            {
                cc = ' ';
                break;
            }
            if(c == '\n')
            {
                cc = '\n';
                break;
            }
            if(c == '\r') {
                cc == '\n';
                break;
            }
        }
        fprintf(temp , "%s" , String2);
        if(cc) fputc(cc , temp);
        returnvalue = ftell(temp);
    }
    else
    {
        for(;j < EndPos - 1;j++)
        {
            int c = fgetc(file);
        }
        fprintf(temp , "%s" , String2);
        returnvalue = ftell(temp);
    }
    while(1)
    {
        int c = fgetc(file);
        if(c == EOF)
            break;
        fputc(c , temp);
    }
    fclose(temp);
    fclose(file);
    remove(ReferenceFileName);
    rename(FileName4 , ReferenceFileName);
    return returnvalue;
}

void Replace(char String1[] , char String2[] , char FileName[] , int all , int at)
{
    strcpy(FileName4 , "temp");
    strcat(FileName4 , ReferenceFileName);
    strcpy(Filename2 , FileName);
    FILE *file = Reference(FileName);
    if(!at && !all)
        at = 1;
    int result1 = StringFormat(String1 , strlen(String1) , 1);
    StringFormat(String2 , strlen(String2) , 0);
    chdir(Curr_Address);
    UndoHelper(file);
    fclose(file);
    file = Reference(Filename2);
    fseek(file , 0 ,SEEK_SET);
    int counter = 0;
   if(at)
    {
        int i;
        for(i = 1; 1;i++)
        {
            long long int pointer = FindFirstWord(file , 1 , String1[0]);
            if(pointer == -1)
            {
                i--;
                printf("%d" , -1);
                break;
            }
            int flag = 1;
            for(int j = 1; j < strlen(String1) ;j++)
            {
                if(String1[j] != getc(file))
                {
                    flag = 0;
                    break;
                }
            }
            long long int EndPos;
            if(flag) {
                counter++;
                char c = 'A';
            }
            if(counter == at)
            {
                EndPos = pointer + strlen(String1);
                if(result1 == 1 || result1 == 3)
                {
                    ReplaceHelper(file , String1 , String2 , BackStar(file ,pointer) , EndPos , result1);
                }
                else ReplaceHelper(file , String1 , String2 , pointer , EndPos , result1);
                return;
            }
        }
    }
    if(all)
    {
        int i , check = 0;
        for(i = 1; 1;i++)
        {
            long long int pointer = FindFirstWord(file , 1 , String1[0]);
            if(pointer == -1)
            {
                i--;
                break;
            }
            int flag = 1;
            for(int j = 1; j < strlen(String1) ;j++)
            {
                if(String1[j] != getc(file))
                {
                    flag = 0;
                    break;
                }
            }
            if(flag) {
                counter++;
                char c = 'A';
                int EndPos = pointer + strlen(String1);
                if(result1 == 1 || result1 == 3)
                {
                    long long int pointer2 = ReplaceHelper(file , String1 , String2 , BackStar(file ,pointer) , EndPos , result1);
                    check++;
                    file = fopen(ReferenceFileName , "rb+");
                    fseek(file ,(int) pointer2 , SEEK_SET);
                }
                else {
                    long long int pointer2 = ReplaceHelper(file , String1 , String2 , pointer, EndPos , result1);
                    check++;
                    file = fopen(ReferenceFileName , "rb+");
                    fseek(file , (int) pointer2 , SEEK_SET);
                }
            }
        }
        if(!check) printf("-1");
        else printf("Replaced!\n");
    }
}



int StringFormat(char *s ,int size , int handle)
{
    char Buffer[size];
    Buffer[0] = '\0';
    int result = 0;
    if(handle && s[size - 1]  == '*' && s[size - 2] != '\\')
    {
        result += 2;
        s[size - 1] = '\0';
    }
    int flag = 0;
    for(int i =0; i < size;i++)
    {
        char temp = s[i];
        if(handle && i == 0 && temp == '*')
        {
            result += 1;
            continue;
        }
        if(temp == '\\' && !flag)
        {
            flag = 1;
            continue;
        }
        else if(flag && temp == '\\')
        {
            strcat(Buffer , "\\");
            flag = 0;
            continue;
        }
        else if(flag && temp == 'n')
        {
            strcat(Buffer , "\n");
            flag = 0;
            continue;
        }
        else if(flag && temp == '\'')
        {
            strcat(Buffer , "\'");
            flag = 0;
            continue;
        }
        else if(temp == '\'') {
            strcat(Buffer , "\"");
            continue;
        }
        else if(temp == '*' && flag && handle)
        {
            strcat(Buffer , "*");
            flag = 0;
            continue;
        }
        strncat(Buffer , &temp , 1);
        flag = 0;
    }

    strcpy(s , Buffer);
    return result;
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
    if(chdir("system") == -1)
    {
        mkdir("system");
        SetFileAttributesA("system" , FILE_ATTRIBUTE_HIDDEN);
        chdir("system");
    }
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
            FILE *UndoTemp = fopen(temp2 ,"wb+");
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
            printf("Invalid File Path! %s\n" , token);
            exit(-1);
        }
        token = strtok(NULL , "/");

    }
    FILE *temp;
    sprintf(ReferenceFileName,"%s", token);
    temp = fopen(token , "rb+");
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