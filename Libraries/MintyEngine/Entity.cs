using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents a game object within the Scene that can have components and scripts attached to it.
    /// </summary>
    public class Entity : Object
    {
        internal readonly ulong ID;

        public string Name
        {
            get => Runtime.Entity_GetName(ID);
            set => Runtime.Entity_SetName(ID, value);
        }

        public string Tag
        {
            get => Runtime.Entity_GetTag(ID);
            set => Runtime.Entity_SetTag(ID, value);
        }

        public bool Enabled
        {
            get => Runtime.Entity_GetEnabled(ID);
            set => Runtime.Entity_SetEnabled(ID, value);
        }

        public Entity Parent
        {
            get => Runtime.Entity_GetParent(ID) as Entity;
            set => Runtime.Entity_SetParent(ID, value.ID);
        }

        public int ChildCount
        {
            get => Runtime.Entity_GetChildCount(ID);
        }

        protected Entity()
        {
            ID = 0;
        }

        internal Entity(ulong id)
        {
            ID = id;
        }

        #region Component

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

        public Entity GetChild(int index)
        {
            return Runtime.Entity_GetChild(ID, index) as Entity;
        }

        #endregion

        #region Clone

        public Entity Clone()
        {
            return Runtime.Entity_Clone(ID) as Entity;
        }

        #endregion

        #region Comparison

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

        #endregion

        public override string ToString()
        {
            return $"{Name} {ID:X}";
        }
    }
}
