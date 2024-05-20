#pragma once

#include "types/M_Object.h"
#include "types/M_Vector.h"

namespace minty
{
	struct Rect;

	/// <summary>
	/// Holds an X and Y coordinate, as well as a width and height for a rectangle in space.
	/// </summary>
	struct RectF
		: public Object
	{
	public:
		/// <summary>
		/// The column position of the Rectangle.
		/// </summary>
		float x;
		/// <summary>
		/// The row position of the Rectangle.
		/// </summary>
		float y;
		/// <summary>
		/// The width of the Rectangle.
		/// </summary>
		float width;
		/// <summary>
		/// The height of the Rectangle.
		/// </summary>
		float height;

	public:
		/// <summary>
		/// Creates an empty Rectangle.
		/// </summary>
		RectF();

		/// <summary>
		/// Creates a new Rectangle using the given width and height.
		/// </summary>
		/// <param name="width">The width of the Rectangle.</param>
		/// <param name="height">The height of the Rectangle.</param>
		RectF(float const width, float const height);

		/// <summary>
		/// Creates a new Rectangle using the given Vector2Int size.
		/// </summary>
		/// <param name="size">The size of the Rectangle.</param>
		RectF(Vector2 const size);

		/// <summary>
		/// Creates a new Rectangle using the given X and Y position, width and height.
		/// </summary>
		/// <param name="x">The column of the Rectangle.</param>
		/// <param name="y">The row of the Rectangle.</param>
		/// <param name="width">The width of the Rectangle.</param>
		/// <param name="height">The height of the Rectangle.</param>
		RectF(float const x, float const y, float const width, float const height);

		/// <summary>
		/// Creates a new Rectangle using the given X and Y position, width and height.
		/// </summary>
		/// <param name="x">The column of the Rectangle.</param>
		/// <param name="y">The row of the Rectangle.</param>
		/// <param name="width">The width of the Rectangle.</param>
		/// <param name="height">The height of the Rectangle.</param>
		RectF(int const x, int const y, int const width, int const height);

		/// <summary>
		/// Creates a new Rectangle using the given position and size.
		/// </summary>
		/// <param name="position">The position of the Rectangle.</param>
		/// <param name="size">The size of the Rectangle.</param>
		RectF(Vector2 const position, Vector2 const size);

		/// <summary>
		/// Creates a new Rectangle using the given position and size.
		/// </summary>
		/// <param name="position">The position of the Rectangle.</param>
		/// <param name="size">The size of the Rectangle.</param>
		RectF(Vector2Int const position, Vector2Int const size);

		// equals operator
		bool operator ==(RectF const& other) const
		{
			return x == other.x && y == other.y && width == other.width && height == other.height;
		}

		// not equals operator
		bool operator !=(RectF const& other) const
		{
			return x != other.x || y != other.y || width != other.width || height != other.height;
		}

		// add operator
		RectF operator +(Vector2 const& point) const
		{
			return RectF(x + point.x, y + point.y, width, height);
		}

		// subtract operator
		RectF operator -(Vector2 const& point) const
		{
			return RectF(x - point.x, y - point.y, width, height);
		}

		/// <summary>
		/// Gets the leftmost column of the Rectangle.
		/// </summary>
		/// <returns>The minimum X position.</returns>
		constexpr float left() const { return x; }
		/// <summary>
		/// Gets the rightmost column of the Rectangle.
		/// </summary>
		/// <returns>The maximum X position.</returns>
		constexpr float right() const { return x + width; }
		/// <summary>
		/// Gets the topmost row of the Rectangle.
		/// </summary>
		/// <returns>The minimum Y position.</returns>
		constexpr float top() const { return y; }
		/// <summary>
		/// Gets the bottommost row of the Rectangle.
		/// </summary>
		/// <returns>The maximum Y position.</returns>
		constexpr float bottom() const { return y + height; }

		/// <summary>
		/// Gets the area of the Rectangle.
		/// </summary>
		/// <returns>The width * height.</returns>
		constexpr float area() const { return width * height; }

		/// <summary>
		/// Gets the position of the Rectangle, as a Vector2Int.
		/// </summary>
		/// <returns>The X and Y position.</returns>
		inline Vector2 position() const { return Vector2(x, y); }
		/// <summary>
		/// Gets the size of the Rectangle, as a Vector2Int.
		/// </summary>
		/// <returns>The size.</returns>
		inline Vector2 size() const { return Vector2(width, height); }
		/// <summary>
		/// Gets the center position of the Rectangle, as a Vector2Int.
		/// </summary>
		/// <returns>The center position.</returns>
		inline Vector2 center() const { return Vector2(x + width / 2.0f, y + height / 2.0f); }

		/// <summary>
		/// Checks if this RectF overlaps with the given RectF.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool overlaps(RectF const& other) const;

		/// <summary>
		/// Returns the RectF that contains the area that this RectF and the given RectF overlap in.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		RectF overlap(RectF const& other) const;

		/// <summary>
		/// Checks if this RectF contains the given Vector2Int.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool contains(Vector2 const& other) const;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		/// <summary>
		/// Creates a Rect using the given bounds of each size of the Rect.
		/// </summary>
		/// <param name="left">The minimum X.</param>
		/// <param name="right">The maximum X.</param>
		/// <param name="top">The minimum Y.</param>
		/// <param name="bottom">The maximum Y.</param>
		/// <returns></returns>
		static RectF bounds(float const left, float const right, float const top, float const bottom);

		/// <summary>
		/// Floors the given RectF.
		/// </summary>
		/// <param name="rect">The RectF to floor.</param>
		/// <returns>A RectF, with the given values, floored.</returns>
		static Rect floor(RectF const& rect);
		/// <summary>
		/// Rounds the given RectF.
		/// </summary>
		/// <param name="rect">The RectF to round.</param>
		/// <returns>A RectF, with the given values, rounded.</returns>
		static Rect round(RectF const& rect);
		/// <summary>
		/// Ceil the given RectF.
		/// </summary>
		/// <param name="rect">The RectF to ceil.</param>
		/// <returns>A RectF, with the given values, ceiled.</returns>
		static Rect ceil(RectF const& rect);
	public:
		friend String to_string(RectF const& value);
	};
}