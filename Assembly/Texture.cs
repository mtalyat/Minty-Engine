using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public class Texture : Asset
    {
        public Texture(Image image, Filter filter, ImageAddressMode addressMode, bool normalizedCoordinates)
            : base(Runtime.Texture_Create(image.ID, filter, addressMode, normalizedCoordinates))
        {

        }
    }
}
