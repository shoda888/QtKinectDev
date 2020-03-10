#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QMessageBox>


template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
    if (pInterfaceToRelease != NULL) {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = NULL;
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

 /*

    cv::VideoCapture cap(0);//デバイスのオープン


        if(!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
        {
            //読み込みに失敗したときの処理
            return;
        }
            cv::Mat frame;
            cap >> frame;
    cap.release();


qDebug() << frame.rows << frame.cols;
   // QImage image("C:/usr/kitamura/sMySoft/QtKinectRecorder/x64/QtKinectRecorder/20170726/2/jpg/0000000001.jpg");
    char *data;
    qDebug() << sizeof(data)/sizeof(uchar);
    //qDebug() << data;

    qDebug() << frame.data;

    QImage image( frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888 );
    qDebug() << image.save("test.jpg");

    qDebug() << image.bits();

    QTime timer;

    timer.start();
    for(int i=0;i<10;i++){
        QImage image( frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888 );
        image.save(QString("test%1.jpg").arg(i));
    }



    qDebug() << "Time:" << timer.elapsed();
    timer.start();
    for(int i=0;i<10;i++){
        QFile file(QString("test%1.dat").arg(i));
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream << data;
       // file.write((const char*)data,640*480);
        file.close();
    }
    qDebug() << "Time:" << timer.elapsed();

    for(int i=0;i<10;i++){
        QFile file(QString("test%1.dat").arg(i));
        file.open(QIODevice::ReadOnly);
        file.read(data,640*480);
        file.close();

        QImage image( (uchar *)data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888 );
        qDebug() << "i:"<<i<<image.save(QString("second%1.jpg").arg(i));



    }
*/


    connect(ui->actionStart_Capture,SIGNAL(triggered(bool)),this,SLOT(startCapture()));
    connect(ui->actionStop_Capture,SIGNAL(triggered(bool)),this,SLOT(stopCapture()));


    QDockWidget *renderDocWidget = new QDockWidget("render");
    renderWidget = new ImageRenderWidget(renderDocWidget);
    renderDocWidget->setWidget(renderWidget);

    QDockWidget *menuDocWidget = new QDockWidget("Menu");
    //menuDocWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    QWidget *menuWidget = createMenuWidget();
    menuDocWidget->setWidget(menuWidget);

    addDockWidget(Qt::TopDockWidgetArea,menuDocWidget);

    this->setCentralWidget(renderDocWidget);

    //qDebug() << this->thread();

    threadSize =4 ;
    imgThreadSize = 1;

    for(int i=0;i<threadSize;i++){
        imageSaveFuncList.push_back(new ImageSave());
        imageSaveThreadList.push_back(new QThread);
        imageSaveFuncList[i]->moveToThread(imageSaveThreadList[i]);
        connect(imageSaveThreadList[i],SIGNAL(started()),imageSaveFuncList[i],SLOT(imageSave()));
        //connect(imageSaveFuncList[i],SIGNAL(finishedSaving()),this,SLOT(convertToMovie()));
        connect(imageSaveFuncList[i],SIGNAL(imageSaveErrorOccured(QImage,QString)),this,SLOT(checkImage(QImage,QString)));

        //qDebug() << "thread"<< i << ":" << imageSaveThreadList[i];
    }

    for(int i=0;i<1;i++){
        posDataSaveFuncList.push_back(new PosDataThread());
        posDataSaveThreadList.push_back(new QThread);
        posDataSaveFuncList[i]->moveToThread(posDataSaveThreadList[i]);
        connect(posDataSaveThreadList[i],SIGNAL(started()),posDataSaveFuncList[i],SLOT(posDataSave()));
        //connect(imageSaveFuncList[i],SIGNAL(finishedSaving()),this,SLOT(convertToMovie()));

        //qDebug() << "thread"<< i << ":" << imageSaveThreadList[i];
    }

    for(int i=0;i<1;i++){
        depthSaveFuncList.push_back(new DepthDataSaveFunc());
        depthSaveThreadList.push_back(new QThread);
        depthSaveFuncList[i]->moveToThread(depthSaveThreadList[i]);
        connect(depthSaveThreadList[i],SIGNAL(started()),depthSaveFuncList[i],SLOT(depthSave()));
        //connect(imageSaveFuncList[i],SIGNAL(finishedSaving()),this,SLOT(convertToMovie()));

        //qDebug() << "thread"<< i << ":" << imageSaveThreadList[i];
    }




    threadIndex = 0;
    for(int i=1;i<threadSize;i++){
        imageSaveFuncList[i]->enablePosDataHandle(false);
    }



   // qDebug() << imageSaveFunc->thread();

    imgIndex = 1;
    depthIndex = 1;
    saveMode = false;
    intervalFlag = false;
    timerID=-1;


}

