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
                Runtime.Transform_GetLocalRotation(Entity.ID, out Vector4 rotation);
                return new Quaternion(rotation.X, rotation.Y, rotation.Z, rotation.W);
            }
            set => Runtime.Transform_SetLocalRotation(Entity.ID, new Vector4(value.X, value.Y, value.Z, value.W));
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

        public Vector3 Right
        {
            get
            {
                Runtime.Transform_GetRight(Entity.ID, out Vector3 right);
                return right;
            }
        }

        public Vector3 Up
        {
            get
            {
                Runtime.Transform_GetUp(Entity.ID, out Vector3 up);
                return up;
            }
        }

        public Vector3 Forward
        {
            get
            {
                Runtime.Transform_GetForward(Entity.ID, out Vector3 forward);
                return forward;
            }
        }

        internal Transform()
        { }
    }
}
