using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Mesh : Asset
    {
        internal Mesh(UUID id)
            : base(id)
        { }

        public Mesh(string path)
            : base(Runtime.Mesh_Create(path))
        { }

        public Mesh(MeshType type)
            : base(Runtime.Mesh_CreateFromType((int)type))
        { }
    }
}
