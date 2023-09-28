#pragma once

#include "M_Object.h"

namespace minty
{
    struct Vector2Int;
    struct Vector3;

    /// <summary>
    /// Holds X and Y coordinates for a floating point pair in space.
    /// </summary>
    struct Vector2 :
        public Object
    {
        /// <summary>
        /// The X position.
        /// </summary>
        float x;

        /// <summary>
        /// The Y position.
        /// </summary>
        float y;

        /// <summary>
        /// Creates a new floating Vector2Int at (0, 0).
        /// </summary>
        Vector2();

        /// <summary>
        /// Creates a new floating Vector2Int at the given X and Y.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        Vector2(float const x, float const y);

        /// <summary>
        /// Creates a new floating Vector2Int at the given X and Y.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        Vector2(int const x, int const y);

        Vector2(Vector2 const& other);

        // equals operator
        bool operator ==(Vector2 const& other) const
        {
            return x == other.x && y == other.y;
        }

        // less than operator
        bool operator < (Vector2 const& other) const
        {
            if (y == other.y)
            {
                return x < other.x;
            }
            else {
                return y < other.y;
            }
        }

        // add operator
        Vector2 operator +(Vector2 const& other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        // subtract operator
        Vector2 operator -(Vector2 const& other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        // multiplication operator
        Vector2 operator *(Vector2 const& other) const
        {
            return Vector2(x * other.x, y * other.y);
        }

        // multiplication scaling operator
        Vector2 operator *(float const scale) const
        {
            return Vector2(x * scale, y * scale);
        }

        // division operator
        Vector2 operator /(Vector2 const& other) const
        {
            return Vector2(x / other.x, y / other.y);
        }

        // division scaling operator
        Vector2 operator /(float const scale) const
        {
            return Vector2(x / scale, y / scale);
        }

        operator minty::Vector2Int() const;

        operator minty::Vector3() const;

        std::string const to_string() const override;

        /// <summary>
        /// Converts the given polar coordinates to cartesian coordinates.
        /// </summary>
        /// <param name="polar">The polar coordinates.</param>
        /// <returns>The cartesian coordinates.</returns>
        static Vector2 to_cartesian(Vector2 const& polar);

        /// <summary>
        /// Converts the given cartesian coordinates to polar coordinates.
        /// </summary>
        /// <param name="cartesian">The cartesian coordinates.</param>
        /// <returns>The polar coordinates.</returns>
        static Vector2 to_polar(Vector2 const& cartesian);
    };
}

namespace std
{
    template<>
    struct hash<minty::Vector2>
    {
        std::size_t operator()(const minty::Vector2& obj) const
        {
            return hash<float>()(obj.x) ^ (hash<float>()(obj.y) << 1);
        }
    };
}