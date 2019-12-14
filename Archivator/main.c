#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------Vars-------------

char* namedir = "";
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
union s_{
    char bytes[4];
    int size;
};

//---------------------------
unsigned int reverseBytes (unsigned int n)
{
    return ((n >> 24) & 0x000000ff) |
           ((n >> 8)  & 0x0000ff00) |
           ((n << 8)  & 0x00ff0000) |
           ((n << 24) & 0xff000000);
}
void create(char* dir,char* name, size_t argc, char* argv[]){
    FILE* file = fopen(concat(dir, name), "wb");
    if(!file){
        printf("Error, when created file-archive");
        return;
    }
    union s_ tmp;
    tmp.size = reverseBytes(argc);
    fwrite(tmp.bytes, 1, 4, file);
    //fputs(tmp.bytes, file);
    fputc('|', file);
    printf("Archived files:\n");
    for(size_t i = 4; i < argc + 4; i++){
        FILE* exportf = fopen(concat(dir, argv[i]), "rb");
        if(!exportf){
            printf("Error extracting information from %s to archive", argv[i]);
            return;
        }
        fseek(exportf, 0, SEEK_END);
        size_t lenght = ftell(exportf);
        fseek(exportf, 0, SEEK_SET);
        union s_ tmp;
        tmp.size = reverseBytes(lenght);
        fwrite(tmp.bytes, 1, 4, file);
        fputc('|', file);
        fputs(argv[i], file);
        fputc('|', file);
        int a = fgetc(exportf);
        while(a != EOF){
            fputc(a, file);
            a = fgetc(exportf);
        }
        fclose(exportf);
        printf("%d) %s\n", i-3, argv[i]);
    }
    fclose(file);
    return;
}
void extract(char* dir, char* name){
    FILE* file = fopen(concat(dir, name), "rb");
    if(!file){
        printf("Error with opening archive!");
        return;
    }
    union s_ tmp;
    fread(&tmp.size, 1, 4,file);
    size_t count = reverseBytes(tmp.size);
    fgetc(file);
    printf("Extracted files:\n");
    for(size_t i = 0; i < count; i++){
        union s_ tmp_;
        fread(&tmp_.size, 1, 4,file);
        size_t count_ = reverseBytes(tmp_.size);
        fgetc(file);
        size_t tmpsize = 0;
        int pos = ftell(file);
        char symbol = fgetc(file);
        while(symbol != '|'){
            tmpsize++;
            symbol = fgetc(file);

        }
        fseek(file, pos, SEEK_SET);
        char namefile[tmpsize];
        fread(&namefile, 1, tmpsize, file);
        printf("%d) %s\n",i+1, namefile);
        fgetc(file);
        FILE* newfile = fopen(concat(dir, namefile), "wb");
        for(size_t j = 0; j < count_; j++){
            fputc(fgetc(file), newfile);
        }
        fclose(newfile);
    }
}
void list(char* dir, char* name){
    FILE* file = fopen(concat(dir, name), "rb");
    if(!file){
        printf("Error with opening archive!");
        return;
    }
    union s_ tmp;
    fread(&tmp.size, 1, 4,file);
    size_t count = reverseBytes(tmp.size);
    fgetc(file);
    printf("Files in archive:\n");
    for(size_t i = 0; i < count; i++){
        union s_ tmp_;
        fread(&tmp_.size, 1, 4,file);
        size_t count_ = reverseBytes(tmp_.size);
        fgetc(file);
        size_t tmpsize = 0;
        int pos = ftell(file);
        char symbol = fgetc(file);
        while(symbol != '|'){
            tmpsize++;
            symbol = fgetc(file);

        }
        fseek(file, pos, SEEK_SET);
        char namefile[tmpsize];
        fread(&namefile, 1, tmpsize, file);
        printf("%d) %s\n",i+1, namefile);
         fseek(file, pos+tmpsize+count_+1, SEEK_SET);
    }

}

int NoEmptyDir(){
    return (namedir[0] != '\0') ? 1 : 0;
}

int main(int argc, char* argv[])
{
    char dir[strlen(argv[0])-12];
    strncpy(dir, argv[0], strlen(argv[0])-12);
    if(!strcmp(argv[3], "--create")){
        create(dir,argv[2], argc - 4, argv);

    }
    else if(!strcmp(argv[3], "--extract")){
        extract(dir, argv[2]);
    }
    else if(!strcmp(argv[3],"--list")){
        list(dir, argv[2]);
    }
    else{
        printf("enough");
    }
    return 0;
}
