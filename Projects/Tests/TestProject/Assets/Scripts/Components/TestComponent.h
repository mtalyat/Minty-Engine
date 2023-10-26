#pragma once

#include <Minty.h>

namespace game
{
    struct TestComponent
        : public minty::Component
    {
        float translation;
        float rotation;
        float scale;
        float translateSpeed;
        float rotateSpeed;
        float scaleSpeed;

        void serialize(minty::Writer& writer) const override;
        void deserialize(minty::Reader const& reader) override;
    };
}