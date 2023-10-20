#pragma once

#include "M_Component.h"
#include "M_EntityRegistry.h"
#include <unordered_set>

namespace minty
{
	struct RelationshipComponent
		: public Component
	{
		// child count
		size_t children;

		// first child entity
		Entity first;

		// previous sibling
		Entity prev;

		// next sibling
		Entity next;

		// parent
		Entity parent;

		//RelationshipComponent(size_t const children = 0, Entity const first = NULL_ENTITY, Entity const prev = NULL_ENTITY, Entity const next = NULL_ENTITY, Entity const parent = NULL_ENTITY);

		void serialize(Writer& writer) const override;
		void deserialize(Reader const& reader) override;

		friend std::string to_string(RelationshipComponent const& value);
	};
}