#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QPainter>
#include <Eigen/dense>
#include <Eigen/Core>
#include <QtConcurrent>
#include <QFuture>
#include <QThread>
#include <QMessageBox>
#include "opencv2/core/eigen.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class Calculator : public QObject
{
    Q_OBJECT

public:
    explicit Calculator(QString& pathToImage);
    ~Calculator();
    void ComputeRsvd(int k);
    void matrixXdToQImage(Eigen::MatrixXd& matrix, int channel);
    QImage qImage_merged_image;
    QPixmap qPixmap_merged_image;

signals:

    void progressValueChangedMy(float progress);

private:
    QString path;
    cv::Mat channels[3];
    cv::Mat w;
    cv::Mat u;
    cv::Mat vt;
    Eigen::MatrixXd eigen_matrices[3];

    void RsvdChannel(int channel, int k);

    long long ticks;
    long long allTicks;
    long long ticksClock;
    void tick();

    std::atomic<bool> stopThreads;
};

#endif // CALCULATOR_H
