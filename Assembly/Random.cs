

namespace MintyEngine
{
    /// <summary>
    /// Random number generator.
    /// </summary>
    public static class Random
    {
        private static readonly System.Random _random = new System.Random();

        /// <summary>
        /// Generates a random int within the range of [0, int.MaxValue).
        /// </summary>
        /// <returns></returns>
        public static int RandomInt()
        {
            return _random.Next();
        }

        /// <summary>
        /// Generates a random int within the range of [0, max).
        /// </summary>
        /// <param name="max">The exclusive upper bound of the range.</param>
        /// <returns></returns>
        public static int RandomInt(int max)
        {
            return _random.Next(max);
        }

        /// <summary>
        /// Generates a random int within the range of [min, max).
        /// </summary>
        /// <param name="min">The inclusive lower bound of the range.</param>
        /// <param name="max">The exclusive upper bound of the range.</param>
        /// <returns></returns>
        public static int RandomInt(int min, int max)
        {
            return _random.Next(min, max);
        }

        /// <summary>
        /// Generates a random byte within the range of [0, byte.MaxValue).
        /// </summary>
        /// <returns></returns>
        public static byte RandomByte()
        {
            return (byte)_random.Next(byte.MaxValue);
        }

        /// <summary>
        /// Generates a random float within the range of [0.0, 1.0).
        /// </summary>
        /// <returns></returns>
        public static float RandomFloat()
        {
            return (float)_random.NextDouble();
        }

        /// <summary>
        /// Generates a random float within the range of [0.0, max).
        /// </summary>
        /// <param name="max"></param>
        /// <returns></returns>
        public static float RandomFloat(float max)
        {
            return RandomFloat() * max;
        }

        /// <summary>
        /// Generates a random float within the range of [min, max).
        /// </summary>
        /// <param name="min"></param>
        /// <param name="max"></param>
        /// <returns></returns>
        public static float RandomFloat(float min, float max)
        {
            return RandomFloat() * (max - min) + min;
        }
    }
}
