using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class RenderTarget : Asset
    {
        internal RenderTarget(UUID id) 
            : base(id)
        { }

        private RenderTarget(bool framebuffer)
            : base(Runtime.RenderTarget_Create(framebuffer))
        {

        }

        public static RenderTarget CreateScreenRenderTarget()
        {
            return new RenderTarget(true);
        }

        public static RenderTarget CreateTextureRenderTarget()
        {
            return new RenderTarget(false);
        }
    }
}