void MainWindow::checkImage(QImage image,QString fileName){

    qDebug() << "Image Check:" << image.save("temp.jpg","JPG")<< image.isNull() << fileName;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startCapture(){

    HRESULT hResult = S_OK;
    hResult = GetDefaultKinectSensor(&pSensor);
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("Kinectの接続が確認できません。"));
        return;
    }

    // Open Sensor
    hResult = pSensor->Open();
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("Kinectと接続ができません。"));
        return;
    }

    // Retrieved Coordinate Mapper

    hResult = pSensor->get_CoordinateMapper(&pCoordinateMapper);
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("座標情報が取得できません。"));
        return;
    }

    // Retrieved Color Frame Source

    hResult = pSensor->get_ColorFrameSource(&pColorSource);
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("RGBデータ取得の初期化に失敗しました。"));
        return ;
    }


    // Retrieved Depth Frame Source

    hResult = pSensor->get_DepthFrameSource(&pDepthSource);
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("デプスデータ取得の初期化に失敗しました。"));
        return ;
    }

    // Open Color Frame Reader

    hResult = pColorSource->OpenReader(&pColorReader);
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("RGBデータ取得の初期化に失敗しました。"));
        return ;
    }

    // Open Depth Frame Reader

    hResult = pDepthSource->OpenReader(&pDepthReader);
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("デプスデータ取得の初期化に失敗しました。"));
        return ;
    }

    // Retrieved Color Frame Size

    hResult = pColorSource->get_FrameDescription(&pColorDescription);
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("RGBデータ情報の取得に失敗しました。"));
        return ;
    }

    pColorDescription->get_Width(&colorWidth); // 1920
    pColorDescription->get_Height(&colorHeight); // 1080

                                                 // To Reserve Color Frame Buffer
    colorBuffer = std::vector<RGBQUAD> (colorWidth * colorHeight);

    // Retrieved Depth Frame Size

    hResult = pDepthSource->get_FrameDescription(&pDepthDescription);
    if (FAILED(hResult)) {
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("デプスデータ情報の取得に失敗しました。"));
        return ;
    }

    pDepthDescription->get_Width(&depthWidth); // 512
    pDepthDescription->get_Height(&depthHeight); // 424

    //qDebug() <<"Depth size:" << depthWidth << depthHeight;

    // To Reserve Depth Frame Buffer
    depthBuffer = std::vector<UINT16>(depthWidth * depthHeight);
    depthData = new uchar[depthWidth * depthHeight];



    //Body

    hResult = pSensor->get_BodyFrameSource(&body_frame_source);
    if (FAILED(hResult)) {
      QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("姿勢データ取得の初期化に失敗しました。"));
      return ;
    }



    hResult = body_frame_source->OpenReader(&body_frame_reader);
    if (FAILED(hResult)) {
      QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("姿勢データ情報の取得に失敗しました。"));
      return ;
    }

    hResult = pSensor->get_CoordinateMapper( &pCoordinateMapper );  //……2
    if( FAILED( hResult ) ){
      QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("姿勢データ情報の取得に失敗しました。"));
      return ;
    }

    timerID = startTimer(1);

}


