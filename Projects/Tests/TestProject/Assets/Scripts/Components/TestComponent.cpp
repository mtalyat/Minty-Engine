#include "TestComponent.h"

using namespace minty;
using namespace game;

void game::TestComponent::serialize(Writer &writer) const
{
    writer.write_object("rotationSpeed", rotationSpeed, Vector3());
}

void game::TestComponent::deserialize(Reader const &reader)
{
    reader.read_object("rotationSpeed", rotationSpeed, Vector3());
}
