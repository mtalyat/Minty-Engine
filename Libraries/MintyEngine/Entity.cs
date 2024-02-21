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
    public class Entity : Object
    {
        public readonly ulong ID;

        public string Name
        {
            get => Runtime.Entity_GetName(ID);
            set => Runtime.Entity_SetName(ID, value);
        }

        protected Entity()
        {
            ID = 0;
            Console.WriteLine("Empty Entity");
        }

        internal Entity(ulong id)
        {
            ID = id;
            Console.WriteLine(id);
            Console.WriteLine($"{this}");
        }

        public T AddComponent<T>() where T : Component
        {
            return Runtime.Entity_AddComponent(ID, typeof(T)) as T;
        }

        public T GetComponent<T>() where T : Component
        {
            return Runtime.Entity_GetComponent(ID, typeof(T)) as T;
        }

        public void RemoveComponent<T>() where T : Component
        {
            Runtime.Entity_RemoveComponent(ID, typeof(T));
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
