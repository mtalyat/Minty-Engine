using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Transform : Component
    {
        public Vector3 LocalPosition
        {
            get
            {
                Runtime.Transform_GetLocalPosition(ID, out Vector3 position);
                return position;
            }
            set => Runtime.Transform_SetLocalPosition(ID, value);
        }

        public Quaternion LocalRotation
        {
            get
            {
                Runtime.Transform_GetLocalRotation(ID, out Vector4 rotation);
                return new Quaternion(rotation.X, rotation.Y, rotation.Z, rotation.W);
            }
            set => Runtime.Transform_SetLocalRotation(ID, new Vector4(value.X, value.Y, value.Z, value.W));
        }

        public Vector3 LocalScale
        {
            get
            {
                Runtime.Transform_GetLocalScale(ID, out Vector3 scale);
                return scale;
            }
            set => Runtime.Transform_SetLocalScale(ID, value);
        }

        public Vector3 GlobalPosition
        {
            get
            {
                Runtime.Transform_GetGlobalPosition(ID, out Vector3 position);
                return position;
            }
        }

        public Quaternion GlobalRotation
        {
            get
            {
                Runtime.Transform_GetGlobalRotation(ID, out Vector4 rotation);
                return new Quaternion(rotation.X, rotation.Y, rotation.Z, rotation.W);
            }
        }

        public Vector3 GlobalScale
        {
            get
            {
                Runtime.Transform_GetGlobalScale(ID, out Vector3 scale);
                return scale;
            }
        }

        public Vector3 Right
        {
            get
            {
                Runtime.Transform_GetRight(ID, out Vector3 right);
                return right;
            }
        }

        public Vector3 Up
        {
            get
            {
                Runtime.Transform_GetUp(ID, out Vector3 up);
                return up;
            }
        }

        public Vector3 Forward
        {
            get
            {
                Runtime.Transform_GetForward(ID, out Vector3 forward);
                return forward;
            }
        }

        internal Transform(UUID id)
            : base(id)
        { }
    }
}
