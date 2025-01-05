using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public class Asset
    {
        private ulong _id;

        public ulong ID => _id;

        internal Asset(ulong id)
        {
            _id = id;
        }
    }
}
