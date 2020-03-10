#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define NOMINMAX
#include <QMainWindow>
#include <QThread>
#include <Windows.h>
#include <Kinect.h>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QToolButton>
#include <QCheckBox>
#include <QPushButton>
#include <QProcess>
#include <QTime>
#include <QSpinBox>
#include <QComboBox>

#include "ImageRenderWidget.h"
#include "ImageSave.h"
#include "PosDataThread.h"
#include "DepthDataSaveFunc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    QWidget *createMenuWidget();
    QLineEdit *saveDirLineEdit;
    QPushButton *startRecordingPushButton;
    QCheckBox *posRenderCheckBox;
    QSpinBox *intervalSpinBox;
    QComboBox *rgbImageSizeComboBox;

    ImageRenderWidget *renderWidget;

    IKinectSensor* pSensor;
    IColorFrameSource* pColorSource;
    IDepthFrameSource* pDepthSource;
    IColorFrameReader* pColorReader;
    IDepthFrameReader* pDepthReader;
    IFrameDescription* pColorDescription;
    IFrameDescription* pDepthDescription;
    ICoordinateMapper* pCoordinateMapper;

    IBodyFrameSource *body_frame_source;
    IBodyFrameReader *body_frame_reader;
    IFrameDescription *body_frame_description;

    int colorWidth;
    int colorHeight;

    int depthWidth;
    int depthHeight;

    int recordIndex;

    std::vector<UINT16> depthBuffer;
    std::vector<RGBQUAD> colorBuffer;
    uchar *depthData;
    QImage rgbImage;
    QImage depthImage;

    int imgIndex;
    int depthIndex;
    bool saveMode;
    bool intervalFlag;
    QTime saveIntervalTime;
    QString saveToDirPath;

    QTime time;

    QList <ImageSave *> imageSaveFuncList;
    QList <QThread *> imageSaveThreadList;
    QList <PosDataThread *>posDataSaveFuncList;
    QList <QThread *> posDataSaveThreadList;
    QList <DepthDataSaveFunc *> depthSaveFuncList;
    QList <QThread *> depthSaveThreadList;

    int threadIndex;
    int threadSize;
    int imgThreadSize;
    QList <QProgressBar *>pBarList;

    QImage capturedImage;
    int timerID;

private slots:
    void openDirToSave();
    void startRecording();
    void stopRecording();
    void convertToMovie();
    void finishConvertingToMovie(int exitCode);

    void startCapture();
    void stopCapture();

    void checkImage(QImage image,QString fileName);
};

#endif // MAINWINDOW_H
