using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents a 3D vector/point in space.
    /// </summary>
    public struct Vector3
    {
        private float _x, _y, _z;

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

        public static Vector3 Zero => new Vector3(0.0f, 0.0f, 0.0f);
        public static Vector3 One => new Vector3(1.0f, 1.0f, 1.0f);
        public static Vector3 Right => new Vector3(1.0f, 0.0f, 0.0f);
        public static Vector3 Left => new Vector3(-1.0f, 0.0f, 0.0f);
        public static Vector3 Down => new Vector3(0.0f, 1.0f, 0.0f);
        public static Vector3 Up => new Vector3(0.0f, -1.0f, 0.0f);
        public static Vector3 Forward => new Vector3(0.0f, 0.0f, 1.0f);
        public static Vector3 Backward => new Vector3(0.0f, 0.0f, -1.0f);

        public Vector3(float xyz)
        {
            _x = xyz;
            _y = xyz;
            _z = xyz;
        }

        public Vector3(float x, float y, float z = 0.0f)
        {
            _x = x;
            _y = y;
            _z = z;
        }

        public static Vector3 operator+(Vector3 left, Vector3 right)
        {
            return new Vector3(left._x + right._x, left._y + right._y, left._z + right._z);
        }

        public static Vector3 operator-(Vector3 left, Vector3 right)
        {
            return new Vector3(left._x - right._x, left._y - right._y, left._z - right._z);
        }

        public static Vector3 operator *(Vector3 left, Vector3 right)
        {
            return new Vector3(left._x * right._x, left._y * right._y, left._z * right._z);
        }

        public static Vector3 operator *(Vector3 left, float scalar)
        {
            return new Vector3(left._x * scalar, left._y * scalar, left._z * scalar);
        }

        public static Vector3 operator *(float scalar, Vector3 right)
        {
            return new Vector3(scalar * right._x, scalar * right._y, scalar * right._z);
        }

        public static Vector3 operator /(Vector3 left, Vector3 right)
        {
            return new Vector3(left._x / right._x, left._y / right._y, left._z / right._z);
        }

        public static Vector3 operator /(Vector3 left, float scalar)
        {
            return new Vector3(left._x / scalar, left._y / scalar, left._z / scalar);
        }

        public override string ToString()
        {
            return $"({_x}, {_y}, {_z})";
        }
    }
}
