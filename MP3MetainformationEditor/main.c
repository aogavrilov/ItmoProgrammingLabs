#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h> // для работы SetConsoleCP, SetConsoleOutputCP
#include <locale.h>
unsigned int reverseBytes (unsigned int n)
{
    return ((n >> 24) & 0x000000ff) |
           ((n >> 8)  & 0x0000ff00) |
           ((n << 8)  & 0x00ff0000) |
           ((n << 24) & 0xff000000);
}
char* concat(char *s1, char *s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);
    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    return result;
}
struct flags{
    unsigned asinhronization : 1;
    unsigned extended_header : 1;
    unsigned experemental_indicator : 1;
};
struct header{
    char id3[3];
    char version[2];
    char flag;
    int size;
};
struct headerFrame{
    char id[4];
    int size;
    unsigned short flags;
};
union fHeader{
    char buffer[10];
    struct headerFrame header_;
};
enum typeCall{
    get,
    show,
    set
};
struct metainformation{


};
int readFrame(FILE* input, char tag_f[4], size_t position, enum typeCall type){
    struct headerFrame hd;
   // int tmp = 0;

    fseek(input, position, SEEK_SET);
    fread(&hd.id, 4, 1, input);
    fread(&hd.size, 4, 1, input);
    fread(&hd.flags, 2, 1, input);
    int size = reverseBytes(hd.size);
    switch(type){
        case get:{
            //printf("%d",size);
            if(!strcmp(hd.id, tag_f)){

                char info[size];
                fread(&info, size, 1, input);
                CharToOem(info,info);
                printf("%s:\t", tag_f);
                for(int i = 0; i < size; i++){

                    printf("%c", info[i]);
                }
                return -1;
            }
            else if(size > 0){
                //printf("|%d|", position);
                return position + 10 + size;
            }
            else
                return -1;
        }
        case show:{
            char info[size];
            fread(&info, 1, size, input);
            CharToOem(info,info);
            if(size > 0){
                printf("ID: %s\tValue: ",hd.id);
                for(int i = 0; i < size; i++){
                    printf("%c",info[i]);
                }
                printf("\n");
            }
            else
                return -1;
        }
        case set:{
            if(!strcmp(hd.id, tag_f)){
                char info[size];
                fread(&info, size, 1, input);
                CharToOem(info,info);
                for(int i = 0; i < size; i++){

                    printf("%c", info[i]);
                }
                return -1;
            }
            else if(size > 0){
                //printf("|%d|", position);
                return position + 10 + size;
            }
            else
                return -1;
        }

    }

    //printf("%s\n", hd.id);
  //  printf("%d\n", size);
  //  printf("%c%c", hd.flags);
    return position + 10 + size;
}
int setprop(FILE* input, char tag_f[4],char* info, char* argv[]){
    size_t pos = 10;
    size_t i_size = strlen(info);
    fseek(input, 0, SEEK_END);
    char* name = strpbrk(argv[1], "=") + 1;
    FILE* outputtmp = fopen(concat(name, "temp"), "wb");
    size_t length = ftell(input);
    fseek(input, 10, SEEK_SET);
    int size = 0;
    struct headerFrame hd;
    while(pos != -1 && pos < length - 10){
        fseek(input, pos, SEEK_SET);
        fread(&hd.id, 4, 1, input);
        fread(&hd.size, 4, 1, input);
        fread(&hd.flags, 2, 1, input);
        size = reverseBytes(hd.size);
        if(!strcmp(hd.id, tag_f)){
            pos = pos + 10 + size;
            break;
        }
        else if(size != 0){
            pos = pos + 10 + size;
        }
        else{
            pos = -1;
            break;
        }
    }
    if(pos == -1){
        fseek(input, 0, SEEK_SET);
        char header[10];
        fread(&header, 10, 1, input);
        fwrite(header, 10, 1, outputtmp);
        union fHeader frameHeader;
        for(int i = 0; i < 4; i++)
            frameHeader.header_.id[i] = tag_f[i];
        frameHeader.header_.size = reverseBytes(i_size);
        frameHeader.header_.flags = 0;
        fwrite(frameHeader.buffer, 1, 10, outputtmp);
        fwrite(info, 1, i_size, outputtmp);
        fseek(input, 10, SEEK_SET);
        for(int i = 0; i < length; i++){
            char tmp;
            tmp = getc(input);
            putc(tmp,outputtmp);
        }
    }
    else{
        fseek(input, 0, SEEK_SET);
        char header[pos-10-size];
        fread(&header, pos-10-size, 1, input);
        fwrite(header, pos-10-size, 1, outputtmp);
        union fHeader frameHeader;
        for(int i = 0; i < 4; i++)
            frameHeader.header_.id[i] = tag_f[i];
        frameHeader.header_.size = reverseBytes(i_size);
        fwrite(frameHeader.buffer, 1, 10, outputtmp);
        fwrite(info, 1, i_size, outputtmp);
        fseek(input, pos, SEEK_SET);
      //  printf("%s", frameHeader.buffer);
        for(int i = pos; i < length; i++){
            char tmp;
            tmp = getc(input);
            //printf("%c", tmp);
            putc(tmp, outputtmp);
        }
    }
   // remove(argv[1] );

  //  printf("\n%s %s",concat(name, "temp"), strpbrk(argv[1], "=") + 1);
    CopyFile(concat(name, "temp"), strpbrk(argv[1], "=") + 1, 0);

    fclose(outputtmp);
    remove(concat(name, "temp"));
    return 0;
}
int main(int argc, char* argv[])
{
    SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
    setlocale(LC_ALL,"Rus");
    FILE* fo = fopen(strpbrk(argv[1],"=") + 1, "rb");
    if(!fo){
        printf("No file in such directory");
        return 1;

    }
   // printf("%c",argv[2][2]/*, argv[3]*/);
    if(argv[2][2] == 'g'){
        size_t pos = 10;
        fseek(fo, 0, SEEK_END);
        size_t length = ftell(fo);
        //printf("%s",strpbrk(argv[2], "=") + 1);
        while(pos != -1 && pos < length - 10){
            pos = readFrame(fo, strpbrk(argv[2], "=") + 1, pos, get);
        }
    }
    else if(!strcmp(argv[2], "--show")){
        size_t pos = 10;
        fseek(fo, 0, SEEK_END);
        size_t length = ftell(fo);
        while(pos != -1 && pos < length - 10){
            pos = readFrame(fo, "", pos, show);
        }
    }
    else if(argv[2][2] == 's'){
        setprop(fo, strpbrk(argv[2], "=") + 1 , strpbrk(argv[3], "=") + 1, argv);
    }
    return 0;
}
