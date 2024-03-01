#include "calculator.h"
#include "randomized_svd.h"


Calculator::Calculator(QString& pathToImage):  path(pathToImage), stopThreads(false)
{
    cv::Mat cvImage = cv::imread(path.toStdString());
    if (cvImage.empty()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Image doesn't exist !");
        messageBox.setFixedSize(500,200);
        return;
    }

    cv::split(cvImage, channels);

    for (int ch = 0; ch < 3; ch++) {
        channels[ch].convertTo(channels[ch], CV_64F, 1.0 / 255.0);
        cv::cv2eigen(channels[ch], eigen_matrices[ch]);
    }


}

Calculator::~Calculator()
{
    stopThreads = true;
}

void Calculator::matrixXdToQImage(Eigen::MatrixXd& matrix, int channel) {
    int width = matrix.cols();
    int height = matrix.rows();

    if (qImage_merged_image.width() != width || qImage_merged_image.height() != height) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Dimensions of matrix and QImage do not match !");
        messageBox.setFixedSize(500,200);
        return;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            if (stopThreads) {
                qInfo();
                return;
            }

            QRgb currentColor = qImage_merged_image.pixel(x, y);
            int red = qRed(currentColor);
            int green = qGreen(currentColor);
            int blue = qBlue(currentColor);

            int pixelValue = static_cast<int>(matrix(y, x) * 255);
            pixelValue = std::max(0, std::min(255, pixelValue));

            switch (channel) {
            case 0:
                blue = pixelValue;
                break;
            case 1:
                green = pixelValue;
                break;
            case 2:
                red = pixelValue;
                break;
            }

            QRgb color = qRgb(red, green, blue);
            qImage_merged_image.setPixel(x, y, color);
        }
    }

}
void Calculator::RsvdChannel(int channel, int k){
    RandomizedSvd rsvd(eigen_matrices[channel].cast<double>(), k);

    const Eigen::VectorXd& singularValues = rsvd.singularValues();
    const Eigen::MatrixXd& U = rsvd.matrixU();
    const Eigen::MatrixXd& V = rsvd.matrixV();

    Eigen::MatrixXd reconstructed_matrix = U * singularValues.asDiagonal() * V.transpose();
    matrixXdToQImage(reconstructed_matrix, channel);
    qInfo() << "Done for channel" << channel;
}

void Calculator::ComputeRsvd(int k)
{
    qImage_merged_image = QImage(channels[0].cols, channels[0].rows, QImage::Format_RGB32);

    QFuture <void> channelBlue = QtConcurrent::run(&Calculator::RsvdChannel, this, 0, k);
    QFuture <void> channelGreen = QtConcurrent::run(&Calculator::RsvdChannel, this, 1, k);
    QFuture <void> channelRed = QtConcurrent::run(&Calculator::RsvdChannel, this, 2, k);

    channelBlue.waitForFinished();
    channelGreen.waitForFinished();
    channelRed.waitForFinished();
    if (qImage_merged_image.isNull()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Failed to create merged image !");
        messageBox.setFixedSize(500,200);
        return;
    }

    qPixmap_merged_image = QPixmap::fromImage(qImage_merged_image);
}
