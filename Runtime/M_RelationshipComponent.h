#pragma once

#include "M_Component.h"
#include "M_Entity.h"

namespace minty
{
	/// <summary>
	/// Holds data for a relationship hiearchy between Entities.
	/// </summary>
	struct RelationshipComponent
		: public Component
	{
		/// <summary>
		/// The index of this Entity among its siblings.
		/// </summary>
		uint32_t index;

		/// <summary>
		/// The number of children this Entity has.
		/// </summary>
		uint32_t children = 0;

		/// <summary>
		/// The first child this Entity has.
		/// </summary>
		Entity first = NULL_ENTITY;
		
		/// <summary>
		/// The last child this Entity has.
		/// </summary>
		Entity last = NULL_ENTITY;

		/// <summary>
		/// The previous sibling this Entity has.
		/// </summary>
		Entity prev = NULL_ENTITY;

		/// <summary>
		/// The next sibling this Entity has.
		/// </summary>
		Entity next = NULL_ENTITY;

		/// <summary>
		/// The parent this Entity is a child to.
		/// </summary>
		Entity parent = NULL_ENTITY;

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend String to_string(RelationshipComponent const& value);
	};
}