void MainWindow::stopCapture(){

    if(timerID!=-1){
        killTimer(timerID);
        timerID=-1;

        // End Processing
        SafeRelease(pColorSource);
        SafeRelease(pDepthSource);
        SafeRelease(body_frame_source);
        SafeRelease(pColorReader);
        SafeRelease(pDepthReader);
        SafeRelease(body_frame_reader);
        SafeRelease(pColorDescription);
        SafeRelease(pDepthDescription);
        SafeRelease(pCoordinateMapper);



        if (pSensor) {
            pSensor->Close();
        }
        SafeRelease(pSensor);
    }


}


void MainWindow::closeEvent(QCloseEvent *event){

    stopCapture();


}

QWidget *MainWindow::createMenuWidget(){


    QWidget *menuWidget = new QWidget();
    QVBoxLayout *vLayout = new QVBoxLayout(menuWidget);

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    QLabel *saveDirLabel = new QLabel(QString::fromLocal8Bit("保存先："));
    saveDirLineEdit = new QLineEdit();
    QToolButton *saveDirToolButton = new QToolButton(menuWidget);

    hLayout1->addWidget(saveDirLabel);
    hLayout1->addWidget(saveDirLineEdit);
    hLayout1->addWidget(saveDirToolButton);

    connect(saveDirToolButton,SIGNAL(clicked(bool)),this,SLOT(openDirToSave()));


    QHBoxLayout *hLayout2 = new QHBoxLayout();

    QCheckBox *renderCheckBox = new QCheckBox(QString::fromLocal8Bit("画像の描画"),menuWidget);
    renderCheckBox->setChecked(true);

    posRenderCheckBox = new QCheckBox(QString::fromLocal8Bit("姿勢の描画"),menuWidget);
    posRenderCheckBox->setChecked(true);

    QLabel *intervalLabel = new QLabel(QString::fromLocal8Bit("Interval(ms):"),menuWidget);
    intervalSpinBox = new QSpinBox(menuWidget);
    intervalSpinBox->setRange(1,60000);
    intervalSpinBox->setValue(1);

    QFont myFont =this->font();
    myFont.setPointSize(11);
    startRecordingPushButton = new QPushButton(QString::fromLocal8Bit("録画開始"),menuWidget);
    startRecordingPushButton->setCheckable(true);
    startRecordingPushButton->setFont(myFont);
    startRecordingPushButton->setStyleSheet("QPushButton {color:black;}QPushButton:checked{color:red;}");
    QPushButton *stopRecordingPushButton = new QPushButton(QString::fromLocal8Bit("録画停止"),menuWidget);
    stopRecordingPushButton->setFont(myFont);
    QSpacerItem *spacer = new QSpacerItem(80,5,QSizePolicy::Expanding);


    QLabel *rgbImageSizeLabel = new QLabel(QString::fromLocal8Bit("画像保存サイズ:"),menuWidget);
    rgbImageSizeComboBox = new QComboBox(menuWidget);
    rgbImageSizeComboBox->addItem("1920x1080");
    rgbImageSizeComboBox->addItem("1280x720");
    rgbImageSizeComboBox->addItem("960x540");
    rgbImageSizeComboBox->addItem("640x360");
    rgbImageSizeComboBox->setCurrentIndex(2);

    hLayout2->addWidget(startRecordingPushButton);
    hLayout2->addWidget(stopRecordingPushButton);
    hLayout2->addWidget(renderCheckBox);
    hLayout2->addWidget(posRenderCheckBox);
    hLayout2->addWidget(intervalLabel);
    hLayout2->addWidget(intervalSpinBox);
    hLayout2->addWidget(rgbImageSizeLabel);
    hLayout2->addWidget(rgbImageSizeComboBox);

    hLayout2->addItem(spacer);

    connect(renderCheckBox,SIGNAL(clicked(bool)),renderWidget,SLOT(enableRendering(bool)));
    connect(posRenderCheckBox,SIGNAL(clicked(bool)),renderWidget,SLOT(enablePosRendering(bool)));

    connect(startRecordingPushButton,SIGNAL(clicked(bool)),this,SLOT(startRecording()));
    connect(stopRecordingPushButton,SIGNAL(clicked(bool)),this,SLOT(stopRecording()));


    vLayout->addLayout(hLayout1);
    //vLayout->addWidget(renderCheckBox);
    vLayout->addLayout(hLayout2);

    return menuWidget;

}

