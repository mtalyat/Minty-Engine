#include "pch.h"
#include "M_Quaternion.h"

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

std::string minty::to_string(Quaternion const& value)
{
    return std::format("Quaternion({})", to_string(value.to_euler_angles()));
}

std::string minty::to_string(glm::quat const& value)
{
    return std::format("Quaternion({})", to_string(glm::eulerAngles(value)));
}
