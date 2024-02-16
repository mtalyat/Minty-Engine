#pragma once
#include "M_Types.h"
#include <cstdint>

namespace minty
{
	/// <summary>
	/// A Universally Unique IDentifier.
	/// Generates a random number to use as a unique ID for each asset/entity.
	/// Each UUID has a 1 in 18,446,744,073,709,551,615 chance of colliding with another UUID.
	/// You are 6 billion times more likely to win the Powerball.
	/// 
	/// Thank you Cherno for this code.
	/// </summary>
	class UUID
	{
	private:
		uint64_t _uuid;

	public:
		/// <summary>
		/// Creates a new UUID with a random value.
		/// </summary>
		UUID();

		/// <summary>
		/// Creates a new UUID with the given value.
		/// </summary>
		/// <param name="uuid"></param>
		UUID(uint64_t const uuid);

		bool empty() const;

		operator uint64_t() const { return _uuid; }
		bool operator==(UUID const other) const;
		bool operator!=(UUID const other) const;

		friend std::ostream& operator<<(std::ostream& stream, UUID const& object);
		friend std::istream& operator>>(std::istream& stream, UUID& object);

		friend String to_string(UUID const value);

		static UUID create_empty();
	};
}

namespace std
{
	template<>
	struct hash<minty::UUID>
	{
		std::size_t operator()(minty::UUID const& uuid) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};
}