#include "calculator.h"
#include "randomized_svd.h"


Calculator::Calculator(QString& pathToImage):  path(pathToImage) {
    cv::Mat cvImage = cv::imread(path.toStdString(), 0);

    // Normalize the pixel values to the range [0, 1]
    cvImage.convertTo(cvImage, CV_64F, 1.0 / 255.0);

    Eigen::MatrixXd eigen_matrix;
    cv::cv2eigen(cvImage, eigen_matrix);

    RandomizedSvd rsvd(eigen_matrix.cast<double>(), 10);

    // Get singular values and matrices U and V
    const Eigen::VectorXd& singularValues = rsvd.singularValues();
    const Eigen::MatrixXd& U = rsvd.matrixU();
    const Eigen::MatrixXd& V = rsvd.matrixV();

    // Construct the reconstructed matrix
    Eigen::MatrixXd reconstructed_matrix = U * singularValues.asDiagonal() * V.transpose();

    // Convert the Eigen matrix back to OpenCV Mat using the Mat constructor
    cv::Mat reconstructed_image;
    cv::eigen2cv(reconstructed_matrix.cast<double>(), reconstructed_image);

    // Display or save the original and reconstructed images as needed
    cv::imshow("Original Image", cvImage);
    cv::imshow("Reconstructed Image", reconstructed_image);
    cv::waitKey(0);
}
