using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents a Color.
    /// </summary>
    public struct Color
    {
        private const int COLOR_SHAMT_R = 0;
        private const int COLOR_SHAMT_G = 8;
        private const int COLOR_SHAMT_B = 16;
        private const int COLOR_SHAMT_A = 24;

        private byte _r;
        public byte R
        {
            get => _r; set => _r = value;
        }
        private byte _g;
        public byte G
        {
            get => _g; set => _g = value;
        }
        private byte _b;
        public byte B
        {
            get => _b; set => _b = value;
        }
        private byte _a;
        public byte A
        {
            get => _a; set => _a = value;
        }

        public static Color White => new Color(255, 255, 255, 255);
        public static Color Black => new Color(0, 0, 0, 255);
        public static Color Red => new Color(255, 0, 0, 255);
        public static Color Green => new Color(0, 255, 0, 255);
        public static Color Blue => new Color(0, 0, 255, 255);
        public static Color Yellow => new Color(255, 255, 0, 255);
        public static Color Cyan => new Color(0, 255, 255, 255);
        public static Color Magenta => new Color(255, 0, 255, 255);
        public static Color Random => new Color(MintyEngine.Random.RandomByte(), MintyEngine.Random.RandomByte(), MintyEngine.Random.RandomByte(), (byte)255);

        public Color(byte r, byte g, byte b, byte a = 255)
        {
            _r = r;
            _g = g;
            _b = b;
            _a = a;
        }

        public Color(int r, int g, int b, int a = 255)
        {
            _r = (byte)(r & 0xff);
            _g = (byte)(g & 0xff);
            _b = (byte)(b & 0xff);
            _a = (byte)(a & 0xff);
        }

        public int ToInt()
        {
            return (_r << COLOR_SHAMT_R) | (_g << COLOR_SHAMT_G) | (_b << COLOR_SHAMT_B) | (_a << COLOR_SHAMT_A);
        }

        public static Color FromInt(int color)
        {
            return new Color(
                (color >> COLOR_SHAMT_R) & 0xff,
                (color >> COLOR_SHAMT_G) & 0xff,
                (color >> COLOR_SHAMT_B) & 0xff,
                (color >> COLOR_SHAMT_A) & 0xff
                );
        }

        public override bool Equals(object obj)
        {
            return obj is Color color && _r == color._r && _g == color._g && _b == color._b && _a == color._a;
        }

        public override int GetHashCode()
        {
            return ToInt();
        }

        public static bool operator ==(Color color1, Color color2)
        {
            return color1.Equals(color2);
        }

        public static bool operator !=(Color color1, Color color2)
        {
            return !color1.Equals(color2);
        }

        public static Color Lerp(Color left, Color right, float t)
        {
            return new Color(
                Math.Lerp(left.R, right.R, t),
                Math.Lerp(left.G, right.G, t),
                Math.Lerp(left.B, right.B, t),
                Math.Lerp(left.A, right.A, t)
                );
        }

        public static Color Lerp(Color left, Color right, float t, int a)
        {
            return new Color(
                Math.Lerp(left.R, right.R, t),
                Math.Lerp(left.G, right.G, t),
                Math.Lerp(left.B, right.B, t),
                a
                );
        }
    }
}
