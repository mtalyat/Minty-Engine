using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Viewport : Asset
    {
        internal Viewport(UUID id)
            : base(id)
        { }

        public Viewport(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f, float minDepth = 0.0f, float maxDepth = 1.0f)
            : base(Runtime.Viewport_Create(x, y, width, height, minDepth, maxDepth))
        { }
    }
}
