using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public class Image : Asset
    {


        public Image(Format format, ImageType type, ImageTiling tiling, ImageAspect aspect, ImageUsage usage, uint width, uint height, bool immutable)
            : base(Runtime.Image_Create(format, type, tiling, aspect, usage, width, height, immutable))
        {

        }
    }
}
