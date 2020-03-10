#include "ImageRenderWidget.h"
#include "ui_ImageRenderWidget.h"
#include <QDebug>

ImageRenderWidget::ImageRenderWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ImageRenderWidget)
{
    ui->setupUi(this);
    renderFlag = true;
    posRenderFlag = true;

    colorList.push_back(Qt::red);
    colorList.push_back(QColor(255,128,0));
    colorList.push_back(Qt::green);
    colorList.push_back(Qt::yellow);
    colorList.push_back(Qt::magenta);
    colorList.push_back(QColor(0,255,255));
    colorList.push_back(QColor(255,174,201));
    colorList.push_back(QColor(255,206,157));


    JointSetList.push_back(QPair <int,int>(0,1));
    JointSetList.push_back(QPair <int,int>(1,20));
    JointSetList.push_back(QPair <int,int>(20,2));
    JointSetList.push_back(QPair <int,int>(2,3));

    JointSetList.push_back(QPair <int,int>(20,4));
    JointSetList.push_back(QPair <int,int>(4,5));
    JointSetList.push_back(QPair <int,int>(5,6));
    JointSetList.push_back(QPair <int,int>(6,7));
    JointSetList.push_back(QPair <int,int>(7,21));
    JointSetList.push_back(QPair <int,int>(7,22));

    JointSetList.push_back(QPair <int,int>(20,8));
    JointSetList.push_back(QPair <int,int>(8,9));
    JointSetList.push_back(QPair <int,int>(9,10));
    JointSetList.push_back(QPair <int,int>(10,11));
    JointSetList.push_back(QPair <int,int>(11,23));
    JointSetList.push_back(QPair <int,int>(11,24));

    JointSetList.push_back(QPair <int,int>(0,12));
    JointSetList.push_back(QPair <int,int>(12,13));
    JointSetList.push_back(QPair <int,int>(13,14));
    JointSetList.push_back(QPair <int,int>(14,15));

    JointSetList.push_back(QPair <int,int>(0,16));
    JointSetList.push_back(QPair <int,int>(16,17));
    JointSetList.push_back(QPair <int,int>(17,18));
    JointSetList.push_back(QPair <int,int>(18,19));

    FPS = 30;
}

ImageRenderWidget::~ImageRenderWidget()
{
    delete ui;
}


void ImageRenderWidget::setPostureData(int id,int index,QPoint point){

    if(postureDataList[id].size() <= index){
        postureDataList[id].push_back(point);
    }
    else{
        postureDataList[id][index] = point;
    }

}

void ImageRenderWidget::cleatPostureData(int id){

    postureDataList[id].clear();

}

void ImageRenderWidget::setFPS(int value){

    FPS = value;

}

void ImageRenderWidget::updateImage(QImage image){

    if(renderFlag){

        render_image = image;
        update();
    }

}

void ImageRenderWidget::enableRendering(bool flag){

    renderFlag = flag;

}


void ImageRenderWidget::enablePosRendering(bool flag){

    posRenderFlag = flag;
}


void ImageRenderWidget::paintEvent(QPaintEvent * event){

    if(renderFlag){
        QPainter painter(this);

        float imageWidht = render_image.width();
        float imageHeight = render_image.height();
        float ratio = (float)render_image.width()/(float)render_image.height();
        float scale=1.0;

        if((float)this->rect().width() / (float)this->rect().height() >= ratio){
            render_image = render_image.scaledToHeight(this->rect().height());
            scale = render_image.height()/imageHeight;
        }
        else{
            render_image = render_image.scaledToWidth(this->rect().width());
            scale = render_image.width()/imageWidht;
        }

        if(render_image.isNull()){
            return;
        }

        QTransform defautl_transform = painter.transform();
        QTransform transform = painter.transform();
        qreal m11 = transform.m11();    // Horizontal scaling
        qreal m12 = transform.m12();    // Vertical shearing
        qreal m13 = transform.m13();    // Horizontal Projection
        qreal m21 = transform.m21();    // Horizontal shearing
        qreal m22 = transform.m22();    // vertical scaling
        qreal m23 = transform.m23();    // Vertical Projection
        qreal m31 = transform.m31();    // Horizontal Position (DX)
        qreal m32 = transform.m32();    // Vertical Position (DY)
        qreal m33 = transform.m33();    // Addtional Projection Factor

        // We need this in a minute
        qreal trasform_scale = m11;

        // Horizontal flip
        m11 = -m11;

        // Re-position back to origin
        if(m31 > 0)
            m31 = 0;
        else
            m31 = (render_image.width() * trasform_scale);

        // Write back to the matrix
        transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);

        // Set the items transformation
        painter.setTransform(transform);

        painter.drawImage(0,0,render_image);
        painter.setTransform(defautl_transform);
        if(posRenderFlag){
            for(int i=0;i<8;i++){
                QPen myPen = QPen(colorList[i]);
                myPen.setWidth(10);
                painter.setPen(myPen);
                for(int j=0;j<postureDataList[i].size();j++){
                    painter.drawPoint(postureDataList[i][j]*scale);
                }

                myPen.setWidth(5);
                painter.setPen(myPen);
                if(postureDataList[i].size()>=25){
                    for(int j=0;j<JointSetList.size();j++){
                        painter.drawLine(postureDataList[i][JointSetList[j].first]*scale,postureDataList[i][JointSetList[j].second]*scale);
                    }


                    QPoint neckP = postureDataList[i][2]*scale;
                    QPoint headP = postureDataList[i][3]*scale;
                    QPoint headTopP = headP + (headP-neckP);
                    myPen.setColor(Qt::red);
                    myPen.setWidth(10);
                    painter.setPen(myPen);
                    painter.drawPoint(headTopP);

                    float length = QLineF(headTopP, neckP).length();
                    QPoint V1 = QPoint(headTopP-neckP)/length;

                    QPoint LeftTop = headTopP + length*0.5*0.678*QPoint(-V1.y(),V1.x());  //0.678は、AISTデータの男性の全頭高の平均値（238.7mm）に対する頭幅の平均値（161.9mm）の比率（暫定値）
                    QPoint RightTop = headTopP + length*0.5*0.678*QPoint(V1.y(),-V1.x());
                    QPoint LeftBottom = neckP + length*0.5*0.678*QPoint(-V1.y(),V1.x());
                    QPoint RightBottom = neckP + length*0.5*0.678*QPoint(V1.y(),-V1.x());

                    myPen.setWidth(5);
                    myPen.setColor(QColor(255,128,0));
                    painter.setPen(myPen);
                    painter.drawRect(QRect(RightTop,LeftBottom));//上記の計算は、上向きをy軸、右向きをx軸とした場合の計算。画像データは、y軸が逆なので、90度回転が逆になるため、RightTopの値が実際には左上の座標値になっている。


                }
            }
        }


        painter.setFont(QFont("Arial",12));
        if(FPS>25){
            painter.setPen(QPen(Qt::blue));
        }
        else{
            painter.setPen(QPen(Qt::red));

        }
        painter.drawText(0,20,QString("FPS:%1").arg(FPS));

    }
    //update();

}