void MainWindow::openDirToSave(){

    QString currentDir = saveDirLineEdit->text();
    QString newDir = QFileDialog::getExistingDirectory(0,"Save dir",currentDir);
    if(newDir.isEmpty()){
        return;
    }

    recordIndex = 1;

    saveDirLineEdit->setText(newDir);

}


int rgbCount=0;
int counter=0;
void MainWindow::startRecording(){

    if(saveDirLineEdit->text().isEmpty()){
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("保存先が指定されていません。"));
    }
    saveToDirPath = saveDirLineEdit->text();

    QDir saveDir(saveToDirPath);
    saveDir.mkdir(QString("%1").arg(recordIndex));
    saveDir.mkdir(QString("%1/jpg").arg(recordIndex));
    saveDir.mkdir(QString("%1/pos").arg(recordIndex));
    saveDir.mkdir(QString("%1/depth").arg(recordIndex));

    int width = rgbImageSizeComboBox->currentText().section("x",0,0).toInt();
    int height = rgbImageSizeComboBox->currentText().section("x",1,1).toInt();


    imgIndex = 1;
    depthIndex = 1;
    counter = 0;
    rgbCount = 0;
    saveMode = true;
    intervalFlag = false;
    saveIntervalTime.start();

    for(int i=0;i<threadSize;i++){
        imageSaveFuncList[i]->setSaveImageSize(width,height);
        imageSaveFuncList[i]->setSaveDir(saveToDirPath+QString("/%1").arg(recordIndex));
        imageSaveFuncList[i]->roopFlag = true;
    }
    imageSaveThreadList[0]->start();

    posDataSaveFuncList[0]->setSaveDir(saveToDirPath+QString("/%1").arg(recordIndex));
    posDataSaveThreadList[0]->start();
    posDataSaveFuncList[0]->roopFlag = true;

    depthSaveFuncList[0]->setSaveDir(saveToDirPath+QString("/%1").arg(recordIndex));
    depthSaveThreadList[0]->start();
    depthSaveFuncList[0]->roopFlag = true;




    //imageSaveFunc->imageSave();




}


