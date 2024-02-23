using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public class Quaternion
    {
        private float _x, _y, _z, _w;

        public Quaternion()
        {
            // identity Quaternion
            _x = 0.0f;
            _y = 0.0f;
            _z = 0.0f;
            _w = 1.0f;
        }
    }
}
