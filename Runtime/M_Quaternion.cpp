#include "pch.h"
#include "M_Quaternion.h"

#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

minty::Quaternion::Quaternion(glm::quat const& other)
    : glm::quat(other)
{}

Vector3 minty::Quaternion::forward() const
{
    return glm::normalize(*this * Vector3(0.0f, 0.0f, 1.0f));
}

Vector3 minty::Quaternion::backward() const
{
    return glm::normalize(*this * Vector3(0.0f, 0.0f, -1.0f));
}

Vector3 minty::Quaternion::right() const
{
    return glm::normalize(*this * Vector3(1.0f, 0.0f, 0.0f));
}

Vector3 minty::Quaternion::left() const
{
    return glm::normalize(*this * Vector3(-1.0f, 0.0f, 0.0f));
}

Vector3 minty::Quaternion::up() const
{
    return glm::normalize(*this * Vector3(0.0f, 1.0f, 0.0f));
}

Vector3 minty::Quaternion::down() const
{
    return glm::normalize(*this * Vector3(0.0f, -1.0f, 0.0f));
}

Vector3 minty::Quaternion::to_euler_angles() const
{
    return glm::eulerAngles(*this);
}

Quaternion minty::Quaternion::from_euler_angles(float const x, float const y, float const z)
{
    return Quaternion(Vector3(x, y, z));
}

Quaternion minty::Quaternion::from_euler_angles(Vector3 const v)
{
    return Quaternion(v);
}

std::istream& minty::operator>>(std::istream& stream, Quaternion& quat)
{
    Vector3 euler;
    stream >> euler;
    Quaternion other(euler);
    quat.x = other.x;
    quat.y = other.y;
    quat.z = other.y;
    quat.w = other.w;
    return stream;
}

std::ostream& minty::operator<<(std::ostream& stream, const Quaternion& quat)
{
    stream << quat.to_euler_angles();
    return stream;
}

String minty::to_string(Quaternion const& value)
{
    return std::format("Quaternion({})", to_string(value.to_euler_angles()));
}
