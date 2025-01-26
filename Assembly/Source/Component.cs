using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    /// <summary>
    /// A component is some sort of data that adds functionality to an Entity.
    /// </summary>
    public abstract class Component : GameObject
    {
        private Entity _entity = null;
        public Entity Entity => _entity;

        internal Component(UUID id)
            : base(id)
        { }
    }
}
