#include "ImageSave.h"
#include <QDebug>
#include <QApplication>
#include <QThread>
#include <QLabel>

ImageSave::ImageSave(QObject *parent) : QObject(parent)
{
    imgIndex = 1;
    depthIndex = 1;
    roopFlag=false;
    posDataFlag = false;
    rockState = false;

    imgWidth = 960;
    imgHeight = 540;
}

void ImageSave::setSaveImageSize(int width,int height){

    imgWidth = width;
    imgHeight = height;


}


void ImageSave::setStartImgIndex(int index){

    imgIndex = index;
    depthIndex = index;
}

int ImageSave::getImageQueueSize(){

    return fileNameQueue.size();

}


void ImageSave::setImageQueue(QQueue <QImage> _imageQueue){


    imageQueue = _imageQueue;

}

void ImageSave::setFileNameQueue(QQueue <QString> _fileNameQueue){

    fileNameQueue = _fileNameQueue;

}



void ImageSave::setSaveDir(QString dir){

    saveDir = dir;

}

void ImageSave::enablePosDataHandle(bool flag){

    posDataFlag = flag;

}

QQueue <QImage> ImageSave::getImageQueue(int start,int end){

    QQueue <QImage> extractedImageQueue;
    if(end==-1){
        end = imageQueue.size();
    }

    for(int i=start;i<end;i++){
        extractedImageQueue.enqueue(imageQueue[start]);
        imageQueue.takeAt(start);
    }

    return extractedImageQueue;

}

QQueue <QString> ImageSave::getFileNameQueue(int start,int end){

    QQueue <QString> extractedFileNameQueue;
    if(end==-1){
        end = fileNameQueue.size();
    }

    qDebug() << "Start:" << start;

    for(int i=start;i<end;i++){
        qDebug() << fileNameQueue[start];
        extractedFileNameQueue.enqueue(fileNameQueue[start]);
        fileNameQueue.takeAt(start);
    }

    return extractedFileNameQueue;


}

void ImageSave::addImage(QImage image,int index,QString fileName){

Start:
    //qDebug() << "addImage:" << index;
    if(!rockState){
        rockState = true;


        imageQueue.enqueue(image.scaled(imgWidth,imgHeight));
        //testImageQueue.push(image);
        //qDebug() << "isNull:" << image.isNull() << ":"<< testImageQueue.back().isNull();
        //qDebug() << "After:" << &testImageQueue.back();
        if(fileName.isEmpty()){
            fileNameQueue.enqueue(QString("%1").arg(index).rightJustified(10,'0'));
            //imageIndexQueue.enqueue(index);
        }
        else{
            fileNameQueue.enqueue(fileName);
        }
        rockState = false;
        //qDebug() << testImageQueue.size() << ":" << index<<  "@" << this->thread();

    }
    else{
        goto Start;
    }
}

void ImageSave::addDepthData(std::vector<UINT16> depthData,int index,QString fileName){


    depthDataQueue.enqueue(depthData);
    if(fileName.isEmpty()){
        depthFileNameQueue.enqueue(QString("%1").arg(index));
        //imageIndexQueue.enqueue(index);
    }
    else{
        depthFileNameQueue.enqueue(fileName);
    }




}

void ImageSave::addPosData(int id,int fn,QVector<QVector3D> posData,QString fileName){

    posDataIDQueue.enqueue(id);
    frameNumberQueue.enqueue(fn);
    posDataQueue.enqueue(posData);
    posDataFileNameQueue.enqueue(fileName);

}


void ImageSave::imageSave(){

    bool processFlag=false;
    int pBarCount=1;
    //QProgressBar *pBar;
/*
    QFile file("file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
*/
    while(roopFlag){
        while (!fileNameQueue.isEmpty()){
            if(fileNameQueue.size() == imageQueue.size()){

                if(!rockState){
                    rockState = true;
                    QString fileName = fileNameQueue.dequeue()+".jpg";//QString("%1.jpg").arg(fileNameQueue.dequeue());
                    /*if(!fileName.contains("_")){
                        fileName = fileName.rightJustified(14,'0');
                    }
                    */
                    QImage image = imageQueue.dequeue();
                    //testImageQueue.pop();
                    rockState = false;
                    //image = image.mirrored(true,false);

                    bool flag = image.save(saveDir+"/jpg/"+fileName,"JPG");

                    //qDebug () << "image save:" << flag;
                    //imgIndex++;

                    if(!flag){
                        emit imageSaveErrorOccured(image,fileName);
                        qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<image.width();
                    }

                    if(processFlag){
                        //QApplication::processEvents();
                        emit saveProcess(pBarCount);
                        //pBar->setValue(pBarCount);
                        pBarCount++;
                    }

                    if(!roopFlag && !processFlag){
                        processFlag = true;
                    }
                }
            }
        }

#ifdef Yamamoto
         while (!depthFileNameQueue.isEmpty()){
            if(depthDataQueue.size() > 0){
                QFile file(saveDir+"/depth/"+depthFileNameQueue.dequeue()+".csv");
                file.open(QIODevice::WriteOnly);
                QTextStream stream(&file);

                std::vector<UINT16> depthBuffer = depthDataQueue.dequeue();
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

                //stream << depthDataQueue.dequeue();
                file.close();

            }
         }
         while (!posDataFileNameQueue.isEmpty()){
            if(posDataFileNameQueue.size()>0){
                QFile file(saveDir+"/pos/"+posDataFileNameQueue.dequeue()+".csv");
                file.open(QIODevice::WriteOnly);
                QTextStream stream(&file);


                int index=1;

                QVector <QVector3D> posVec = posDataQueue.dequeue();

                stream << posDataIDQueue.dequeue() << "," << frameNumberQueue.dequeue();
                for(int i=0;i<posVec.size();i++){
                    stream << "," << posVec[i].x() << "," <<  posVec[i].y() << "," << posVec[i].z();
                }

                file.close();
            }
        }

#endif
    }
    //pBar->close();

#ifdef Yamamoto
    if(posDataFlag){

        QFile file(saveDir+"/"+"pos.csv");
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);

        emit updateMaximum(posDataIDQueue.size());
        int index=1;

        while (!posDataIDQueue.isEmpty()){
            QVector <QVector3D> posVec = posDataQueue.dequeue();

            stream << posDataIDQueue.dequeue() << "," << frameNumberQueue.dequeue();
            for(int i=0;i<posVec.size();i++){
                stream << "," << posVec[i].x() << "," <<  posVec[i].y() << "," << posVec[i].z();
            }
            stream << "\r\n";
            emit saveProcess(index);
            index++;
        }
        file.close();

    }
#endif

    emit finishedSaving();

    this->thread()->quit();


}
