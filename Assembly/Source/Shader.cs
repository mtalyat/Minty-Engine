using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Shader : Asset
    {
        internal Shader(UUID id)
            : base(id)
        { }

        public Shader(string path)
            : base(Runtime.Shader_Create(path))
        { }
    }
}
