using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    /// <summary>
    /// An Object with an internal game UUID.
    /// </summary>
    public abstract class GameObject : Object
    {
        /// <summary>
        /// The UUID of this Object.
        /// </summary>
        internal readonly UUID ID;

        internal GameObject(UUID id)
        {
            ID = id;
        }

        public override bool Equals(object obj)
        {
            GameObject other = obj as GameObject;

            if (other == null)
            {
                return false;
            }

            return other.ID == ID;
        }

        public override int GetHashCode()
        {
            return ID.GetHashCode();
        }
    }
}
