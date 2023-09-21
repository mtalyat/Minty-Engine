#pragma once
#include "M_Object.h"

#include "M_Entities.h"

namespace minty
{
	/// <summary>
	/// 
	/// </summary>
	class Scene :
		public Object
	{
	private:
		Registry _registry;

	public:
		/// <summary>
		/// Creates an empty Scene.
		/// </summary>
		Scene();

		/// <summary>
		/// Gets the registry used in the Scene.
		/// </summary>
		/// <returns></returns>
		Registry& getRegistry();
	};
}