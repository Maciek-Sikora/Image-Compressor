#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QScreen>
#include <QObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    startWindow = new StartWindow(this);
    QObject::connect(startWindow, SIGNAL(childSignal(const QString&)), this, SLOT(setImage(const QString&)));
    startWindow->show();
    setAcceptDrops(true);
    qDebug() << "Opencv version: " CV_VERSION;
    this->setMaximumSize(qApp->screens()[0]->size());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* e)
{
    QStringList accepted_types;
    accepted_types << "jpg" << "png";
    foreach(const QUrl & url, e->mimeData()->urls())
    {
        QString fname = url.toLocalFile();
        QFileInfo info(fname);
        if (info.exists())
        {
            if (accepted_types.contains(info.suffix().trimmed(), Qt::CaseInsensitive)){
                qInfo() << "Selected File: " << fname << "\n info " << info ;
                setImage(fname);
            }
        }
    }
}
void MainWindow::setImage(QString filePath){
    if (!filePath.isEmpty()) {
        qInfo() << "Selected File: " << filePath;
        QPixmap image(filePath);
        int w = ui->label_Image->width();
        int h = ui->label_Image->height();
        // set a scaled pixmap to a w x h window keeping its aspect ratio
        ui->label_Image->setPixmap(image.scaled(w,h,Qt::KeepAspectRatioByExpanding));
        // ui->label_Image->setPixmap(image);
        calculator = new Calculator(filePath);

    } else {
        qWarning() << "No file selected.";
    }
}
void MainWindow::setImage(QPixmap qPixmap){

    int w = ui->label_Image->width();
    int h = ui->label_Image->height();
    // set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->label_Image->setPixmap(qPixmap.scaled(w,h,Qt::KeepAspectRatioByExpanding));
}
void MainWindow::on_pushButton_SelectPath_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C://",
        "Image File (*.jpg *.png)"
        );
    setImage(filePath);
}



void MainWindow::on_horizontalSlider_sliderReleased()
{
    calculator->ComputeRsvd(ui->horizontalSlider->value());

   setImage(calculator->qPixmap_reconstructed_image);
}