void MainWindow::stopRecording(){

    saveMode = false;
    startRecordingPushButton->setChecked(false);

    if(threadSize>1){
        for(int i=0;i<threadSize;i++){
            if(imageSaveThreadList[i]->isRunning()){
                imageSaveThreadList[i]->quit();
            }
        }

        int size = imageSaveFuncList[0]->getImageQueueSize();
        imageSaveFuncList[1]->setImageQueue(imageSaveFuncList[0]->getImageQueue((int)floor(size/2)));
        imageSaveFuncList[1]->setFileNameQueue(imageSaveFuncList[0]->getFileNameQueue((int)floor(size/2)));

        for(int i=0;i<threadSize;i++){
            imageSaveThreadList[i]->start();
        }

    }


//#ifdef OLDVer
    for(int i=0;i<threadSize;i++){
        if(imageSaveThreadList[i]->isRunning()){
            pBarList.push_back(new QProgressBar);
            pBarList.last()->setWindowTitle(QString("Image Data Save Process#%1").arg(i+1));
            pBarList.last()->setFormat("%v/%m");
            pBarList.last()->setMaximum(imageSaveFuncList[i]->getImageQueueSize());
            qDebug() << "Max:" << pBarList.last()->maximum();
            connect(imageSaveFuncList[i],SIGNAL(saveProcess(int)),pBarList.last(),SLOT(setValue(int)));
            //connect(imageSaveFuncList[i],SIGNAL(updateMaximum(int)),pBarList.last(),SLOT(setMaximum(int)));
            connect(imageSaveFuncList[i],SIGNAL(finishedSaving()),pBarList.last(),SLOT(close()));
            imageSaveFuncList[i]->roopFlag = false;
            pBarList.last()->show();
        }
    }
//#endif

    for(int i=0;i<1;i++){
        if(posDataSaveThreadList[i]->isRunning()){
            pBarList.push_back(new QProgressBar);
            pBarList.last()->setWindowTitle(QString("Posture Data Save Process#%1").arg(i+1));
            pBarList.last()->setFormat("%v/%m");
            pBarList.last()->setMaximum(posDataSaveFuncList[i]->getQueueSize());
            connect(posDataSaveFuncList[i],SIGNAL(saveProcess(int)),pBarList.last(),SLOT(setValue(int)));
            //connect(posDataSaveFuncList[i],SIGNAL(updateMaximum(int)),pBarList.last(),SLOT(setMaximum(int)));
            connect(posDataSaveFuncList[i],SIGNAL(finishedSaving()),pBarList.last(),SLOT(close()));
            posDataSaveFuncList[i]->roopFlag = false;
            pBarList.last()->show();
        }
    }

    for(int i=0;i<1;i++){
        if(depthSaveThreadList[i]->isRunning()){
            pBarList.push_back(new QProgressBar);
            pBarList.last()->setWindowTitle(QString("Depth Data Save Process#%1").arg(i+1));
            pBarList.last()->setFormat("%v/%m");
            pBarList.last()->setMaximum(depthSaveFuncList[i]->getQueueSize());
            connect(depthSaveFuncList[i],SIGNAL(saveProcess(int)),pBarList.last(),SLOT(setValue(int)));
            //connect(depthSaveFuncList[i],SIGNAL(updateMaximum(int)),pBarList.last(),SLOT(setMaximum(int)));
            connect(depthSaveFuncList[i],SIGNAL(finishedSaving()),pBarList.last(),SLOT(close()));
            depthSaveFuncList[i]->roopFlag = false;
            pBarList.last()->show();
        }
    }


    imgIndex = 1;
    depthIndex = 1;

    ui->statusBar->showMessage(QString("CapturedSize:%1").arg(counter));

    qDebug() << "CaptureCount:" << counter;
    qDebug() << "rgbCount:" << rgbCount;

    recordIndex++;

}


void MainWindow::convertToMovie(){



    QProcess *process = new QProcess();
    QStringList args;
    QString fileName = saveToDirPath.section("\\",-1,-1);
    args << "-f" << "image2" << "-r" "30" "-i" <<  saveToDirPath+"\\+%10d.jpg" << "-r" << "30" << "-an" << "-vcodec" << "libx264" << "-movflags" << "faststart" << "-pix_fmt" << "yuv420p" <<  fileName+".mp4";

    //qDebug() << process->arguments();
    process->start("ffmpeg.exe",args);

    connect(process,SIGNAL(finished(int)),this,SLOT(finishConvertingToMovie(int)));


}

void MainWindow::finishConvertingToMovie(int exitCode){

    if(exitCode==0){
        QMessageBox::information(0,"Information",QString::fromLocal8Bit("動画の保存が完了しました。"));
    }
    else{
        QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("動画の保存に失敗しました。"));
    }

}



