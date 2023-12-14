#pragma once

#include <Minty.h>

namespace game
{
    struct TestComponent
        : public minty::Component
    {
        minty::Vector3 rotation;
        minty::Vector3 rotationSpeed;

        void serialize(minty::Writer& writer) const override;
        void deserialize(minty::Reader const& reader) override;
    };
}