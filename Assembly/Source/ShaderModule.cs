using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public class ShaderModule : Asset
    {
        public ShaderModule(string path)
            : base(Runtime.ShaderModule_Create(path))
        {

        }
    }
}
