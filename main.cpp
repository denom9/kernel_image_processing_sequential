#include <iostream>
#include <chrono>
#include "PPM.h"
#include "kernels.h"
int main(int argc, char* argv[]) {

    Image_t* inputImg = PPM_import(argv[1]);
    const int imageWidth = Image_getWidth(inputImg);
    const int imageHeight = Image_getHeight(inputImg);
    const int imageChannels = Image_getChannels(inputImg);


    //creo le immagini di output
    Image_t* edge1Output =          Image_new(imageWidth,imageHeight,imageChannels);
    Image_t* edge2Output =          Image_new(imageWidth,imageHeight,imageChannels);
    Image_t* edge3Output =          Image_new(imageWidth,imageHeight,imageChannels);
    Image_t* sharpenOutput =        Image_new(imageWidth,imageHeight,imageChannels);
    Image_t* boxblurOutput =        Image_new(imageWidth,imageHeight,imageChannels);
    Image_t* gaussianblur3Output =  Image_new(imageWidth,imageHeight,imageChannels);
    Image_t* gaussianblur5Output =  Image_new(imageWidth,imageHeight,imageChannels);
    Image_t* unsharpOutput =        Image_new(imageWidth,imageHeight,imageChannels);

    //std::cout << imageWidth << "x" << imageHeight << "-" << imageChannels << std::endl;


    int kernelSize = 3;




    int ik = 0;
    int jk = 0;
    int kernelIndex;
    float currentPixel = 0,b = 0;
    float sum[6];


    auto start = std::chrono::system_clock::now();


    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            for(int c = 0; c < imageChannels; c++) {

                for (int ii = 0; ii < kernelSize; ii++) {
                    //ik = (((i - kernelSize/2 + ii) < 0) || ((i - kernelSize/2 + ii) > imageHeight)) ? imageHeight - 1 : i - kernelSize/2 + ii;
                    ik = ((i - kernelSize/2 + ii) < 0) ? 0 : ((i - kernelSize/2 + ii) > imageHeight - 1) ? imageHeight - 1 : i - kernelSize/2 + ii;
                    for (int jj = 0; jj < kernelSize; jj++) {
                        //jk = (((j - kernelSize/2 + jj) < 0) || ((j - kernelSize/2 + jj) > imageWidth)) ? imageWidth - 1 : j - kernelSize/2 + jj;
                        jk = ((j - kernelSize/2 + jj) < 0) ? 0 : ((j - kernelSize/2 + jj) > imageWidth - 1) ? imageWidth - 1 : j - kernelSize/2 + jj;
                        currentPixel = Image_getPixel(inputImg, jk, ik, c);
                        kernelIndex = (kernelSize-1 - ii) * kernelSize + (kernelSize-1 - jj);
                        sum[0] += (currentPixel * edge1[kernelIndex]);
                        sum[1] += (currentPixel * edge2[kernelIndex]);
                        sum[2] += (currentPixel * edge3[kernelIndex]);
                        sum[3] += (currentPixel * sharpen[kernelIndex]);
                        sum[4] += (currentPixel * boxblur[kernelIndex]);
                        sum[5] += (currentPixel * gaussianblur3[kernelIndex]);
                    }
                }
                Image_setPixel(edge1Output, j, i, c, sum[0]);
                Image_setPixel(edge2Output, j, i, c, sum[1]);
                Image_setPixel(edge3Output, j, i, c, sum[2]);
                Image_setPixel(sharpenOutput, j, i, c, sum[3]);
                Image_setPixel(boxblurOutput, j, i, c, sum[4]);
                Image_setPixel(gaussianblur3Output, j, i, c, sum[5]);
                for(int h = 0; h < 6; h++)
                    sum[h] = 0;

            }
        }
    }

    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed = end-start;
    std::cout << elapsed.count();

    PPM_export("../processed/edge1.ppm",edge1Output);
    PPM_export("../processed/edge2.ppm",edge2Output);
    PPM_export("../processed/edge3.ppm",edge3Output);
    PPM_export("../processed/sharpen.ppm",sharpenOutput);
    PPM_export("../processed/boxblur.ppm",boxblurOutput);
    PPM_export("../processed/gaussianblur3.ppm",gaussianblur3Output);

    return 0;
}

