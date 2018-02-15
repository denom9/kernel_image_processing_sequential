#include <iostream>
#include <chrono>
#include <cmath>
#include <cstring>
#include "PPM.h"
#include "kernels.h"


int main(int argc, char* argv[]) {

    Image_t* inputImg = PPM_import(argv[1]);
    char* outputPath = argv[2];
    int kernelSize = (*argv[3] != '3' && *argv[3] != '5' && *argv[3] != '7')? 3 : (int)*argv[3] - '0';

    const int imageWidth = Image_getWidth(inputImg);
    const int imageHeight = Image_getHeight(inputImg);
    const int imageChannels = Image_getChannels(inputImg);

    int ik = 0;
    int jk = 0;
    int kernelIndex;
    int border = kernelSize / 2;

    float currentPixel = 0;
    float* kernel = (*argv[3] == '5')? kernel5 : (*argv[3] == '7')? kernel7 : kernel3;
    float sum;

    Image_t* output = Image_new(imageWidth,imageHeight,imageChannels);
    float* data = Image_getData(output);

    auto start = std::chrono::system_clock::now();

    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            for(int c = 0; c < imageChannels; c++) {

                for (int ii = 0; ii < kernelSize; ii++) {
                    ik = ((i - border + ii) < 0) ? 0 : ((i - border + ii) > imageHeight - 1) ? imageHeight - 1 : i - border + ii;
                    for (int jj = 0; jj < kernelSize; jj++) {
                        jk = ((j - border + jj) < 0) ? 0 : ((j - border + jj) > imageWidth - 1) ? imageWidth - 1 : j - border + jj;
                        currentPixel = Image_getPixel(inputImg, jk, ik, c);
                        kernelIndex = (kernelSize-1 - ii) * kernelSize + (kernelSize-1 - jj);
                        sum += (currentPixel * kernel[kernelIndex]);
                    }
                }
                data[(i*imageWidth+j)*imageChannels+c] = sum;
                sum = 0;
            }
        }
    }

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed = end-start;
    std::cout << elapsed.count();

    PPM_export(strcat(outputPath,"/output_sequential.ppm"),output);

    return 0;
}