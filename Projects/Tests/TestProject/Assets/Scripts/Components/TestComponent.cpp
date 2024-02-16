#include "TestComponent.h"

using namespace minty;
using namespace game;

void game::TestComponent::serialize(Writer &writer) const
{
    writer.write("rotationSpeed", rotationSpeed);
}

void game::TestComponent::deserialize(Reader const &reader)
{
    rotationSpeed = reader.read_object<Vector3>("rotationSpeed");
}
