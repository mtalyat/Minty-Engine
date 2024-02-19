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

        protected Entity()
        {
            ID = 0;
        }

        internal Entity(ulong id)
        {
            ID = id;
        }
    }
}
