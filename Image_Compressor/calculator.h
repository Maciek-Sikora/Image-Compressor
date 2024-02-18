#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>

#include <Eigen/dense>
#include <Eigen/Core>
#include "opencv2/core/eigen.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"




class Calculator
{
public:
    Calculator(QString& pathToImage);
    void ComputeRsvd(int k);
    QImage matrixXdToQImage(Eigen::MatrixXd& matrix);
    QPixmap qPixmap_reconstructed_image;


private:
    QString path;
    cv::Mat image;
    cv::Mat w;
    cv::Mat u;
    cv::Mat vt;
    Eigen::MatrixXd eigen_matrix;



};

#endif // CALCULATOR_H
