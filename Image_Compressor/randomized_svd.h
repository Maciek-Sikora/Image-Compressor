#ifndef RANDOMIZED_SVD_H
#define RANDOMIZED_SVD_H

#include <chrono>
#include <cmath>
#include "Eigen/Dense"



using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::MatrixBase;


class RandomizedSvd {
public:
    RandomizedSvd(const MatrixXd& m, int rank, int oversamples = 10, int iter = 2);

    VectorXd singularValues();
    MatrixXd matrixU();
    MatrixXd matrixV();

private:
    MatrixXd U_, V_;
    VectorXd S_;

    void ComputeRandomizedSvd(const MatrixXd& A, int rank, int oversamples,
                              int iter);

    MatrixXd FindRandomizedRange(const MatrixXd& A, int size, int iter);
};



#endif // RANDOMIZED_SVD_H
