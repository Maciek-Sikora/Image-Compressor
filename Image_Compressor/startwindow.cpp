#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::closeEvent(QCloseEvent *event)
{
    if (!selectedImage) {
        qApp->exit();
    }
    event->accept();
}

void StartWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void StartWindow::dropEvent(QDropEvent *event)
{
    QStringList acceptedTypes = {"jpg", "png"};
    foreach(const QUrl &url, event->mimeData()->urls())
    {
        QString filePath = url.toLocalFile();
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists() && acceptedTypes.contains(fileInfo.suffix(), Qt::CaseInsensitive)) {
            qInfo() << "Selected File: " << filePath << "\n info " << fileInfo;
            selectedImage = true;
            emit childClosed();
            emit setImage(filePath);
            close();
        } else {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","File doesn't exits or wrong type !");
            messageBox.setFixedSize(500,200);
        }
    }
}

void StartWindow::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QString(),
        "Image File (*.jpg *.png)"
        );
    if (filePath.isEmpty()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","File doesn't exits or wrong type !");
        messageBox.setFixedSize(500,200);
        return;
    }
    selectedImage = true;
    emit childClosed();
    emit setImage(filePath);
    close();
}

