#include <Minty.h>

namespace game
{
    struct MaterialBufferObject
    {
        int textureId;
        int x, y, z;
        minty::Vector4 color;
    };
}