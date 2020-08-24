#pragma once

#include <memory>

#include "../basecommon.h"
#ifdef ENABLE_BLAS
#define EIGEN_USE_BLAS
#endif
#include <Eigen/Dense>

#include "math.hpp"

namespace PROJ_NAMESPACE
{

namespace Yutils
{

namespace Math
{

class SYMBOL_SHOW Matrix
{
public:

    static std::shared_ptr<Matrix> create() NOTHROW;

    Eigen::Matrix4d data() const NOTHROW;

    void setData(Eigen::Matrix4d &data) NOTHROW;

    // Set matrix to default / no transformation
    void identity() NOTHROW;

    // Multiplies matrix with given one
    void multiply(std::shared_ptr<Matrix> &other) NOTHROW;

    // Applies translation to matrix
    void translate(double x, double y, double z) NOTHROW;

    // Applies translation to matrix
    void scale(double x, double y, double z) NOTHROW;

    // Applies rotation to matrix
    void rotate(std::string &axis, double angle) THROW;

    // Inverses matrix
    void inverse() NOTHROW;

    // Applies matrix to point
    Eigen::Vector4d
    transform(double x, double y, double z, double w) NOTHROW;

private:

    Matrix() {}

    Matrix(const Matrix &) = delete;

    Matrix& operator=(const Matrix &other) = delete;

    Matrix& operator=(Matrix &&other) = delete;

    Eigen::Matrix4d m_data;

};

} //end namespace Math

} //end namespace Yutils

} //end namespace PROJ_NAMESPACE
