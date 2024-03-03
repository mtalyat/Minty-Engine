#include "pch.h"
#include "M_Quaternion.h"

#include "M_Reader.h"
#include "M_Writer.h"

using namespace minty;

Vector3 minty::forward(Quaternion const& value)
{
    return glm::normalize(value * Vector3(0.0f, 0.0f, 1.0f));
}

Vector3 minty::backward(Quaternion const& value)
{
    return glm::normalize(value * Vector3(0.0f, 0.0f, -1.0f));
}

Vector3 minty::right(glm::quat const& value)
{
    return glm::normalize(value * Vector3(1.0f, 0.0f, 0.0f));
}

Vector3 minty::left(Quaternion const& value)
{
    return glm::normalize(value * Vector3(-1.0f, 0.0f, 0.0f));
}

Vector3 minty::up(Quaternion const& value)
{
    return glm::normalize(value * Vector3(0.0f, -1.0f, 0.0f));
}

Vector3 minty::down(Quaternion const& value)
{
    return glm::normalize(value * Vector3(0.0f, 1.0f, 0.0f));
}

std::istream& minty::operator>>(std::istream& stream, Quaternion& quat)
{
    Vector3 euler;
    stream >> euler;
    Quaternion other = from_euler(euler);
    quat.x = other.x;
    quat.y = other.y;
    quat.z = other.y;
    quat.w = other.w;
    return stream;
}

std::ostream& minty::operator<<(std::ostream& stream, Quaternion const& quat)
{
    stream << to_euler(quat);
    return stream;
}

Vector3 minty::to_euler(Quaternion const& value)
{
    return glm::eulerAngles(value) * Math::RAD2DEG;
}

Quaternion minty::from_euler(Vector3 const& value)
{
    return Quaternion(value * Math::DEG2RAD);
}

String minty::to_string(Quaternion const& value)
{
    return std::format("Quaternion({})", to_string(to_euler(value)));
}
