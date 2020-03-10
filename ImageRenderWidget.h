#ifndef IMAGERENDERWIDGET_H
#define IMAGERENDERWIDGET_H

#include <QFrame>
#include <QPainter>
#include <QPair>
namespace Ui {
class ImageRenderWidget;
}

class ImageRenderWidget : public QFrame
{
    Q_OBJECT

public:
    explicit ImageRenderWidget(QWidget *parent = 0);
    ~ImageRenderWidget();

    void setPostureData(int id,int index,QPoint point);
    void cleatPostureData(int id);
    void setFPS(int value);
protected:
    void paintEvent(QPaintEvent * event);

private:
    Ui::ImageRenderWidget *ui;
    QImage render_image;
    bool renderFlag;
    bool posRenderFlag;

    QList <QColor> colorList;
    QList <QPoint> postureDataList[8];
    QList < QPair <int,int> > JointSetList;
    int FPS;

public slots:
    void updateImage(QImage image);
    void enableRendering(bool flag);
    void enablePosRendering(bool flag);


};

#endif // RGBIMAGERENDERWIDGET_H
