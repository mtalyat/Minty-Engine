#pragma once

#include "Minty/Types/M_Object.h"
#include "Minty/Types/M_Vector.h"

namespace Minty
{
	struct RectF;

	/// <summary>
	/// Holds an X and Y coordinate, as well as a width and height for a rectangle in space.
	/// </summary>
	struct Rect
		: public Object
	{
	public:
		/// <summary>
		/// The column position of the Rectangle.
		/// </summary>
		int x;
		/// <summary>
		/// The row position of the Rectangle.
		/// </summary>
		int y;
		/// <summary>
		/// The width of the Rectangle.
		/// </summary>
		int width;
		/// <summary>
		/// The height of the Rectangle.
		/// </summary>
		int height;

	public:
		/// <summary>
		/// Creates an empty Rectangle.
		/// </summary>
		Rect() = default;

		/// <summary>
		/// Creates a new Rectangle using the given width and height.
		/// </summary>
		/// <param name="width">The width of the Rectangle.</param>
		/// <param name="height">The height of the Rectangle.</param>
		Rect(int const width, int const height)
			: x(0)
			, y(0)
			, width(width)
			, height(height)
		{}

		/// <summary>
		/// Creates a new Rectangle using the given Vector2Int size.
		/// </summary>
		/// <param name="size">The size of the Rectangle.</param>
		Rect(Vector2Int const size)
			: x(0)
			, y(0)
			, width(size.x)
			, height(size.y)
		{}

		/// <summary>
		/// Creates a new Rectangle using the given X and Y position, width and height.
		/// </summary>
		/// <param name="x">The column of the Rectangle.</param>
		/// <param name="y">The row of the Rectangle.</param>
		/// <param name="width">The width of the Rectangle.</param>
		/// <param name="height">The height of the Rectangle.</param>
		Rect(int const x, int const y, int const width, int const height)
			: x(x)
			, y(y)
			, width(width)
			, height(height)
		{}

		/// <summary>
		/// Creates a new Rectangle using the given position and size.
		/// </summary>
		/// <param name="position">The position of the Rectangle.</param>
		/// <param name="size">The size of the Rectangle.</param>
		Rect(Vector2Int const position, Vector2Int const size)
			: x(position.x)
			, y(position.y)
			, width(size.x)
			, height(size.y)
		{}

		// equals operator
		bool operator ==(Rect const& other) const
		{
			return x == other.x && y == other.y && width == other.width && height == other.height;
		}

		// not equals operator
		bool operator !=(Rect const& other) const
		{
			return x != other.x || y != other.y || width != other.width || height != other.height;
		}

		// add operator
		Rect operator +(Vector2Int const& point) const
		{
			return Rect(x + point.x, y + point.y, width, height);
		}

		// subtract operator
		Rect operator -(Vector2Int const& point) const
		{
			return Rect(x - point.x, y - point.y, width, height);
		}

		/// <summary>
		/// Gets the leftmost column of the Rectangle.
		/// </summary>
		/// <returns>The minimum X position.</returns>
		constexpr int left() const { return x; }
		/// <summary>
		/// Gets the rightmost column of the Rectangle.
		/// </summary>
		/// <returns>The maximum X position.</returns>
		constexpr int right() const { return x + width; }
		/// <summary>
		/// Gets the topmost row of the Rectangle.
		/// </summary>
		/// <returns>The minimum Y position.</returns>
		constexpr int top() const { return y; }
		/// <summary>
		/// Gets the bottommost row of the Rectangle.
		/// </summary>
		/// <returns>The maximum Y position.</returns>
		constexpr int bottom() const { return y + height; }

		/// <summary>
		/// Gets the area of the Rectangle.
		/// </summary>
		/// <returns>The width * height.</returns>
		constexpr int area() const { return width * height; }

		/// <summary>
		/// Gets the position of the Rectangle, as a Vector2Int.
		/// </summary>
		/// <returns>The X and Y position.</returns>
		inline Vector2Int position() const { return Vector2Int(x, y); }
		/// <summary>
		/// Gets the size of the Rectangle, as a Vector2Int.
		/// </summary>
		/// <returns>The size.</returns>
		inline Vector2Int size() const { return Vector2Int(width, height); }
		/// <summary>
		/// Gets the center position of the Rectangle, as a Vector2Int.
		/// </summary>
		/// <returns>The center position.</returns>
		inline Vector2Int center() const { return Vector2Int(x + width / 2, y + height / 2); }

		/// <summary>
		/// Checks if this Rect overlaps with the given Rect.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool overlaps(Rect const& other) const;

		/// <summary>
		/// Returns the Rect that contains the area that this Rect and the given Rect overlap in.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		Rect overlap(Rect const& other) const;

		/// <summary>
		/// Checks if this Rect contains the given Vector2Int.
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		bool contains(Vector2Int const& other) const;

		/// <summary>
		/// Converts this Rect to a RectF.
		/// </summary>
		/// <returns>A RectF with this Rect's values.</returns>
		RectF to_RectF() const;

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
		static Rect bounds(int const left, int const right, int const top, int const bottom);

		/// <summary>
		/// Rounds the given RectF into a Rect.
		/// </summary>
		/// <param name="rect">The RectF to round.</param>
		/// <returns>A Rect, with the given rect's values, rounded.</returns>
		static Rect round(RectF const& rect);

	public:
		friend String to_string(Rect const& rect);
	};
}