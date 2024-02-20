using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents a game object within the Scene.
    /// </summary>
    public class Entity
    {
        public readonly ulong ID;

        public string Name => Runtime.Entity_GetName(ID);

        public Entity()
        {
            ID = 0;
        }

        internal Entity(ulong id)
        {
            ID = id;
        }

        public override bool Equals(object obj)
        {
            return obj is Entity entity && entity.ID == ID;
        }

        public override int GetHashCode()
        {
            return ID.GetHashCode();
        }

        public static bool operator ==(Entity left, Entity right)
        {
            if(left is null)
            {
                return right is null;
            }

            return left.Equals(right);
        }

        public static bool operator !=(Entity left, Entity right) => !(left == right);

        public override string ToString()
        {
            return $"{Name} {ID:X}";
        }
    }
}
