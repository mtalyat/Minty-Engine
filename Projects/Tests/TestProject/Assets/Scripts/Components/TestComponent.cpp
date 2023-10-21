#include "TestComponent.h"

using namespace minty;
using namespace game;

void game::TestComponent::serialize(Writer &writer) const
{
    writer.write("translateSpeed", translateSpeed, 0.0f);
    writer.write("rotateSpeed", rotateSpeed, 0.0f);
    writer.write("scaleSpeed", scaleSpeed, 0.0f);
}

void game::TestComponent::deserialize(Reader const &reader)
{
    translateSpeed = reader.read_float("translateSpeed");
    rotateSpeed = reader.read_float("rotateSpeed");
    scaleSpeed = reader.read_float("scaleSpeed");
}
