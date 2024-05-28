#pragma once
#include "Minty/Types/M_Types.h"
#include <cstdint>

namespace Minty
{
	constexpr static uint64_t INVALID_UUID = 0;

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
		UUID() = default;

		/// <summary>
		/// Creates a new UUID with the given value.
		/// </summary>
		/// <param name="uuid"></param>
		UUID(uint64_t const uuid)
			: _uuid(uuid) {}

		bool valid() const
		{
			return _uuid > 0;
		}

		uint64_t data() const
		{
			return _uuid;
		}

		operator uint64_t() const { return _uuid; }
		bool operator==(UUID const other) const;
		bool operator!=(UUID const other) const;
		bool operator==(uint64_t const other) const;
		bool operator!=(uint64_t const other) const;
		bool operator!() const;

		friend std::ostream& operator<<(std::ostream& stream, UUID const& object);
		friend std::istream& operator>>(std::istream& stream, UUID& object);

		static UUID create();

	public:
		friend String to_string(UUID const value);
	};
}

namespace std
{
	template<>
	struct hash<Minty::UUID>
	{
		std::size_t operator()(Minty::UUID const& uuid) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};
}