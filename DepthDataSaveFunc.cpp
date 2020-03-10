#include "DepthDataSaveFunc.h"
#include <QFile>
#include <QTextStream>
#include <QThread>
#include <iostream>  // for debug writing
#include <string>    // useful for reading and writing

#include <fstream>   // ifstream, ofstream
#include <sstream>   // istringstream
#include <QDebug>

#include <iostream>
#include <fstream>

//static const std::vector<int> m_param_png{ CV_IMWRITE_PNG_COMPRESSION, 9, CV_IMWRITE_PNG_STRATEGY, CV_IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY };
#define DEPTH_FILE_PATH   ".\\depth\\%09d.dat"
#define SENDDATA_HEADER_DEPTH	"DATA_PNG"
#define WIDTH_DEPTH		512
#define HEIGHT_DEPTH	424
#define SIZE_DEPTH		(512*424)

DepthDataSaveFunc::DepthDataSaveFunc(QObject *parent) : QObject(parent)
{
    roopFlag = false;
}


void DepthDataSaveFunc::setSaveDir(QString dir){

    saveDir = dir;
}


int DepthDataSaveFunc::getQueueSize(){

    return depthDataQueue.size();

}

void DepthDataSaveFunc::addDepthData(std::vector<UINT16> depthData,int index,QString fileName){


    depthDataQueue.enqueue(depthData);

    if(fileName.isEmpty()){
        depthFileNameQueue.enqueue(QString("%1").arg(index).rightJustified(10,'0'));
        //imageIndexQueue.enqueue(index);
    }
    else{
        depthFileNameQueue.enqueue(fileName);
    }
}

#ifdef PNGVer
bool savePNG(QString fileName,int imageWidth,int imageHeight,std::vector<UINT16> data){
    FILE* fpout = fopen(fileName.toLocal8Bit().data(), "wb");
    if (fpout == NULL)
        return false;

    // png_structp構造体を確保・初期化
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, png_voidp_NULL,png_error_ptr_NULL, png_error_ptr_NULL);
    if (write_ptr == NULL){
        fclose(fpout);
        return false;
    }

    // png_infop構造体を確保・初期化
    png_infop write_info_ptr = png_create_info_struct(write_ptr);
    // libpngにfpoutを渡す
    png_init_io(write_ptr, fpout);
    png_set_write_status_fn(write_ptr, png_write_status_ptr_NULL);
/*
    unsigned short buffer[SIZE_DEPTH];
    memcpy(buffer, &data[0], SIZE_DEPTH);
*/
    png_bytep* row_pointers = (png_bytep*)png_malloc(write_ptr, imageHeight * sizeof(png_bytep));

    // IHDRチャンク情報を設定
    png_set_IHDR(write_ptr, write_info_ptr, imageWidth, imageHeight, 16, PNG_COLOR_TYPE_GRAY,
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);




    ushort* pW = reinterpret_cast<ushort*>(&data[0]);
    int imax = 0;
    for (int j = 0; j < imageHeight; j++, pW += imageWidth){
        for (int i = 0; i < imageWidth; i++){
            if (pW[i] > imax) imax = pW[i];
        }
    }

    int ishift = 0;
    while (imax){
        imax = imax >> 1;
        ishift++;
    }




    png_color_8 sigbit;
    sigbit.gray = ishift;
    png_set_sBIT(write_ptr, write_info_ptr, &sigbit);
    png_uint_16* pwbits = (png_uint_16*)&data[0];
    ishift = 16 - ishift;

    for (int i = 0; i < imageHeight; i++, pwbits += imageWidth){
        row_pointers[i] = (png_bytep)png_malloc(write_ptr, imageWidth << 1);
        for (int ii = 0; ii < imageWidth; ii++){
            png_uint_16 tmp = pwbits[ii] << ishift;
            row_pointers[i][ii << 1] = (png_byte)(tmp >> 8);
            row_pointers[i][(ii << 1) + 1] = (png_byte)tmp;
        }
    }
    png_set_rows(write_ptr, write_info_ptr, row_pointers);
    png_write_png(write_ptr, write_info_ptr, 0, NULL);


    for (int i = 0; i < imageHeight; i++)
        png_free(write_ptr, row_pointers[i]);
    png_free(write_ptr, row_pointers);

    png_destroy_write_struct(&write_ptr, &write_info_ptr);
    fclose(fpout);
    return true;
}

#endif



void DepthDataSaveFunc::depthSave(){


    bool processFlag=false;
    int pBarCount=1;


    while(roopFlag){

        while (!depthFileNameQueue.isEmpty()){
           if(depthDataQueue.size() > 0){
               std::vector<UINT16> depthBuffer = depthDataQueue.dequeue();


//#define CSVVer
#ifdef CSVVer
               QString fileName = saveDir+"/depth/"+depthFileNameQueue.dequeue();
               QFile file(fileName+".csv");
               file.open(QIODevice::WriteOnly);
               QTextStream stream(&file);

               for( int y = 0; y < 424; y++ ){
                   for( int x = 0; x < 512; x++ ){
                       UINT16 depth = depthBuffer[y * 512 + x];
                       if(x>0){
                           stream <<  ",";
                       }
                       stream <<  QString("%1").arg(depth);
                   }
                   stream <<  "\r\n";
               }
               stream.flush();
               //stream << depthDataQueue.dequeue();
               file.close();
#elseif LIBPNG
               savePNG(fileName+".png",512,424,depthBuffer);
#else


               cv::Mat matDepth(HEIGHT_DEPTH, WIDTH_DEPTH, CV_16UC1);
               memcpy(matDepth.data, reinterpret_cast<uchar*>(&depthBuffer[0]), SIZE_DEPTH * sizeof(unsigned short));
               QString fileName = saveDir+"/depth/"+depthFileNameQueue.dequeue()+".tiff";
               cv::imwrite(fileName.toLocal8Bit().data(),matDepth);


#endif

               if(processFlag){
                   emit saveProcess(pBarCount);
                   pBarCount++;
               }

               if(!roopFlag && !processFlag){
                   processFlag = true;
               }

           }
        }
    }

    emit finishedSaving();

    this->thread()->quit();


}
