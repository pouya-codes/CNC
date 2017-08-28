#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>


#include "cameraview.h"
#include <QMouseEvent>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonGetPicture_clicked();
    void lableClicked(QMouseEvent* e) ;

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    CameraView *cameraview ;
    cv::Mat pic , fixed ;
    bool picSet = false ;
    std::vector<cv::Point> pixelpoints ;
    std::vector<cv::Point> realpoints ;
};

#endif // MAINWINDOW_H
