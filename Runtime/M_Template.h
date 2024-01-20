#pragma once

#include "M_Object.h"
#include "M_ISerializable.h"
#include "M_EntityRegistry.h"
#include "M_Register.h"
#include <unordered_map>

namespace minty
{
	/*/// <summary>
	/// Holds the data for a template for an Entity.
	/// </summary>
	class Template
		: public Object
	{
	private:
		struct Data
		{
			std::vector<Component*> components;
		};

		Register<Data> _data;

	public:
		/// <summary>
		/// Creates a new empty Template.
		/// </summary>
		Template();

		/// <summary>
		/// Creates a new Template from the given Entity.
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="registry"></param>
		Template(Entity const entity, EntityRegistry& registry);
		
	private:
		void emplace(Entity const entity, EntityRegistry& registry, std::unordered_map<Entity, ID>& entities);

	public:
		virtual void serialize(Writer& writer) const;
		
		virtual void deserialize(Reader const& reader);
	};*/
}