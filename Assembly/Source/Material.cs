using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Material : Asset
    {
        public MaterialTemplate Template
        {
            get => Runtime.Material_GetTemplate(ID) as MaterialTemplate;
        }

        internal Material(UUID id)
            : base(id)
        { }

        public Material(string path)
            : base(Runtime.Material_Create(path))
        { }

        /// <summary>
        /// Sets the value of the Material Shader input with the given name.
        /// </summary>
        /// <param name="name">The name of the Shader input.</param>
        /// <param name="value">The value of the Shader input.</param>
        public void SetInput<T>(string name, T t) where T : struct
        {
            Runtime.Material_SetInput(ID, name, (int)typeof(T).ToMintyType(), t.ToString());
        }
    }
}
