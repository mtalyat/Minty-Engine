using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents a 2D vector/point in space.
    /// </summary>
    public struct Vector2
    {
        private float _x, _y;

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

        public Vector2(float x, float y)
        {
            _x = x;
            _y = y;
        }

        public override string ToString()
        {
            return $"({_x}, {_y})";
        }
    }
}
