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
            get => Runtime.Transform_GetLocalPosition(Entity.ID);
            set => Runtime.Transform_SetLocalPosition(Entity.ID, value);
        }
    }
}
