using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
        internal readonly ulong ID;

        protected GameObject()
        {
            ID = 0;
        }

        internal GameObject(ulong id)
        {
            ID = id;
        }
    }
}
