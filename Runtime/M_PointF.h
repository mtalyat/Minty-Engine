#pragma once

#include "M_Object.h"

namespace minty
{
    struct Point;

    /// <summary>
    /// Holds X and Y coordinates for a floating point pair in space.
    /// </summary>
    struct PointF :
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
        /// Creates a new floating Point at (0, 0).
        /// </summary>
        PointF();

        /// <summary>
        /// Creates a new floating Point at the given X and Y.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        PointF(float const x, float const y);

        /// <summary>
        /// Creates a new floating Point at the given X and Y.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        PointF(int const x, int const y);

        PointF(PointF const& other);

        // equals operator
        bool operator ==(PointF const& other) const
        {
            return x == other.x && y == other.y;
        }

        // less than operator
        bool operator < (PointF const& other) const
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
        PointF operator +(PointF const& other) const
        {
            return PointF(x + other.x, y + other.y);
        }

        // subtract operator
        PointF operator -(PointF const& other) const
        {
            return PointF(x - other.x, y - other.y);
        }

        // multiplication operator
        PointF operator *(PointF const& other) const
        {
            return PointF(x * other.x, y * other.y);
        }

        // multiplication scaling operator
        PointF operator *(float const scale) const
        {
            return PointF(x * scale, y * scale);
        }

        // division operator
        PointF operator /(PointF const& other) const
        {
            return PointF(x / other.x, y / other.y);
        }

        // division scaling operator
        PointF operator /(float const scale) const
        {
            return PointF(x / scale, y / scale);
        }

        /// <summary>
        /// Converts this PointF to a Point.
        /// </summary>
        /// <returns>A Point containing this PointF's values, rounded.</returns>
        Point toPoint() const;

        std::string const toString() const override;

        /// <summary>
        /// Converts the given polar coordinates to cartesian coordinates.
        /// </summary>
        /// <param name="polar">The polar coordinates.</param>
        /// <returns>The cartesian coordinates.</returns>
        static PointF polarToCartesian(PointF const& polar);

        /// <summary>
        /// Converts the given cartesian coordinates to polar coordinates.
        /// </summary>
        /// <param name="cartesian">The cartesian coordinates.</param>
        /// <returns>The polar coordinates.</returns>
        static PointF cartesianToPolar(PointF const& cartesian);
    };
}

namespace std
{
    template<>
    struct hash<minty::PointF>
    {
        std::size_t operator()(const minty::PointF& obj) const
        {
            return hash<float>()(obj.x) ^ (hash<float>()(obj.y) << 1);
        }
    };
}