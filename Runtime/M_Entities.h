#pragma once

#include "libraries/entt/entt.hpp"

namespace minty
{
	typedef entt::entity Entity;

	class Registry
		: public entt::registry
	{
	private:

	public:
		Registry();

		std::string get_name(Entity const entity) const;
	};
}