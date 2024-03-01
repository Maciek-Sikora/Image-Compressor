#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    startWindow = new StartWindow(this);
    QObject::connect(startWindow, SIGNAL(setImage(const QString&)), this, SLOT(onSetImage(const QString&)));
    QObject::connect(startWindow, SIGNAL(childClosed()), this, SLOT(displayMainWindow()));
    startWindow->show();
    ui->setupUi(this);
    setAcceptDrops(true);
    this->setMaximumSize(qApp->screens()[0]->size());
}
void MainWindow::displayMainWindow(){
    this->show();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete image;
    delete calculator;
    delete startWindow;
    if(watcher->isRunning())
        watcher->waitForFinished();
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
                qInfo() << "Selected File: " << fname << "\n";
                onSetImage(fname);
            } else {
                QMessageBox messageBox;
                messageBox.critical(0,"Error","File doesn't exits or wrong type !");
                messageBox.setFixedSize(500,200);
            }
        }
    }
}
void MainWindow::onSetImage(QString filePath){
    qInfo() << "Selected File: " << filePath;

    if (filePath.isEmpty())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","File doesn't exits or wrong type !");
        messageBox.setFixedSize(500,200);
        return;
    }

    image = new QPixmap(filePath);
    if (!checkPixmap(*image)){
        return;
    }

    int w = ui->label_Image->width();
    int h = ui->label_Image->height();
    ui->label_Image->setPixmap(image->scaled(w, h, Qt::KeepAspectRatioByExpanding));

    calculator = new Calculator(filePath);

    max_SingularValues = qMin(image->width(), image->height());
    k = max_SingularValues;
    updateStats(k);
}
void MainWindow::setImage(QPixmap &img){
    int w = ui->label_Image->width();
    int h = ui->label_Image->height();
    reconstructedImage = &img;
    ui->label_Image->setPixmap(img.scaled(w,h,Qt::KeepAspectRatioByExpanding));
}

void MainWindow::on_pushButton_SelectPath_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QString(),
        "Image File (*.jpg *.png)"
        );
    onSetImage(filePath);
}

bool MainWindow::checkPixmap(const QPixmap &qPixmap) {
    if (qPixmap.width() < 30 || qPixmap.height() < 30) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Image is too small !");
        messageBox.setFixedSize(500,200);
        return false;
    }

    if (qPixmap.width() * qPixmap.height() > 400000) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Image is too large !");
        messageBox.setFixedSize(500,200);
        return false;
    }
    return true;
}

void MainWindow::updateStats(long long singularValue){
    QString size = QString::number(image->width()) + " x " + QString::number(image->height());
    ui->label_Size->setText(size);

    pixels = image->width() * image->height();
    ui->label_Pixels->setText(QString::number(pixels));

    weigh = image->width() * image->height() * 3  * sizeof(double)/ (1024 * 1024);
    ui->label_Weigh->setText(QString::number(weigh, 'f', 2) + " MB");

    ui->label_MaxK->setText(QString::number(max_SingularValues));
    ui->label_SingularValues->setText(QString::number(singularValue));

    optimization = (image->width() * singularValue) + singularValue + (singularValue * image->height());
    ui->label_CompSize->setText(QString::number(optimization));

    svd_weigh = ((image->width() * singularValue) + singularValue + (singularValue * image->height())) * 3 * sizeof(double)/ (1024.0 * 1024.0);
    ui->label_CompWeigh->setText(QString::number(svd_weigh , 'f', 2) + " MB");

    svd_ratio = image->width() * image->height() / ((image->width() * singularValue) + singularValue + (singularValue * image->height()));
    ui->label_CompRatio->setText(QString::number(svd_ratio));

}
void MainWindow::blockUI(){
    ui->pushButton_SelectPath->setDisabled(true);
    ui->horizontalSlider->setDisabled(true);
    ui->pushButton_SaveImage->setDisabled(true);
    setAcceptDrops(false);
}
void MainWindow::onUnlockUI(){
    ui->pushButton_SelectPath->setDisabled(false);
    ui->horizontalSlider->setDisabled(false);
    ui->pushButton_SaveImage->setDisabled(false);
    setAcceptDrops(true);
}
void MainWindow::computeRsvd(int k) {

    calculator->ComputeRsvd(k);
    setImage(calculator->qPixmap_merged_image);
    emit computationFinished();
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    blockUI();
    k = static_cast<int>(ui->horizontalSlider->value() * max_SingularValues / 100) ;
    QFuture <void> future = QtConcurrent::run(&MainWindow::computeRsvd, this, k);
    watcher = new QFutureWatcher<void>(this);
    connect(watcher, SIGNAL(finished()), this, SLOT(onUnlockUI()), Qt::QueuedConnection);
    watcher->setFuture(future);
    updateStats(k);
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_SingularValues->setText(QString::number(value));
    updateStats(static_cast<int>(value * max_SingularValues / 100));
}


void MainWindow::on_pushButton_SaveImage_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image File"),
                                                    QString(),
                                                    tr("Images (*.jpg *.png)"));
    if (!fileName.isEmpty())
    {
        reconstructedImage->save(fileName);
    } else {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "The location doesn't exist !");
        messageBox.setFixedSize(500,200);
        return;
    }
}

