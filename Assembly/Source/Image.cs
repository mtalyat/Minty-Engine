using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Image : Asset
    {
        internal Image(UUID id)
            : base(id)
        { }

        public Image(Format format, ImageType type, ImageTiling tiling, ImageAspect aspect, ImageUsage usage, uint width, uint height, bool immutable)
            : base(Runtime.Image_Create((int)format, (int)type, (int)tiling, (int)aspect, (int)usage, width, height, immutable))
        { }
    }
}
