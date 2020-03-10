#ifndef POSDATATHREAD_H
#define POSDATATHREAD_H

#include <QObject>
#include <QStack>
#include <QQueue>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QProgressBar>
#include <Windows.h>
#include <QThread>

class PosDataThread : public QObject
{
    Q_OBJECT
public:
    explicit PosDataThread(QObject *parent = 0);
    void setSaveDir(QString dir);
    void addPosData(int id,int fn,QVector<QVector3D> posData,QString fileName="");
    void enablePosDataHandle(bool flag);
    int getQueueSize();

    bool roopFlag;
    int posDataIndex;

private:
    QQueue <QVector<QVector3D>> posDataQueue;
    QQueue <int> posDataIDQueue;
    QQueue <int> frameNumberQueue;
    QQueue <QString> posDataFileNameQueue;
    QString saveDir;
    bool posDataFlag;

    QFile file;
    QTextStream stream;

signals:
    void finishedSaving();
    void saveProcess(int current);
    void updateMaximum(int max);

public slots:
    void posDataSave();
};

#endif // POSDATATHREAD_H
