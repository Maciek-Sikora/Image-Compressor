#include "calculator.h"
#include "randomized_svd.h"


Calculator::Calculator(QString& pathToImage):  path(pathToImage)
{
    cv::Mat cvImage = cv::imread(path.toStdString(), cv::IMREAD_GRAYSCALE);

    // Normalize the pixel values to the range [0, 1]
    cvImage.convertTo(cvImage, CV_64F, 1.0 / 255.0);

    cv::cv2eigen(cvImage, Calculator::eigen_matrix);


}

QImage Calculator::matrixXdToQImage(Eigen::MatrixXd& matrix) {
    QImage image(matrix.cols(), matrix.rows(), QImage::Format_Grayscale8);
    for (int y = 0; y < matrix.rows(); ++y) {
        for (int x = 0; x < matrix.cols(); ++x) {
            // Scale the pixel value from [0, 1] to [0, 255] and convert to integer
            int pixelValue = static_cast<int>(matrix(y, x) * 255.0);
            // Ensure the pixel value is within [0, 255]
            pixelValue = std::max(0, std::min(255, pixelValue));
            // Set pixel value in QImage
            image.setPixel(x, y, qRgb(pixelValue, pixelValue, pixelValue));
        }
    }
    return image;
}

void Calculator::ComputeRsvd(int k)
{
    RandomizedSvd rsvd(eigen_matrix.cast<double>(), k);

    // Get singular values and matrices U and V
    const Eigen::VectorXd& singularValues = rsvd.singularValues();
    const Eigen::MatrixXd& U = rsvd.matrixU();
    const Eigen::MatrixXd& V = rsvd.matrixV();

    // Construct the reconstructed matrix
    Eigen::MatrixXd reconstructed_matrix = U * singularValues.asDiagonal() * V.transpose();

    // Convert the Eigen matrix back to OpenCV Mat using the Mat constructor
    qPixmap_reconstructed_image = QPixmap::fromImage(matrixXdToQImage(reconstructed_matrix));


}
