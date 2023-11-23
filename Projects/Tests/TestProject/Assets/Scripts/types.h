#include <Minty.h>

namespace game
{
    struct MaterialBufferObject
    {
        minty::ID textureId;
        int x;
        int y;
        int z;
        minty::Vector4 color;
    };
}