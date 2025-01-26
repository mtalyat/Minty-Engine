using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    public class Asset : GameObject
    {
        internal Asset(UUID id)
            : base(id)
        {}
    }
}
