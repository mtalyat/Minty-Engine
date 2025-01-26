#pragma once
#include "Minty/Core/Base.h"

#include "Minty/Core/Type.h"

#include <cstdint>

namespace Minty
{
	using UUID_t = Size;

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
		UUID_t m_uuid;

	public:
		/// <summary>
		/// Creates a new UUID with a random value.
		/// </summary>
		UUID() = default;

		/// <summary>
		/// Creates a new UUID with the given value.
		/// </summary>
		/// <param name="uuid"></param>
		UUID(UUID_t const uuid)
			: m_uuid(uuid) {}

		Bool valid() const
		{
			return m_uuid > 0;
		}

		Size data() const
		{
			return m_uuid;
		}

		operator UUID_t() const { return m_uuid; }
		Bool operator==(UUID const other) const;
		Bool operator!=(UUID const other) const;
		Bool operator==(UUID_t const other) const;
		Bool operator!=(UUID_t const other) const;

		friend std::ostream& operator<<(std::ostream& stream, UUID const& object);
		friend std::istream& operator>>(std::istream& stream, UUID& object);

		static UUID create();
	};
}

namespace std
{
	template<>
	struct hash<Minty::UUID>
	{
		Minty::Size operator()(Minty::UUID const& uuid) const
		{
			return hash<Minty::Size>()(static_cast<Minty::Size>(uuid));
		}
	};
}