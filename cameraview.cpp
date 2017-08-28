#include "cameraview.h"
#include "ui_cameraview.h"




CameraView::CameraView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CameraView)
{
    ui->setupUi(this);
    cap = new cv::VideoCapture("rtsp://192.168.1.21:8554/test") ;
    if(!cap->isOpened())
        return ;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePicture()));
    timer->start(50);
}

CameraView::~CameraView()
{
    delete ui;
}
cv::Mat CameraView::getPic() {
    return pic ;
}

void CameraView::updatePicture() {
    if (!cap->read(frame)) return;
    cv::Mat resized ;
    cv::resize(frame,resized,cv::Size(ui->label->width(),ui->label->height())) ;
    QImage pic = ASM::cvMatToQImage(resized) ;

    ui->label->setPixmap(QPixmap::fromImage(pic));
    QCoreApplication::processEvents();
}

void CameraView::on_pushButton_clicked()
{
    timer->stop();
    frame.copyTo(pic);

}
