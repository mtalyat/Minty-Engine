using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Scissor : Asset
    {
        internal Scissor(UUID id)
            : base(id)
        { }

        public Scissor(int x, int y, uint width, uint height)
            : base(Runtime.Scissor_Create(x, y, width, height))
        {

        }
    }
}
