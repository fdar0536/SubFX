#include <new>

#include "YutilsCpp"

using namespace PROJ_NAMESPACE::Yutils::Math;

std::shared_ptr<Matrix> Matrix::create() NOTHROW
{
    Matrix *ret(new (std::nothrow) Matrix());
    if (!ret)
    {
        return nullptr;
    }

    ret->m_data = Eigen::Matrix4d::Identity();

    return std::shared_ptr<Matrix>(ret);
}

Eigen::Matrix4d Matrix::data() const NOTHROW
{
    return m_data;
}

void Matrix::setData(Eigen::Matrix4d &data) NOTHROW
{
    m_data = data;
}

void Matrix::identity() NOTHROW
{
    m_data = Eigen::Matrix4d::Identity();
}

void Matrix::multiply(std::shared_ptr<Matrix> &other) NOTHROW
{
    m_data *= other->m_data;
}

void Matrix::translate(double x, double y, double z) NOTHROW
{
    /* it generates
     * 1 0 0 x
     * 0 1 0 y
     * 0 0 1 z
     * 0 0 0 1
    */
    Eigen::Affine3d transform(Eigen::Translation3d(x, y, z));
    m_data *= transform.matrix();
}

void Matrix::scale(double x, double y, double z) NOTHROW
{
    Eigen::Affine3d scaling(Eigen::Scaling(x, y, z));
    m_data *= scaling.matrix();
}

void Matrix::rotate(std::string &axis, double angle) THROW
{
    if (axis.length() == 0)
    {
        throw std::invalid_argument("rotate: axis CANNOT be empty.");
    }

    Eigen::Affine3d q;
    switch (axis.at(0))
    {
    case 'x':
    case 'X':
    {
        q = Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitX());
        break;
    }
    case 'y':
    case 'Y':
    {
        q = Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitY());
        break;
    }
    case 'z':
    case 'Z':
    {
        q = Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitZ());
        break;
    }
    default:
    {
        throw std::invalid_argument("Invalid axis.");
    }
    } //end switch (axis.at(0))

    m_data *= q.matrix();
}

void Matrix::inverse() NOTHROW
{
    m_data = m_data.inverse();
}

Eigen::Vector4d
Matrix::transform(double x, double y, double z, double w) NOTHROW
{
    Eigen::Vector4d v(x, y, z, w);
    return v.adjoint() * m_data;
}
