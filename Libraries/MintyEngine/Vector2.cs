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

        public static Vector2 Zero => new Vector2(0.0f, 0.0f);
        public static Vector2 One => new Vector2(1.0f, 1.0f);
        public static Vector2 Right => new Vector2(1.0f, 0.0f);
        public static Vector2 Left => new Vector2(-1.0f, 0.0f);
        public static Vector2 Down => new Vector2(0.0f, 1.0f);
        public static Vector2 Up => new Vector2(0.0f, -1.0f);

        public Vector2(float xy)
        {
            _x = xy;
            _y = xy;
        }

        public Vector2(float x, float y)
        {
            _x = x;
            _y = y;
        }

        public static Vector2 operator +(Vector2 left, Vector2 right)
        {
            return new Vector2(left._x + right._x, left._y + right._y);
        }

        public static Vector2 operator -(Vector2 left, Vector2 right)
        {
            return new Vector2(left._x - right._x, left._y - right._y);
        }

        public static Vector2 operator *(Vector2 left, Vector2 right)
        {
            return new Vector2(left._x * right._x, left._y * right._y);
        }

        public static Vector2 operator *(Vector2 left, float scalar)
        {
            return new Vector2(left._x * scalar, left._y * scalar);
        }

        public static Vector2 operator *(float scalar, Vector2 right)
        {
            return new Vector2(scalar * right._x, scalar * right._y);
        }

        public static Vector2 operator /(Vector2 left, Vector2 right)
        {
            return new Vector2(left._x / right._x, left._y / right._y);
        }

        public static Vector2 operator /(Vector2 left, float scalar)
        {
            return new Vector2(left._x / scalar, left._y / scalar);
        }

        public override string ToString()
        {
            return $"({_x}, {_y})";
        }
    }
}
