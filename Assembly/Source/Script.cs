using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    /// <summary>
    /// A Script is a custom component that can add extra functionality to an Entity.
    /// </summary>
    public class Script : Component
    {
        public Script()
            : base(0)
        { }
    }
}
