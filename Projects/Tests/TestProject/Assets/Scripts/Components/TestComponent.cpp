#include "TestComponent.h"

using namespace minty;
using namespace game;

void game::TestComponent::serialize(Writer &writer) const
{
    writer.write("rotationSpeed", rotationSpeed, Vector3());
}

void game::TestComponent::deserialize(Reader const &reader)
{
    rotationSpeed = reader.read_vector3("rotationSpeed");
}