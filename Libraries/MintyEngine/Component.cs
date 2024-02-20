using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// A component is some sort of data that adds functionality to an Entity.
    /// </summary>
    public abstract class Component
    {
        public Entity Entity { get; internal set; } = null;
    }
}
