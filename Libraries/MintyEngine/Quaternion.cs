using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public struct Quaternion
    {
        private float _x, _y, _z, _w;

        private Quaternion(float x, float y, float z, float w)
        {
            _x = x;
            _y = y;
            _z = z;
            _w = w;
        }

        public static Quaternion Identity()
        {
            return new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }
}
