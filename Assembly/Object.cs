using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// The base that all MintyEngine types derive from.
    /// </summary>
    public abstract class Object
    {
        public static void Destroy(Entity entity)
        {
            Runtime.Object_DestroyEntity(entity.ID);
        }

        public static void DestroyImmediate(Entity entity)
        {
            Runtime.Object_DestroyImmediateEntity(entity.ID);
        }

        public static void Destroy(Component component)
        {
            Runtime.Entity_RemoveComponent(component.Entity.ID, component.GetType());
        }
    }
}
