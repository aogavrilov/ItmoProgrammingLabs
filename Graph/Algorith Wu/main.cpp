#include <iostream>
#include <fstream>
#include <wrl.h>
#include <vector>
#include <cmath>
using namespace std;
enum commands{
    InversColor = '0',
    InversHorizontal = '1',
    InversVertical = '2',
    RotateLeft90 = '3',
    RotateRight90 = '4',
    WriteLine = '5'
};

class Image{
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
    FILE* inpImage;
    FILE* outImage;
    vector<char> Image2;

public:
    Image(char* path){
        inpImage = fopen(path,"rb");
        if(!inpImage){
            printf("Error: Can't open file!\n");

        }

        fscanf(inpImage, "%s\n%d %d\n%u\n", &h.type, &h.x, &h.y, &h.a);
       // cout << h.a;
        if(!strcmp(h.type, "P5")) {
            char *P5 = new char[h.x * h.y];
            int t = fread(P5, sizeof(char), h.x * h.y, inpImage);
            if (t == 0) {
                printf("Error: Incorrect pnm file!");
                throw (exception());
            }
            for (int i = 0; i < h.x * h.y; i++) {
                this->P5.push_back(P5[i]);
            }
            delete[] P5;

        }
        else if(!strcmp(h.type,"P6")) {
            init(h.x * h.y);
            char *Image = new char[h.x * h.y * 3];

            int t = fread(Image, sizeof(char), h.x * h.y * 3, inpImage);
            if (t == 0) {
                printf("Error: Incorrect pnm file!");
                throw (exception());
            }
            for (int i = 0; i < h.x * h.y * 3; i++) {
                Image2.push_back(Image[i]);
            }
            delete[] Image;

        }
        else {
            printf("It's not PNM file!");
            throw (exception());
        }
    }
    ~Image(){
        //smb
        fclose(outImage);
        fclose(inpImage);
    }
    int saveImage(char* address){
        //fclose(inpImage);

        outImage = fopen(address,"wb");
        if(!outImage){
            printf("Error: Invalid error!");
            return 5;
        }

        fprintf(outImage, "%s\n%d %d\n%d\n", h.type, h.x, h.y, h.a);

        if(!strcmp(h.type, "P5")) {
            char* Image1 = new char[h.x * h.y];
            for (int i = 0; i < h.x * h.y; i++) {
                Image1[i] = P5[i];
            }
            fwrite(Image1, sizeof(char), h.x * h.y, outImage);
            delete[] Image1;
        }
        else{
            char* Image1 = new char[h.x * h.y * 3];
            for (int i = 0; i < h.x * h.y; i++) {
                Image1[i * 3 + 0] = P6[i].red;
                Image1[i * 3 + 1] = P6[i].green;
                Image1[i * 3 + 2] = P6[i].blue;
            }
            fwrite(Image1, sizeof(char), h.x * h.y * 3, outImage);
            delete[] Image1;
        }
        fclose(outImage);

    }
    int inverseColor(){
        if(!strcmp(h.type, "P5")) {
            for (int i = 0; i < h.x * h.y; i++) {
                P5[i] = h.a - P5[i];
            }
            return 0;
        }
        else if(!strcmp(h.type, "P6")){
            for (int i = 0; i < h.x * h.y ; i++) {
                P6[i].red = h.a - Image2[i * 3 + 0];
                P6[i].green = h.a - Image2[i * 3 + 1];
                P6[i].blue = h.a - Image2[i * 3 + 2];
            }
            return 0;
        }
        return 1;
    }
    int inverseHorizontal(){
        if(!strcmp(h.type, "P5")) {
            char* Image2 = new char[h.x * h.y];
            for(int i = 0; i < h.x; i++){
                for(int j = 0; j < h.y; j++){
                    Image2[j * h.x + i] = P5[(j+1) * h.x - i-1];
                }
            }
            for(int i = 0; i < h.x * h.y; i++){
                P5[i] = Image2[i];
            }
            return 0;
        }
        else if(!strcmp(h.type, "P6")){
            for (int i = 0; i < h.x ; i++) {
                for(int j = 0; j < h.y; j++){
                    P6[j * h.x + i].red = Image2[(j + 1) * h.x * 3 - (i * 3 + 2) - 1];
                    P6[j * h.x + i].green = Image2[(j + 1) * h.x * 3 - (i * 3 + 1) - 1];
                    P6[j * h.x + i].blue = Image2[(j + 1) * h.x * 3 - (i * 3 + 0) - 1];
                }
            }
            return 0;
        }
        return 1;
    }
    int inverseVertical(){
        if(!strcmp(h.type, "P5")) {
            char* Image2 = new char[h.x * h.y];
            for(int i = 0; i < h.x; i++){
                for(int j = 0; j < h.y; j++){
                    Image2[j * h.x + i] = P5[(h.y - j) * h.x - i];
                }
            }
            for(int i = 0; i < h.x * h.y; i++){
                P5[i] = Image2[i];
            }
            return 0;
        }
        else if(!strcmp(h.type, "P6")){
            for (int i = 0; i < h.x ; i++) {
                for(int j = 0; j < h.y; j++){
                    P6[j * h.x + i].red = Image2[(h.y - j) * h.x * 3 - (i * 3 + 2) - 1];
                    P6[j * h.x + i].green = Image2[(h.y- j)  * h.x * 3 - (i * 3 + 1) - 1];
                    P6[j * h.x + i].blue = Image2[(h.y- j)  * h.x * 3 - (i * 3 + 0) - 1];
                }
            }
            return 0;
        }
        return 1;
    }
    int rotateLeft90(){
        if(!strcmp(h.type, "P5")) {
            char* Image2 = new char[h.x * h.y];
            for(int i = 0; i < h.x; i++){
                for(int j = 0; j < h.y; j++){
                    Image2[h.y * (h.x - i - 1) + j] = P5[j * h.x + i];
                }
            }
            for(int i = 0; i < h.x * h.y; i++){
                P5[i] = Image2[i];
            }
            int tmp_h_y = h.y;
            h.y = h.x;
            h.x = tmp_h_y;
            return 0;
        }
        else if(!strcmp(h.type, "P6")){
            vector <RGB> tmp(h.x * h.y);
            for (int i = 0; i < h.x * h.y ; i++) {
                P6[i].red = Image2[i * 3 + 0];
                P6[i].green = Image2[i * 3 + 1];
                P6[i].blue = Image2[i * 3 + 2];
            }
            for (int i = 0; i < h.x; i++) {
                for(int j = 0; j < h.y; j++) {
                    tmp[h.y * (h.x - i - 1) + j] = P6[j * h.x + i];
                }
            }
            P6 = tmp;
            int tmp_h_y = h.y;
            h.y = h.x;
            h.x = tmp_h_y;
            return 0;
        }
        return 1;
    }
    int rotateRight90(){
        if(!strcmp(h.type, "P5")) {
            char* Image2 = new char[h.x * h.y];
            for(int i = 0; i < h.x; i++){
                for(int j = 0; j < h.y; j++){
                    Image2[i * h.y + (h.y - j - 1)] = P5[j * h.x + i];
                }
            }
            for(int i = 0; i < h.x * h.y; i++){
                P5[i] = Image2[i];
            }
            int tmp_h_y = h.y;
            h.y = h.x;
            h.x = tmp_h_y;
            return 0;
        }
        else if(!strcmp(h.type, "P6")){
            vector <RGB> tmp(h.x * h.y);
            for (int i = 0; i < h.x * h.y ; i++) {
                P6[i].red = Image2[i * 3 + 0];
                P6[i].green = Image2[i * 3 + 1];
                P6[i].blue = Image2[i * 3 + 2];
            }
            for (int i = 0; i < h.x; i++) {
                for(int j = 0; j < h.y; j++){
                    tmp[i * h.y + (h.y - j - 1)] = P6[j * h.x + i];
                }
            }
            P6 = tmp;
            int tmp_h_y = h.y;
            h.y = h.x;
            h.x = tmp_h_y;
        }
        return 1;

    }
    int writeLine(float x0, float y0, float x1, float y1, float gamma = 2, size_t brightness = 255, float thickness = 1){
        x1 = x1 > h.x ? h.x : x1;
        x0 = x0 > h.x ? h.x : x0;
        y0 = y0 > h.y ? h.y : y0;
        y1 = y1 > h.y ? h.y : y1;
        if(x1 < x0){
            swap(x1, x0);
            swap(y1, y0);
        }
        float dx = x1 - x0;
        float dy = y1 - y0;
        float gradient = dy / dx;
        size_t xstart = (size_t)(x0 + 0.5);
        size_t ystart = (size_t)(y0 + gradient * (xstart - x0));
        size_t xend = (size_t)(x1 + 0.5);
        size_t yend = (size_t)(y1 + gradient * (xend - x1));
        float y = ystart - y0 + gradient;
        for(size_t x = xstart; x <= xend; x++){
            /*
            int pixc1 = (int)(brightness * (1 - y + (size_t)y));
            int pixc2 = (int)(brightness * (y - (size_t)y));
             */
            int pixc1 = (int)(brightness);
            int pixc2 = (int)(brightness);
            int p1gc = pow(pow((int)(pixc1 / 255), 0.5), gamma) * 255;
            int p2gc = pow(pow((int)(pixc2 / 255), 0.5), gamma) * 255;
            P5[(size_t)y * h.x + x] = (char)(pixc1);
            P5[((size_t)y + 1) * h.x + x] = (char)(pixc2);
             y += gradient;
        }
    }
};

int main(int argc, char *argv[]) {
    Image img = Image(argv[1]);
    switch(*argv[2]){
        case InversColor:{
            img.inverseColor();
            break;
        }
        case InversHorizontal:{
            img.inverseHorizontal();
            break;
        }
        case InversVertical:{
            img.inverseVertical();
            break;
        }
        case RotateLeft90:{
            img.rotateLeft90();
            break;
        }
        case RotateRight90: {
            img.rotateRight90();
            break;
        }
        case WriteLine:{
            img.writeLine(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]),
                    atoi(argv[9]), atoi(argv[10]));
            img.saveImage(argv[3]);
            break;
        }
    }
    if(*argv[2] != WriteLine){
        img.saveImage(argv[1]);
    }
    return 0;
}
