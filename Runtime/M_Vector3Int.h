#pragma once
#include "M_Object.h"

namespace minty
{
	struct Vector3;
    struct Vector2Int;

    /// <summary>
    /// Holds X, Y and Z coordinates for a point in space.
    /// </summary>
    struct Vector3Int :
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
        /// The Z position.
        /// </summary>
        int z;

        /// <summary>
        /// Creates a new Vector3Int at (0, 0).
        /// </summary>
        Vector3Int();

        /// <summary>
        /// Creates a new Vector3Int using the given X, Y and Z values.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        Vector3Int(int const x, int const y, int const z = 0);

        /// <summary>
        /// Creates a Vector3Int using the given X, Y and Z values, floored.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        Vector3Int(float const x, float const y, float const z = 0.0f);

        // copy constructor
        Vector3Int(Vector3Int const& other);

        // equals operator
        bool operator ==(Vector3Int const& other) const
        {
            return x == other.x && y == other.y;
        }

        // less than operator
        bool operator < (Vector3Int const& other) const
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
        Vector3Int operator +(Vector3Int const& other) const
        {
            return Vector3Int(x + other.x, y + other.y);
        }

        // subtract operator
        Vector3Int operator -(Vector3Int const& other) const
        {
            return Vector3Int(x - other.x, y - other.y);
        }

        // multiplication operator
        Vector3Int operator *(Vector3Int const& other) const
        {
            return Vector3Int(x * other.x, y * other.y);
        }

        // multiplication scaling operator
        Vector3Int operator *(int const scale) const
        {
            return Vector3Int(x * scale, y * scale);
        }

        // division operator
        Vector3Int operator /(Vector3Int const& other) const
        {
            return Vector3Int(x / other.x, y / other.y);
        }

        // division scaling operator
        Vector3Int operator /(int const scale) const
        {
            return Vector3Int(x / scale, y / scale);
        }

        operator Vector3() const;

        operator Vector2Int() const;

        std::string const toString() const override;
    };
}

