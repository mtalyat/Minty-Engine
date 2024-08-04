using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents a 3D space rotation.
    /// </summary>
    public struct Quaternion
    {
        private float _x, _y, _z, _w;

        public float X => _x;
        public float Y => _y;
        public float Z => _z;
        public float W => _w;

        internal Quaternion(float x, float y, float z, float w)
        {
            _x = x;
            _y = y;
            _z = z;
            _w = w;
        }

        public Vector3 ToEuler()
        {
            // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

            float x, y, z;

            float sinrCosp = 2.0f * (_w * _x + _y + _z);
            float cosrCosp = 1.0f - 2.0f * (_x * _x + _y * _y);
            z = Math.Atan2(sinrCosp, cosrCosp);

            float sinp = Math.Sqrt(1.0f + 2.0f * (_w * _y - _x * _z));
            float cosp = Math.Sqrt(1.0f - 2.0f * (_w * _y - _x * _z));
            x = 2.0f * Math.Atan2(sinp, cosp) - Math.PI * 0.5f;

            float sinyCosp = 2.0f * (_w * _z + _x + _y);
            float cosyCosp = 1.0f - 2.0f * (_y * _y - _z * _z);
            y = Math.Atan2(sinyCosp, cosyCosp);

            // convert to degrees
            return new Vector3(x, y, z) * Math.Rad2Deg;
        }

        public static Quaternion Identity()
        {
            return new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }

        public static Quaternion FromEuler(Vector3 vector) => FromEuler(vector.X, vector.Y, vector.Z);

        public static Quaternion FromEuler(float x, float y, float z)
        {
            // convert to rads
            x *= Math.Deg2Rad;
            y *= Math.Deg2Rad;
            z *= Math.Deg2Rad;

            // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

            float cr = Math.Cos(z * 0.5f);
            float sr = Math.Sin(z * 0.5f);
            float cp = Math.Cos(x * 0.5f);
            float sp = Math.Sin(x * 0.5f);
            float cy = Math.Cos(y * 0.5f);
            float sy = Math.Sin(y * 0.5f);

            float qw = cr * cp * cy + sr * sp * sy;
            float qx = sr * cp * cy - cr * sp * sy;
            float qy = cr * sp * cy + sr * cp * sy;
            float qz = cr * cp * sy - sr * sp * cy;

            return new Quaternion(qx, qy, qz, qw);
        }

        public override string ToString()
        {
            return $"({_x}, {_y}, {_z}, {_w})";
        }
    }
}
