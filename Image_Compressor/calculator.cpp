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

    // Ensure that the dimensions of the matrix match the dimensions of the QImage
    if (qImage_merged_image.width() != width || qImage_merged_image.height() != height) {
        qWarning() << "Dimensions of matrix and QImage do not match";
        return;
    }

    // Iterate over each pixel of the image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Get the current color value at this position in the image
            QRgb currentColor = qImage_merged_image.pixel(x, y);
            int red = qRed(currentColor);
            int green = qGreen(currentColor);
            int blue = qBlue(currentColor);

            // Set the pixel value for the corresponding channel from the matrix
            int pixelValue = static_cast<int>(matrix(y, x));
            pixelValue = std::max(0, std::min(255, pixelValue));
            if (channel == 0) {
                blue = pixelValue;
            } else if (channel == 1) {
                green = pixelValue;
            } else if (channel == 2) {
                red = pixelValue;
            }

            // Set the new color
            QRgb color = qRgb(red, green, blue);
            qImage_merged_image.setPixel(x, y, color);
        }
    }

}
void Calculator::ComputeRsvd(int k)
{
    std::vector<QImage> reconstructedImages(3); // Store reconstructed images for each channel
    qImage_merged_image = QImage(channels[0].cols, channels[0].rows, QImage::Format_RGB32);
    // qImage_merged_image.setColor(1, qRgb(255,255,255));

    // Compute RSVD for each channel and store reconstructed images
    for (int ch = 0; ch < 3; ch++) {
        RandomizedSvd rsvd(eigen_matrices[ch].cast<double>(), k);

        // Get singular values and matrices U and V
        const Eigen::VectorXd& singularValues = rsvd.singularValues();
        const Eigen::MatrixXd& U = rsvd.matrixU();
        const Eigen::MatrixXd& V = rsvd.matrixV();

        // Construct the reconstructed matrix
        Eigen::MatrixXd reconstructed_matrix = U * singularValues.asDiagonal() * V.transpose();
        qInfo() << "Rec " << reconstructed_matrix(0,0);
        // Store the reconstructed image for this channel
        matrixXdToQImage(reconstructed_matrix, ch);
        qInfo() << "Done for channel" << ch;
    }

    qPixmap_merged_image = QPixmap::fromImage(qImage_merged_image);
}
