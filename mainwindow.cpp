#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cameraview.h>
#include <QLineEdit>
#include <QFileDialog>
#include <QFile>
//https://www.stev.org/post/raspberrypisimplertspserver
using namespace cv ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->labelPicture, SIGNAL( clicked(QMouseEvent*)), SLOT(lableClicked(QMouseEvent*)));
    pic = cv::imread("/home/pouya/1.jpg") ;
    //    cv::resize(pic,pic,cv::Size(ui->scrollArea->width(),ui->scrollArea->height())) ;
    QImage Qpic = ASM::cvMatToQImage(pic) ;
    ui->labelPicture->setPixmap(QPixmap::fromImage(Qpic));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonGetPicture_clicked()
{
    cameraview = new CameraView(this);
    cameraview->exec();
    pic = cameraview->getPic() ;
    if(!pic.empty()) {
        cv::Mat resized ;
        cv::resize(pic,resized,cv::Size(ui->scrollArea->width(),ui->scrollArea->height())) ;
        QImage Qpic = ASM::cvMatToQImage(resized) ;
        ui->labelPicture->setPixmap(QPixmap::fromImage(Qpic));
    }
    std::cout << pic.cols << std::endl;
    cameraview->~CameraView();

}
void MainWindow::lableClicked(QMouseEvent* e) {

    if(!pic.empty()) {

        if (picSet) {
            if(e->button()==Qt::LeftButton) {
                if(pixelpoints.size()<4)
                    pixelpoints.push_back(cv::Point(e->x(),e->y()));
            }

            if(e->button()==Qt::RightButton ) {
                if(pixelpoints.size()!=0)
                    pixelpoints.pop_back();
            }

            cv::Mat drawpic ;
            pic.copyTo(drawpic);

            ui->labelP1X->setText("0");
            ui->labelP1Y->setText("0");
            ui->labelP2X->setText("0");
            ui->labelP2Y->setText("0");
            ui->labelP3X->setText("0");
            ui->labelP3Y->setText("0");
            ui->labelP4X->setText("0");
            ui->labelP4Y->setText("0");

            switch (pixelpoints.size()) {
            case 4:
                ui->labelP4X->setText(QString::number(pixelpoints[3].x));
                ui->labelP4Y->setText(QString::number(pixelpoints[3].y));
            case 3:
                ui->labelP3X->setText(QString::number(pixelpoints[2].x));
                ui->labelP3Y->setText(QString::number(pixelpoints[2].y));
            case 2:
                ui->labelP2X->setText(QString::number(pixelpoints[1].x));
                ui->labelP2Y->setText(QString::number(pixelpoints[1].y));
            case 1:
                ui->labelP1X->setText(QString::number(pixelpoints[0].x));
                ui->labelP1Y->setText(QString::number(pixelpoints[0].y));
            }

            if(pixelpoints.size()!=0)
                for(cv::Point point : pixelpoints)
                    cv::circle(drawpic,point,6,cv::Scalar(0,255,0),-1);



            QImage Qpic = ASM::cvMatToQImage(drawpic) ;
            ui->labelPicture->setPixmap(QPixmap::fromImage(Qpic));
        }

        else {
            QImage Qpic = ASM::cvMatToQImage(pic) ;
            ui->labelPicture->setPixmap(QPixmap::fromImage(Qpic));
            picSet = true ;
        }
    }


}






void MainWindow::on_pushButton_clicked()
{
    cv::Point2f pixel[] = {cv::Point2f(pixelpoints[0].x,pixelpoints[0].y),
                           cv::Point2f(pixelpoints[1].x,pixelpoints[1].y),
                           cv::Point2f(pixelpoints[2].x,pixelpoints[2].y),
                           cv::Point2f(pixelpoints[3].x,pixelpoints[3].y)} ;
    cv::Point2f real[] =  {cv::Point2f(ui->lineEditX1->text().toFloat(),ui->lineEditY1->text().toFloat()),
                           cv::Point2f(ui->lineEditX2->text().toFloat(),ui->lineEditY2->text().toFloat()),
                           cv::Point2f(ui->lineEditX3->text().toFloat(),ui->lineEditY3->text().toFloat()),
                           cv::Point2f(ui->lineEditX4->text().toFloat(),ui->lineEditY4->text().toFloat())} ;
    cv::Mat warpMatrix = cv::getPerspectiveTransform(pixel,real) ;

    cv::warpPerspective(pic, fixed, warpMatrix, cv::Size(ui->lineEditX4->text().toInt(),ui->lineEditY4->text().toInt()), INTER_LINEAR, BORDER_CONSTANT);
    QImage Qpic = ASM::cvMatToQImage(fixed) ;
    ui->labelPicture->setPixmap(QPixmap::fromImage(Qpic));

}

void MainWindow::on_pushButton_2_clicked()
{
    if(!fixed.empty()) {
        QString save_path = QFileDialog::getSaveFileName(this,"Save file","","Corel Files (*.cdr)") ;
        QFile outfile( save_path );
        if(save_path!="" && outfile.open(QIODevice::WriteOnly | QIODevice::Text)) {

            QTextStream out(&outfile);

            out <<  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            out << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n" ;
            out << "<svg xmlns=\"http://www.w3.org/2000/svg\" xml:space=\"preserve\" width=\"8.5in\" height=\"11in\" version=\"1.1\" style=\"shape-rendering:geometricPrecision;"
                   " text-rendering:geometricPrecision; image-rendering:optimizeQuality; fill-rule:evenodd; clip-rule:evenodd\" "
                   "viewBox=\"0 0 8500 11000\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n" ;
            out << " <g id=\"Layer_x0020_1_0\">\n" ;

            cv::Mat threshold ;
            cv::cvtColor(fixed,threshold,cv::COLOR_BGR2GRAY) ;
            cv::threshold(threshold,threshold,130,255,cv::THRESH_BINARY_INV) ;
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;

            cv::findContours( threshold, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );

            int idx = 0;
            for( ; idx >= 0; idx = hierarchy[idx][0] )
            {

                Scalar color( rand()&255, rand()&255, rand()&255 );
                cv::drawContours(fixed, contours, idx, color, 1, 8, hierarchy );
            }

            for (uint var = 0; var < contours.size(); ++var) {
                QString points = "<polyline points=\"" ;
                for (uint idx = 0; idx < contours[var].size(); ++idx) {
                    points+= QString::number(contours[var][idx].x) + " " + QString::number(contours[var][idx].y) ;
                    if(idx!=contours[var].size()-1)
                        points+="," ;
                }
                points += "\" stroke=\"red\" stroke-width=\"4\" fill=\"none\" />" ;
                out << points << "\n";
            }
            QImage Qpic = ASM::cvMatToQImage(fixed) ;
            ui->labelPicture->setPixmap(QPixmap::fromImage(Qpic));

            out << "</g>" ;
            out << "</svg>" ;

            outfile.close();
        }



    }

}
