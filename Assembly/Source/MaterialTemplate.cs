using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class MaterialTemplate : Asset
    {
        public Shader Shader
        {
            get => Runtime.MaterialTemplate_GetShader(ID) as Shader;
        }

        internal MaterialTemplate(UUID id)
            : base(id)
        { }

        public MaterialTemplate(string path)
            : base(Runtime.MaterialTemplate_Create(path))
        { }
    }
}
