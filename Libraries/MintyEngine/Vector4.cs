using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents a 4D vector/point in space.
    /// </summary>
    public struct Vector4
    {
        private float _x, _y, _z, _w;

        public float X
        {
            get => _x;
            set => _x = value;
        }

        public float Y
        {
            get => _y;
            set => _y = value;
        }

        public float Z
        {
            get => _z;
            set => _z = value;
        }

        public float W
        {
            get => _w;
            set => _w = value;
        }

        public Vector4(float x, float y, float z = 0.0f, float w = 0.0f)
        {
            _x = x;
            _y = y;
            _z = z;
            _w = w;
        }

        public override string ToString()
        {
            return $"({_x}, {_y}, {_z}, {_w})";
        }
    }
}
