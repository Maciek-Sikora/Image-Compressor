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
#include "opencv2/core/eigen.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"




class Calculator : public QObject
{
    Q_OBJECT

public:
    Calculator(QString& pathToImage);
    void ComputeRsvd(int k);
    void matrixXdToQImage(Eigen::MatrixXd& matrix, int channel);
    QImage qImage_merged_image;
    QPixmap qPixmap_merged_image;


private:
    QString path;
    cv::Mat image;
    cv::Mat channels[3];
    cv::Mat w;
    cv::Mat u;
    cv::Mat vt;
    Eigen::MatrixXd eigen_matrices[3];

};

#endif // CALCULATOR_H
