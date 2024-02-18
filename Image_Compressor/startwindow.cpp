#include "startwindow.h"
#include "ui_startwindow.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QScreen>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QMessageBox>

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

void StartWindow::closeEvent(QCloseEvent *bar)
{
    if(!selectedImage){
        qApp->exit();
    }
    bar->accept();
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void StartWindow::dropEvent(QDropEvent* e)
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
                selectedImage = true;
                emit childClosed();
                emit setImage(fname);
                close();

            }
        }
    }
}

void StartWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C://",
        "Image File (*.jpg *.png)"
        );
    selectedImage = true;
    emit childClosed();
    emit setImage(filePath);
    close();
}

