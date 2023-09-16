#pragma once

#include "M_Object.h"
#include <iostream>

namespace minty
{
    struct Vector2;
    struct Vector3Int;

    /// <summary>
    /// Holds X and Y coordinates for a point in space.
    /// </summary>
    struct Vector2Int :
        public Object
    {
        /// <summary>
        /// The X position.
        /// </summary>
        int x;

        /// <summary>
        /// The Y position.
        /// </summary>
        int y;

        /// <summary>
        /// Creates a new Vector2Int at (0, 0).
        /// </summary>
        Vector2Int();

        /// <summary>
        /// Creates a new Vector2Int using the given X and Y values.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        Vector2Int(int const x, int const y);

        /// <summary>
        /// Creates a Vector2Int using the given X and Y values, floored.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        Vector2Int(float const x, float const y);

        // copy constructor
        Vector2Int(Vector2Int const& other);

        // equals operator
        bool operator ==(Vector2Int const& other) const
        {
            return x == other.x && y == other.y;
        }

        // less than operator
        bool operator < (Vector2Int const& other) const
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
        Vector2Int operator +(Vector2Int const& other) const
        {
            return Vector2Int(x + other.x, y + other.y);
        }

        // subtract operator
        Vector2Int operator -(Vector2Int const& other) const
        {
            return Vector2Int(x - other.x, y - other.y);
        }

        // multiplication operator
        Vector2Int operator *(Vector2Int const& other) const
        {
            return Vector2Int(x * other.x, y * other.y);
        }

        // multiplication scaling operator
        Vector2Int operator *(int const scale) const
        {
            return Vector2Int(x * scale, y * scale);
        }

        // division operator
        Vector2Int operator /(Vector2Int const& other) const
        {
            return Vector2Int(x / other.x, y / other.y);
        }

        // division scaling operator
        Vector2Int operator /(int const scale) const
        {
            return Vector2Int(x / scale, y / scale);
        }

        operator Vector2() const;

        operator Vector3Int() const;

        std::string const toString() const override;

        /// <summary>
        /// Converts the given polar coordinates to cartesian coordinates.
        /// </summary>
        /// <param name="polar">The polar coordinates.</param>
        /// <returns>The cartesian coordinates.</returns>
        static Vector2Int polarToCartesian(Vector2Int const& polar);

        /// <summary>
        /// Converts the given cartesian coordinates to polar coordinates.
        /// </summary>
        /// <param name="cartesian">The cartesian coordinates.</param>
        /// <returns>The polar coordinates.</returns>
        static Vector2Int cartesianToPolar(Vector2Int const& cartesian);
    };
}

namespace std
{
    template<>
    struct hash<minty::Vector2Int>
    {
        std::size_t operator()(const minty::Vector2Int& obj) const
        {
            return hash<int>()(obj.x) ^ (hash<int>()(obj.y) << 1);
        }
    };
}