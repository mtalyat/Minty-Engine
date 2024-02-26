using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SystemMath = System.Math;

namespace MintyEngine
{
    public static class Math
    {
        public const float PI = (float)SystemMath.PI;

        public const float Deg2Rad = PI / 180.0f;
        public const float Rad2Deg = 180.0f / PI;

        public static float Abs(float value) => (float)SystemMath.Abs(value);

        public static float Min(float value, float value2)
        {
            return value < value2 ? value : value2;
        }

        public static float Max(float value, float value2)
        {
            return value > value2 ? value : value2;
        }

        public static float Clamp(float value, float min, float max)
        {
            return SystemMath.Min(SystemMath.Max(value, min), max);
        }

        public static int Sign(float value)
        {
            return value < 0 ? -1 : (value > 0 ? 1 : 0);
        }

        public static float Sin(float radians) => (float)SystemMath.Sin(radians);

        public static float Asin(float radians) => (float)SystemMath.Asin(radians);

        public static float Cos(float radians) => (float)SystemMath.Cos(radians);

        public static float Acos(float radians) => (float)SystemMath.Acos(radians);

        public static float Tan(float radians) => (float)SystemMath.Tan(radians);

        public static float Atan(float radians) => (float)SystemMath.Atan(radians);

        public static float Atan2(float y, float x) => (float)SystemMath.Atan2(y, x);
    }
}
