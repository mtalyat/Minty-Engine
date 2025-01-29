using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class MeshRenderer : Component
    {
        public MeshType Type
        {
            get => (MeshType)Runtime.MeshRenderer_GetType(Entity.ID);
            set => Runtime.MeshRenderer_SetType(Entity.ID, (int)value);
        }

        public Mesh Mesh
        {
            get => Runtime.MeshRenderer_GetMesh(Entity.ID) as Mesh;
            set => Runtime.MeshRenderer_SetMesh(Entity.ID, value.ID);
        }

        public Material Material
        {
            get => Runtime.MeshRenderer_GetMaterial(Entity.ID) as Material;
            set => Runtime.MeshRenderer_SetMaterial(Entity.ID, value.ID);
        }

        internal MeshRenderer(UUID id)
            : base(id)
        { }
    }
}
