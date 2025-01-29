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
        public Viewport Viewport
        {
            get => Runtime.Shader_GetViewport(ID) as Viewport;
            set => Runtime.Shader_SetViewport(ID, value.ID);
        }

        public Scissor Scissor
        {
            get => Runtime.Shader_GetScissor(ID) as Scissor;
            set => Runtime.Shader_SetScissor(ID, value.ID);
        }

        internal Shader(UUID id)
            : base(id)
        { }

        public Shader(string path)
            : base(Runtime.Shader_Create(path))
        { }

        /// <summary>
        /// Checks if this Shader has an input with the given name.
        /// </summary>
        /// <param name="name">The name to check.</param>
        /// <returns>True if this Shader has an input with the given name, otherwise false.</returns>
        public bool HasInput(string name)
        {
            return Runtime.Shader_HasInput(ID, name);
        }
    }
}
