#include <iostream>
#include <fstream>
#include <wrl.h>
#include <vector>
using namespace std;
enum commands{
    InversColor = '0',
    InversHorizontal = '1',
    InversVertical = '2',
    RotateLeft90 = '3',
    RotateRight90 = '4'
};
struct header{
    char type[3];
    size_t x;

    size_t y;
    size_t a;
};
struct RGB{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
struct Image{
    header h;
    vector<RGB> P6;
    void init(int s){
        for(int i = 0; i < s; i++){
            RGB tmp;
            tmp.blue = 0;
            tmp.green = 0;
            tmp.red = 0;
            P6.push_back(tmp);
        }
    }
    vector<char> P5;
};
int saveImage(Image Img, FILE* inpImage, char* address){
    fclose(inpImage);

    FILE* outImage;
    outImage = fopen(address,"wb");
    if(!outImage){
        printf("Error: Invalid error!");
        return 5;
    }

    fprintf(outImage, "%s\n%d %d\n%d\n", Img.h.type, Img.h.x, Img.h.y, Img.h.a);

    if(!strcmp(Img.h.type, "P5")) {
        char* Image1 = new char[Img.h.x * Img.h.y];
        for (int i = 0; i < Img.h.x * Img.h.y; i++) {
            Image1[i] = Img.P5[i];
        }
        fwrite(Image1, sizeof(char), Img.h.x * Img.h.y, outImage);
        delete[] Image1;
    }
    else{
        char* Image1 = new char[Img.h.x * Img.h.y * 3];
        for (int i = 0; i < Img.h.x * Img.h.y; i++) {
            Image1[i * 3 + 0] = Img.P6[i].red;
            Image1[i * 3 + 1] = Img.P6[i].green;
            Image1[i * 3 + 2] = Img.P6[i].blue;
        }
        fwrite(Image1, sizeof(char), Img.h.x * Img.h.y * 3, outImage);
        delete[] Image1;
    }
    fclose(outImage);

}
int inverseColor(Image& Img, vector<char> Image = vector<char>(0)){
    if(!strcmp(Img.h.type, "P5")) {
        for (int i = 0; i < Img.h.x * Img.h.y; i++) {
            Img.P5[i] = Img.h.a - Img.P5[i];
        }
        return 0;
    }
    else if(!strcmp(Img.h.type, "P6")){
        for (int i = 0; i < Img.h.x * Img.h.y ; i++) {
            Img.P6[i].red = Img.h.a - Image[i * 3 + 0];
            Img.P6[i].green = Img.h.a - Image[i * 3 + 1];
            Img.P6[i].blue = Img.h.a - Image[i * 3 + 2];
        }
        return 0;
    }
    return 1;
}
int inverseHorizontal(Image& Img, vector<char> Image = vector<char>(0)){
    if(!strcmp(Img.h.type, "P5")) {
        char* Image2 = new char[Img.h.x * Img.h.y];
        for(int i = 0; i < Img.h.x; i++){
            for(int j = 0; j < Img.h.y; j++){

                Image2[j * Img.h.x + i] = Img.P5[(j+1) * Img.h.x - i-1];// ����� �� -1

            }
        }
        for(int i = 0; i < Img.h.x * Img.h.y; i++){
            Img.P5[i] = Image2[i];
        }
        return 0;
    }
    else if(!strcmp(Img.h.type, "P6")){
        for (int i = 0; i < Img.h.x ; i++) {
            for(int j = 0; j < Img.h.y; j++){
                Img.P6[j * Img.h.x + i].red = Image[(j + 1) * Img.h.x * 3 - (i * 3 + 2) - 1];
                Img.P6[j * Img.h.x + i].green = Image[(j + 1) * Img.h.x * 3 - (i * 3 + 1) - 1];
                Img.P6[j * Img.h.x + i].blue = Image[(j + 1) * Img.h.x * 3 - (i * 3 + 0) - 1];
            }
        }
        return 0;
    }
    return 1;
}
int inverseVertical(Image& Img, vector<char> Image = vector<char>(0)){
    if(!strcmp(Img.h.type, "P5")) {
        char* Image2 = new char[Img.h.x * Img.h.y];
        for(int i = 0; i < Img.h.x; i++){
            for(int j = 0; j < Img.h.y; j++){

                Image2[j * Img.h.x + i] = Img.P5[(Img.h.y - j) * Img.h.x - i];

            }
        }
        for(int i = 0; i < Img.h.x * Img.h.y; i++){
            Img.P5[i] = Image2[i];
        }
        return 0;
    }
    else if(!strcmp(Img.h.type, "P6")){
        for (int i = 0; i < Img.h.x ; i++) {
            for(int j = 0; j < Img.h.y; j++){
                Img.P6[j * Img.h.x + i].red = Image[(Img.h.y- j) * Img.h.x * 3 - (i * 3 + 2) - 1];
                Img.P6[j * Img.h.x + i].green = Image[(Img.h.y- j)  * Img.h.x * 3 - (i * 3 + 1) - 1];
                Img.P6[j * Img.h.x + i].blue = Image[(Img.h.y- j)  * Img.h.x * 3 - (i * 3 + 0) - 1];
            }
        }
        return 0;
    }
    return 1;
}
int rotateLeft90(Image& Img, vector<char> Image = vector<char>(0)){
    if(!strcmp(Img.h.type, "P5")) {
        char* Image2 = new char[Img.h.x * Img.h.y];
        for(int i = 0; i < Img.h.x; i++){
            for(int j = 0; j < Img.h.y; j++){
                Image2[Img.h.y * (Img.h.x - i - 1) + j] = Img.P5[j * Img.h.x + i];

            }
        }
        for(int i = 0; i < Img.h.x * Img.h.y; i++){
            Img.P5[i] = Image2[i];
        }
        int tmp_h_y = Img.h.y;
        Img.h.y = Img.h.x;
        Img.h.x = tmp_h_y;
        return 0;
    }
    else if(!strcmp(Img.h.type, "P6")){
        vector <RGB> tmp(Img.h.x * Img.h.y);
        for (int i = 0; i < Img.h.x * Img.h.y ; i++) {
            Img.P6[i].red = Image[i * 3 + 0];
            Img.P6[i].green = Image[i * 3 + 1];
            Img.P6[i].blue = Image[i * 3 + 2];
        }
        for (int i = 0; i < Img.h.x; i++) {
            for(int j = 0; j < Img.h.y; j++) {
                tmp[Img.h.y * (Img.h.x - i - 1) + j] = Img.P6[j * Img.h.x + i];
            }
        }
        Img.P6 = tmp;
        int tmp_h_y = Img.h.y;
        Img.h.y = Img.h.x;
        Img.h.x = tmp_h_y;
        return 0;
    }
    return 1;
}
int rotateRight90(Image& Img, vector<char> Image = vector<char>(0)){
    if(!strcmp(Img.h.type, "P5")) {
        char* Image2 = new char[Img.h.x * Img.h.y];
        for(int i = 0; i < Img.h.x; i++){
            for(int j = 0; j < Img.h.y; j++){
                Image2[i * Img.h.y + (Img.h.y - j - 1)] = Img.P5[j * Img.h.x + i];
            }
        }
        for(int i = 0; i < Img.h.x * Img.h.y; i++){
            Img.P5[i] = Image2[i];
        }
        int tmp_h_y = Img.h.y;
        Img.h.y = Img.h.x;
        Img.h.x = tmp_h_y;
        return 0;
    }
    else if(!strcmp(Img.h.type, "P6")){
        vector <RGB> tmp(Img.h.x * Img.h.y);
        for (int i = 0; i < Img.h.x * Img.h.y ; i++) {
            Img.P6[i].red = Image[i * 3 + 0];
            Img.P6[i].green = Image[i * 3 + 1];
            Img.P6[i].blue = Image[i * 3 + 2];
        }
        for (int i = 0; i < Img.h.x; i++) {
            for(int j = 0; j < Img.h.y; j++){
                tmp[i * Img.h.y + (Img.h.y - j - 1)] = Img.P6[j * Img.h.x + i];
            }
        }
        Img.P6 = tmp;
        int tmp_h_y = Img.h.y;
        Img.h.y = Img.h.x;
        Img.h.x = tmp_h_y;
    }
    return 1;

}
int main(int argc, char *argv[]) {

    FILE* inpImage;
    inpImage = fopen(argv[1],"rb");
    if(!inpImage){
        printf("Error: Can't open file!\n");
        cout << argv[2];
        return 3;
    }
    Image Img;
    fscanf(inpImage, "%s\n%d %d\n%d\n", &Img.h.type, &Img.h.x, &Img.h.y, &Img.h.a);
    if(!strcmp(Img.h.type, "P5")) {
        char* P5 = new char[Img.h.x * Img.h.y];
        int t = fread(P5, sizeof(char), Img.h.x * Img.h.y, inpImage);
        for(int i = 0 ; i < Img.h.x * Img.h.y; i++){
            Img.P5.push_back(P5[i]);
        }
        if(t == 0)
        {
            printf("Error: Incorrect pnm file!");
            return 4;
        }

        switch(*argv[2]){
            case InversColor:{
                inverseColor(Img);
                saveImage(Img, inpImage, argv[1]);
                break;
            }
            case InversHorizontal:{
                inverseHorizontal(Img);
                saveImage(Img, inpImage, argv[1]);

                break;
            }
            case InversVertical:{
                inverseVertical(Img);
                saveImage(Img, inpImage,argv[1]);
                break;
            }
            case RotateLeft90:{
                rotateLeft90(Img);
                saveImage(Img, inpImage, argv[1]);
                break;
            }
            case RotateRight90: {
                rotateRight90(Img);
                saveImage(Img, inpImage, argv[1]);
                break;
            }
        }
        delete[] P5;
    }
    else if(!strcmp(Img.h.type,"P6")){
        Img.init(Img.h.x * Img.h.y);
        char* Image = new char[Img.h.x * Img.h.y * 3];
        vector<char> Image2;
        int t = fread(Image, sizeof(char), Img.h.x * Img.h.y * 3, inpImage);
        for(int i = 0; i < Img.h.x * Img.h.y * 3; i++){
            Image2.push_back(Image[i]);
        }
        if(t == 0)
        {
            printf("Error: Incorrect pnm file!");
            return 4;
        }
        switch(*argv[2]) {
            case InversColor: {
                inverseColor(Img, Image2);
                saveImage(Img, inpImage, argv[1]);
                break;
            }
            case InversHorizontal: {
                inverseHorizontal(Img, Image2);
                saveImage(Img, inpImage, argv[1]);
                break;
            }
            case InversVertical: {
                inverseVertical(Img, Image2);
                saveImage(Img, inpImage, argv[1]);
                break;
            }
            case RotateLeft90: {

                rotateLeft90(Img, Image2);
                saveImage(Img, inpImage, argv[1]);
                break;
            }
            case RotateRight90: {
                rotateRight90(Img, Image2);
                saveImage(Img, inpImage, argv[1]);
                break;
            }

        }
        delete[] Image;
    }
    else {
        printf("It's not PNM file!");
        return 2;
    }
    return 0;
}
