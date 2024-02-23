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

        public static Vector4 Zero => new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        public static Vector4 One => new Vector4(1.0f, 1.0f, 1.0f, 1.0f);

        public Vector4(float xyzw)
        {
            _x = xyzw;
            _y = xyzw;
            _z = xyzw;
            _w = xyzw;
        }

        public Vector4(float x, float y, float z = 0.0f, float w = 0.0f)
        {
            _x = x;
            _y = y;
            _z = z;
            _w = w;
        }

        public static Vector4 operator +(Vector4 left, Vector4 right)
        {
            return new Vector4(left._x + right._x, left._y + right._y, left._z + right._z, left._w + right._w);
        }

        public static Vector4 operator -(Vector4 left, Vector4 right)
        {
            return new Vector4(left._x - right._x, left._y - right._y, left._z - right._z, left._w - right._w);
        }

        public static Vector4 operator *(Vector4 left, Vector4 right)
        {
            return new Vector4(left._x * right._x, left._y * right._y, left._z * right._z, left._w * right._w);
        }

        public static Vector4 operator *(Vector4 left, float scalar)
        {
            return new Vector4(left._x * scalar, left._y * scalar, left._z * scalar, left._w * scalar);
        }

        public static Vector4 operator /(Vector4 left, Vector4 right)
        {
            return new Vector4(left._x / right._x, left._y / right._y, left._z / right._z, left._w / right._w);
        }

        public static Vector4 operator /(Vector4 left, float scalar)
        {
            return new Vector4(left._x / scalar, left._y / scalar, left._z / scalar, left._w / scalar);
        }

        public override string ToString()
        {
            return $"({_x}, {_y}, {_z}, {_w})";
        }
    }
}
