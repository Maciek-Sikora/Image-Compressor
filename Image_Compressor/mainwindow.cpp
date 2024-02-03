#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Opencv version: " CV_VERSION;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_SelectPath_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C://",
        "JPG File (*.jpg)"
        );


    if (!filePath.isEmpty()) {
        QTextStream qin(stdin);
        qInfo() << "Selected File: " << filePath;

        QPixmap image(filePath);
        ui->label_Image->setPixmap(image);

        cv::Mat cvImage = cv::imread(filePath.toStdString(), cv::IMREAD_COLOR);
        if (!cvImage.empty()) {
            cv::imshow("Display window", cvImage);
            cv::waitKey(0);
        } else {
            qCritical() << "Error loading image using OpenCV.";
        }
    } else {
        qWarning() << "No file selected.";
    }
}
