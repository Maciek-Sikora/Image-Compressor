#include "calculator.h"
#include "randomized_svd.h"


Calculator::Calculator(QString& pathToImage):  path(pathToImage)
{
    cv::Mat cvImage = cv::imread(path.toStdString());
    if (cvImage.empty()) {
        qDebug() << "Failed to load image!";
        return;
    }

    cv::split(cvImage, channels);

    // Normalize the pixel values to the range [0, 1]
    for (int ch = 0; ch < 3; ch++) {
        channels[ch].convertTo(cvImage, CV_64F, 1.0 / 255.0);
        cv::cv2eigen(channels[ch], eigen_matrices[ch]);
    }


}

void Calculator::matrixXdToQImage(Eigen::MatrixXd& matrix, int channel) {
    int width = matrix.cols();
    int height = matrix.rows();

    if (qImage_merged_image.width() != width || qImage_merged_image.height() != height) {
        qWarning() << "Dimensions of matrix and QImage do not match";
        return;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QRgb currentColor = qImage_merged_image.pixel(x, y);
            int red = qRed(currentColor);
            int green = qGreen(currentColor);
            int blue = qBlue(currentColor);

            int pixelValue = static_cast<int>(matrix(y, x));
            pixelValue = std::max(0, std::min(255, pixelValue));
            if (channel == 0) {
                blue = pixelValue;
            } else if (channel == 1) {
                green = pixelValue;
            } else if (channel == 2) {
                red = pixelValue;
            }
            QRgb color = qRgb(red, green, blue);
            qImage_merged_image.setPixel(x, y, color);
        }
    }

}
void Calculator::ComputeRsvd(int k)
{
    std::vector<QImage> reconstructedImages(3);
    qImage_merged_image = QImage(channels[0].cols, channels[0].rows, QImage::Format_RGB32);
    for (int ch = 0; ch < 3; ch++) {
        RandomizedSvd rsvd(eigen_matrices[ch].cast<double>(), k);

        const Eigen::VectorXd& singularValues = rsvd.singularValues();
        const Eigen::MatrixXd& U = rsvd.matrixU();
        const Eigen::MatrixXd& V = rsvd.matrixV();

        Eigen::MatrixXd reconstructed_matrix = U * singularValues.asDiagonal() * V.transpose();
        qInfo() << "Rec " << reconstructed_matrix(0,0);
        matrixXdToQImage(reconstructed_matrix, ch);
        qInfo() << "Done for channel" << ch;
    }

    qPixmap_merged_image = QPixmap::fromImage(qImage_merged_image);
}
