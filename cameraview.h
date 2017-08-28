#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QDialog>
#include "asmOpenCV.h"
#include "opencv2/highgui/highgui.hpp"
#include <QTimer>

namespace Ui {
class CameraView;
}

class CameraView : public QDialog
{
    Q_OBJECT

public:
    explicit CameraView(QWidget *parent = 0);
    ~CameraView();
    cv::Mat getPic();
private slots:
    void updatePicture();

    void on_pushButton_clicked();

private:
    Ui::CameraView *ui;
    cv::Mat pic ;

    cv::VideoCapture *cap ;
    cv::Mat frame ;
    QTimer * timer;
};

#endif // CAMERAVIEW_H
