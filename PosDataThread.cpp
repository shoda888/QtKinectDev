#include "PosDataThread.h"
#include <QDebug>

PosDataThread::PosDataThread(QObject *parent) : QObject(parent)
{
    posDataFlag = true;
    roopFlag = false;
}



void PosDataThread::setSaveDir(QString dir){

    saveDir = dir;

    posDataIndex = 1;
    file.setFileName(saveDir+"/pos/"+"pos.csv");
    file.open(QIODevice::WriteOnly);
    stream.setDevice(&file);

}

int PosDataThread::getQueueSize(){

    return posDataIDQueue.size();

}

void PosDataThread::enablePosDataHandle(bool flag){

    posDataFlag = flag;

}


void PosDataThread::addPosData(int id,int fn,QVector<QVector3D> posData,QString fileName){


    //qDebug() << "addPosData:" << id << fn;
    posDataIDQueue.enqueue(id);
    frameNumberQueue.enqueue(fn);
    posDataQueue.enqueue(posData);
    posDataFileNameQueue.enqueue(fileName);

}



void PosDataThread::posDataSave(){

    bool processFlag=false;
    int pBarCount=1;



    while(roopFlag){
        while(!posDataQueue.isEmpty()){

            //emit updateMaximum(posDataIDQueue.size());
            int index=1;

            while (!posDataIDQueue.isEmpty()){
                QVector <QVector3D> posVec = posDataQueue.dequeue();
                //qDebug() << "Save Pos Data" << file.isOpen() << posDataIDQueue.first() << frameNumberQueue.first();
                stream << posDataIDQueue.dequeue() << "," << frameNumberQueue.dequeue();
                for(int i=0;i<posVec.size();i++){
                    stream << "," << posVec[i].x() << "," <<  posVec[i].y() << "," << posVec[i].z();
                }
                stream << "\r\n";

                if(processFlag){
                    emit saveProcess(pBarCount);
                    pBarCount++;
                }

                if(!roopFlag && !processFlag){
                    processFlag = true;
                }
                index++;
            }


        }
    }


    stream.flush();
    qDebug() << "Pos.csv close.";
    file.close();
    emit finishedSaving();

    this->thread()->quit();


}
