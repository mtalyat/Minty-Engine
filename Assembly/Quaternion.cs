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
        private readonly float _x, _y, _z, _w;

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

            float roll, pitch, yaw;

            float sinrCosp = 2.0f * (_w * _x + _y * _z);
            float cosrCosp = 1.0f - 2.0f * (_x * _x + _y * _y);
            pitch = (float)Math.Atan2(sinrCosp, cosrCosp);

            float sinp = 2.0f * (_w * _y - _z * _x);
            yaw = (float)(Math.Abs(sinp) >= 1.0f ? Math.CopySign(Math.PI * 0.5f, sinp) : Math.Asin(sinp));

            float sinyCosp = 2.0f * (_w * _z + _x * _y);
            float cosyCosp = 1.0f - 2.0f * (_y * _y + _z * _z);
            roll = (float)Math.Atan2(sinyCosp, cosyCosp);

            // Convert to degrees
            return new Vector3(pitch * Math.Rad2Deg, yaw * Math.Rad2Deg, roll * Math.Rad2Deg);
        }

        public static Quaternion Identity()
        {
            return new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }

        public static Quaternion FromEuler(Vector3 vector) => FromEuler(vector.X, vector.Y, vector.Z);

        public static Quaternion FromEuler(float x, float y, float z)
        {
            // convert to rads
            float roll = x * Math.Deg2Rad;
            float pitch = y * Math.Deg2Rad;
            float yaw = z * Math.Deg2Rad;

            // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

            float cp = (float)Math.Cos(pitch * 0.5f);
            float sp = (float)Math.Sin(pitch * 0.5f);
            float cy = (float)Math.Cos(yaw * 0.5f);
            float sy = (float)Math.Sin(yaw * 0.5f);
            float cr = (float)Math.Cos(roll * 0.5f);
            float sr = (float)Math.Sin(roll * 0.5f);

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
