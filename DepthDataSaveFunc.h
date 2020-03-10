#ifndef DEPTHDATASAVEFUNC_H
#define DEPTHDATASAVEFUNC_H

#include <QObject>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QProgressBar>
#include <Windows.h>
#include <QThread>
#include <QQueue>

#define OPENCV

#ifdef OPENCV
//インクルードファイル指定
#include <C:/OpenCV2.4.9/build/include/opencv2/opencv.hpp>
//静的リンクライブラリの指定
//#include <C:/OpenCV2.4.9/build/include/opencv2/opencv_lib.hpp>
#endif

#ifdef PNGVer
#include "png.h"
#include "pngconf.h"


#define 	png_voidp_NULL   (png_voidp)NULL
#define png_error_ptr_NULL   (png_error_ptr)NULL
#define png_write_status_ptr_NULL   (png_write_status_ptr)NULL
#endif


class DepthDataSaveFunc : public QObject
{
    Q_OBJECT
public:
    explicit DepthDataSaveFunc(QObject *parent = 0);
    void addDepthData(std::vector<UINT16>,int index,QString fileName="");
    void setSaveDir(QString dir);
    int getQueueSize();

    bool roopFlag;

private:

    QQueue < std::vector<UINT16> > depthDataQueue;
    QQueue <int> depthIndexQueue;
    QQueue <QString> depthFileNameQueue;
    int depthIndex;
    QString saveDir;
    std::ofstream *p_ofs_depth;

signals:
    void finishedSaving();
    void saveProcess(int current);

public slots:
    void depthSave();
};

#endif // DEPTHDATASAVEFUNC_H
