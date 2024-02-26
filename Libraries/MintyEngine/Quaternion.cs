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

        private Quaternion(float x, float y, float z, float w)
        {
            _x = x;
            _y = y;
            _z = z;
            _w = w;
        }

        public Vector3 ToEuler()
        {
            float x, y, z;

            // Roll (Z-axis rotation)
            float sinr_cosp = 2 * (_w * _z + _x * _y);
            float cosr_cosp = 1 - 2 * (_y * _y + _z * _z);
            z = Math.Atan2(sinr_cosp, cosr_cosp);

            // Pitch (X-axis rotation)
            float sinp = 2 * (_w * _x - _z * _y);
            if (Math.Abs(sinp) >= 1)
                x = (Math.PI / 2.0f * Math.Sign(sinp)); // use 90 degrees if out of range
            else
                x = Math.Asin(sinp);

            // Yaw (Y-axis rotation)
            float siny_cosp = 2 * (_w * _y + _z * _x);
            float cosy_cosp = 1 - 2 * (_x * _x + _y * _y);
            y = Math.Atan2(siny_cosp, cosy_cosp);

            return new Vector3(x, y, z);
        }

        public static Quaternion Identity()
        {
            return new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
        }

        public static Quaternion FromEuler(Vector3 vector) => FromEuler(vector.X, vector.Y, vector.Z);

        public static Quaternion FromEuler(float x, float y, float z)
        {
            // convert euler angles to a quaternion and return that
            float cy = Math.Cos(y * 0.5f);
            float sy = Math.Sin(y * 0.5f);
            float cp = Math.Cos(x * 0.5f);
            float sp = Math.Sin(x * 0.5f);
            float cr = Math.Cos(z * 0.5f);
            float sr = Math.Sin(z * 0.5f);

            return new Quaternion(
                sr * cp * cy - cr * sp * sy,
                cr * sp * cy + sr * cp * sy,
                cr * cp * sy - sr * sp * cy,
                cr * cp * cy + sr * sp * sy);
        }
    }
}
