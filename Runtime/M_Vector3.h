#pragma once
#include "M_Object.h"

namespace minty
{
    struct Vector3Int;
    struct Vector2;

    /// <summary>
    /// Holds X, Y and Z coordinates for a floating point pair in space.
    /// </summary>
    struct Vector3 :
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
        /// The Z position.
        /// </summary>
        float z;

        /// <summary>
        /// Creates a new floating Vector3Int at (0, 0).
        /// </summary>
        Vector3();

        /// <summary>
        /// Creates a new floating Vector3Int at the given X, Y and Z.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="z"></param>
        Vector3(float const x, float const y, float const z = 0.0f);

        /// <summary>
        /// Creates a new floating Vector3Int at the given X, Y and Z.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="z"></param>
        Vector3(int const x, int const y, int const z = 0);

        Vector3(Vector3 const& other);

        // equals operator
        bool operator ==(Vector3 const& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        // less than operator
        bool operator < (Vector3 const& other) const
        {
            if (y == other.y)
            {
                if (x == other.x)
                {
                    return z < other.z;
                }
                else
                {
                    return x < other.x;
                }
            }
            else {
                return y < other.y;
            }
        }

        // add operator
        Vector3 operator +(Vector3 const& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        // subtract operator
        Vector3 operator -(Vector3 const& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        // multiplication operator
        Vector3 operator *(Vector3 const& other) const
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        // multiplication scaling operator
        Vector3 operator *(float const scale) const
        {
            return Vector3(x * scale, y * scale, z * scale);
        }

        // division operator
        Vector3 operator /(Vector3 const& other) const
        {
            return Vector3(x / other.x, y / other.y, z / other.z);
        }

        // division scaling operator
        Vector3 operator /(float const scale) const
        {
            return Vector3(x / scale, y / scale, z / scale);
        }

        operator Vector3Int() const;

        operator Vector2() const;

        std::string const toString() const override;
    };
}

