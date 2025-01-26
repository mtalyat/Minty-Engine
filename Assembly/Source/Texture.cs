using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Texture : Asset
    {
        internal Texture(UUID id)
            : base(id)
        { }

        public Texture(Image image, Filter filter, ImageAddressMode addressMode, bool normalizedCoordinates)
            : base(Runtime.Texture_Create(image.ID, filter, addressMode, normalizedCoordinates))
        { }
    }
}
