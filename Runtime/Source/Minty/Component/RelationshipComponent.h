#pragma once
#include "Minty/Component/Component.h"

#include "Minty/Core/Type.h"
#include "Minty/Entity/Entity.h"
#include "Minty/Entity/EntityRegistry.h"
#include "Minty/Entity/EntitySerializationData.h"

namespace Minty
{
	/// <summary>
	/// Holds relationship data for an Entity.
	/// </summary>
	struct RelationshipComponent
		: public Component
	{
		/// <summary>
		/// The index of this Entity among its siblings.
		/// </summary>
		UInt index;

		/// <summary>
		/// The number of children this Entity has.
		/// </summary>
		UInt children = 0;

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

		void deserialize(Reader& reader) override;
	};
}
