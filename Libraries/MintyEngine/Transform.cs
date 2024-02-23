using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public class Transform : Component
    {
        public Vector3 LocalPosition
        {
            get
            {
                Runtime.Transform_GetLocalPosition(Entity.ID, out Vector3 position);
                return position;
            }
            set => Runtime.Transform_SetLocalPosition(Entity.ID, value);
        }

        public Quaternion LocalRotation
        {
            get
            {
                Runtime.Transform_GetLocalRotation(Entity.ID, out Quaternion rotation);
                return rotation;
            }
            set => Runtime.Transform_SetLocalRotation(Entity.ID, value);
        }

        public Vector3 LocalScale
        {
            get
            {
                Runtime.Transform_GetLocalScale(Entity.ID, out Vector3 scale);
                return scale;
            }
            set => Runtime.Transform_SetLocalScale(Entity.ID, value);
        }
    }
}
