#include "randomized_svd.h"

RandomizedSvd::RandomizedSvd(const MatrixXd& m, int rank, int oversamples, int iter)
    : U_(), V_(), S_() {
    ComputeRandomizedSvd(m, rank, oversamples, iter);
}

VectorXd RandomizedSvd::singularValues() { return S_; }
MatrixXd RandomizedSvd::matrixU() { return U_; }
MatrixXd RandomizedSvd::matrixV() { return V_; }

void RandomizedSvd::ComputeRandomizedSvd(const MatrixXd& A, int rank, int oversamples, int iter) {
    using namespace std::chrono;

    // If matrix is too small for desired rank/oversamples
    if((rank + oversamples) > fmin(A.rows(), A.cols())) {
        rank = fmin(A.rows(), A.cols());
        oversamples = 0;
    }

    MatrixXd Q = FindRandomizedRange(A, rank + oversamples, iter);
    MatrixXd B = Q.transpose() * A;

    Eigen::JacobiSVD<MatrixXd> svd(B, Eigen::ComputeThinU | Eigen::ComputeThinV);

    U_ = (Q * svd.matrixU()).block(0, 0, A.rows(), rank);
    V_ = svd.matrixV().block(0, 0, A.cols(), rank);
    S_ = svd.singularValues().head(rank);
}

MatrixXd RandomizedSvd::FindRandomizedRange(const MatrixXd& A, int size, int iter) {
    int nr = A.rows(), nc = A.cols();
    MatrixXd L(nr, size);
    Eigen::FullPivLU<MatrixXd> lu1(nr, size);
    MatrixXd Q = MatrixXd::Random(nc, size);
    Eigen::FullPivLU<MatrixXd> lu2(nc, nr);


    for (int i = 0; i < iter; ++i) {
        lu1.compute(A * Q);
        L.setIdentity();
        L.block(0, 0, nr, size).triangularView<Eigen::StrictlyLower>() =
            lu1.matrixLU();

        lu2.compute(A.transpose() * L);
        Q.setIdentity();
        Q.block(0, 0, nc, size).triangularView<Eigen::StrictlyLower>() =
            lu2.matrixLU();
    }

    Eigen::ColPivHouseholderQR<MatrixXd> qr(A * Q);
    return qr.householderQ() * MatrixXd::Identity(nr, size);
}
