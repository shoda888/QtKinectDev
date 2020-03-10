#ifndef IMAGESAVE_H
#define IMAGESAVE_H

#include <QObject>
#include <QStack>
#include <QQueue>
#include <QImage>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QProgressBar>
#include <Windows.h>
#include <queue>
using namespace std;

class ImageSave : public QObject
{
    Q_OBJECT
public:
    explicit ImageSave(QObject *parent = 0);
    void addImage(QImage image,int index,QString fileName="");
    void addPosData(int id,int fn,QVector<QVector3D> posData,QString fileName="");
    void addDepthData(std::vector<UINT16>,int index,QString fileName="");

    void setSaveDir(QString dir);
    void setStartImgIndex(int index);
    int getImageQueueSize();
    void enablePosDataHandle(bool flag);
    void setSaveImageSize(int width,int height);


    QQueue <QImage> getImageQueue(int start,int end=-1);
    QQueue <QString> getFileNameQueue(int start,int end=-1);

    void setImageQueue(QQueue <QImage> _imageQueue);
    void setFileNameQueue(QQueue <QString> _fileNameQueue);


    bool roopFlag;
signals:

public slots:
    void imageSave();

private:
    //QStack <QImage> imageStack;
    std::queue <QImage> testImageQueue;
    QQueue <QImage> imageQueue;
    QQueue <int> imageIndexQueue;
    QQueue <QString> fileNameQueue;

    QQueue < std::vector<UINT16> > depthDataQueue;
    QQueue <int> depthIndexQueue;
    QQueue <QString> depthFileNameQueue;

    QQueue <QVector<QVector3D>> posDataQueue;
    QQueue <int> posDataIDQueue;
    QQueue <int> frameNumberQueue;
    QQueue <QString> posDataFileNameQueue;
    QString saveDir;
    int imgIndex;
    int depthIndex;
    bool posDataFlag;

    int imgWidth;
    int imgHeight;

    bool rockState;//True: Locked, False:Unlock

signals:
    void finishedSaving();
    void saveProcess(int current);
    void updateMaximum(int max);
    void imageSaveErrorOccured(QImage image,QString fileName);

};

#endif // IMAGESAVE_H