#define ImageDataSize 1920*1080*4
void MainWindow::timerEvent(QTimerEvent *event){

    HRESULT hResult = S_OK;




    IColorFrame* pColorFrame = nullptr;
    hResult = pColorReader->AcquireLatestFrame(&pColorFrame);

    if (SUCCEEDED(hResult)) {
        // Retrieved Color Data

        QImage image = QImage(1920,1080,QImage::Format_RGB32);

        hResult = pColorFrame->CopyConvertedFrameDataToArray(ImageDataSize, reinterpret_cast<BYTE*>(image.bits()), ColorImageFormat::ColorImageFormat_Bgra);
        //hResult = pColorFrame->CopyConvertedFrameDataToArray(buffer.size(), &buffer[0], ColorImageFormat::ColorImageFormat_Bgra);


        //hResult = pColorFrame->CopyConvertedFrameDataToArray(colorBuffer.size() * sizeof(RGBQUAD), reinterpret_cast<BYTE*>(&colorBuffer[0]), ColorImageFormat::ColorImageFormat_Bgra);
        if (FAILED(hResult)) {
            QMessageBox::warning(0,"Warning",QString::fromLocal8Bit("Depthデータの取得に失敗しました。"));
        }

        if(saveMode){

            counter++;
        }

        QString now = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz");

        //QImage image = QImage(reinterpret_cast<BYTE*>(colorBuffer.data()),1920,1080,QImage::Format_RGB32);//Format_RGBA8888

        if(image.isNull()){
            qDebug() << "image isNull!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        }

        //image = image.mirrored(true,false);
        /*
        if(image.isNull()){
            image = capturedImage;
            saveDirLineEdit->setEnabled(false);
        }
        else{
            saveDirLineEdit->setEnabled(true);
        }
        */

#ifdef Yamamoto
        if(!intervalFlag){
            qDebug() << saveIntervalTime.elapsed();
            if(saveIntervalTime.elapsed() >= intervalSpinBox->value()){
                intervalFlag = true;
            }
        }
#endif
        renderWidget->updateImage(image);
        if(saveMode){
#ifdef Yamamoto
            if(intervalFlag){
#endif
               imageSaveFuncList[threadIndex]->addImage(image,imgIndex);
               /*
               if(threadSize>1){
                   threadIndex++;
                   threadIndex = threadIndex%threadSize;
               }
               */
               imgIndex++;
               rgbCount++;

               saveIntervalTime.restart();
#ifdef Yamamoto
            }
#endif
        }

        SafeRelease(pColorFrame);

        //capturedImage = image;

        //Depth


        IDepthFrame* pDepthFrame = nullptr;
        hResult = pDepthReader->AcquireLatestFrame(&pDepthFrame);
        if (SUCCEEDED(hResult)) {
            // Retrieved Depth Data
            hResult = pDepthFrame->CopyFrameDataToArray(depthBuffer.size(), &depthBuffer[0]);
            if (FAILED(hResult)) {
                qDebug() << "Error : IDepthFrame::CopyFrameDataToArray()";
            }


            //QImage depthImage = QImage(depthWidth,depthHeight,QImage::Format_RGB32);
            //QPainter painter(&depthImage);

/*
            for( int y = 0; y < depthHeight; y++ ){
                for( int x = 0; x < depthWidth; x++ ){
                    UINT16 depth = depthBuffer[y * depthWidth + x];

                    double value = depth/8000.0*255.0;
                    if(value>255.0){
                        value = 255.0;
                    }
                    //qDebug() << value;
                    QColor color = QColor(value,value,value);
                    QPen myPen = QPen(color);
                    painter.setPen(myPen);
                    painter.drawPoint(x,y);
                }
            }
*/
            if(saveMode){
#ifdef Yamamoto
                if(intervalFlag){
#endif
                    //qDebug() << "thread:" << threadIndex << "imgIndex:" << imgIndex;
                   //imageSaveFuncList[0]->addDepthData(depthBuffer,depthIndex,now);
                   //imageSaveFuncList[0]->addDepthData(depthBuffer,depthIndex);
                    depthSaveFuncList[0]->addDepthData(depthBuffer,depthIndex);
                   depthIndex++;
#ifdef Yamamoto
                }
#endif
            }

            //renderWidget->updateImage(depthImage);
        }
        SafeRelease(pDepthFrame);


        //Body
        IBodyFrame* pBodyFrame = nullptr;
        hResult = body_frame_reader->AcquireLatestFrame( &pBodyFrame );

        if( SUCCEEDED( hResult ) ){
            IBody* pBody[BODY_COUNT] = { 0 };
            static QVector<QVector3D> postureDataVec[BODY_COUNT];
            hResult = pBodyFrame->GetAndRefreshBodyData( BODY_COUNT, pBody );

            if( SUCCEEDED( hResult ) ){
                for( int count = 0; count < BODY_COUNT; count++ ){

                    BOOLEAN bTracked = false;
                    hResult = pBody[count]->get_IsTracked( &bTracked );

                    if( SUCCEEDED( hResult ) && bTracked ){
                        //qDebug() << "FOUND!!!!!";
                        Joint joint[JointType::JointType_Count];
                        hResult = pBody[ count ]->GetJoints( JointType::JointType_Count, joint );
                        if( SUCCEEDED( hResult ) ){

                            // Joint
                            for( int type = 0; type < JointType::JointType_Count; type++ ){
                                if(saveMode){
                                    postureDataVec[count].push_back(QVector3D(joint[type].Position.X,joint[type].Position.Y,joint[type].Position.Z));
                                }

                                if(posRenderCheckBox->isChecked()){
                                    ColorSpacePoint colorSpacePoint = { 0 };
                                    pCoordinateMapper->MapCameraPointToColorSpace( joint[type].Position, &colorSpacePoint );
                                    int x = static_cast<int>( colorSpacePoint.X );
                                    int y = static_cast<int>( colorSpacePoint.Y );
                                    if( ( x >= 0 ) && ( x < colorWidth ) && ( y >= 0 ) && ( y < colorHeight ) ){
                                       // qDebug() << "ID:" << count << " index:"<<type << QPoint(x,y);
                                        renderWidget->setPostureData(count,type,QPoint(colorWidth-x,y));
                                       // cv::circle( bufferMat, cv::Point( x, y ), 30, static_cast< cv::Scalar >( cv::Vec3b( 0, 0, 255 ) ), -1, CV_AA );
                                    }
                                }
                            }
                            if(saveMode){
#ifdef Yamamoto
                                if(intervalFlag){
#endif                              //qDebug() << "addPosData";
                                    posDataSaveFuncList[0]->addPosData(count,imgIndex-1,postureDataVec[count]);
                                    //imageSaveFuncList[0]->addPosData(count,imgIndex-1,postureDataVec[count]);
                                    //imageSaveFuncList[0]->addPosData(count,imgIndex-1,postureDataVec[count],now);
#ifdef Yamamoto
                                }
#endif
                            }
                        }
                    }
                    else{
                        renderWidget->cleatPostureData(count);
                    }
                }
                //cv::resize( bufferMat, bodyMat, cv::Size(), 0.5, 0.5 );
            }

            for( int count = 0; count < BODY_COUNT; count++ ){
                SafeRelease( pBody[count] );
                postureDataVec[count].clear();
            }
            SafeRelease(pBodyFrame);

        }


#ifdef Yamamoto
        intervalFlag = false;
#endif

        renderWidget->setFPS(floor(1000.0/time.elapsed()));
        //qDebug() << time.elapsed();
        time.start();

    }
    /*
    else{

        ui->statusBar->showMessage(QString::fromLocal8Bit("カラー画像を取得できません。"));
        qDebug() << QString::fromLocal8Bit("カラー画像を取得できません。");
        BOOLEAN flag;
        pSensor->get_IsAvailable(&flag);
        if(!flag){
            ui->statusBar->showMessage("Not Available");
        }
        qDebug() << "get_IsAvailable:" << flag;
        pSensor->get_IsOpen(&flag);
        if(!flag){
            ui->statusBar->showMessage("Not Open");
        }
        qDebug() << "get_IsOpen:" << flag;

    }
    */


}
