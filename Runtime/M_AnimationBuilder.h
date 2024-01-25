#pragma once

#include "M_Base.h"
#include "M_Dynamic.h"
#include "M_Animation.h"

namespace minty
{
	/// <summary>
	/// Holds data to create a new Animation.
	/// </summary>
	struct AnimationBuilder
	{
		/// <summary>
		/// The name of the Animation.
		/// </summary>
		String name;

		/// <summary>
		/// The amount of time this Animation runs for, in seconds.
		/// </summary>
		float length;

		/// <summary>
		/// The animation will loop when it ends.
		/// </summary>
		bool loops;

		/// <summary>
		/// A list of all Entities being affected by this Animation.
		/// </summary>
		std::vector<String> entities;

		/// <summary>
		/// A list of all Components being edited by this Animation.
		/// </summary>
		std::vector<String> components;

		/// <summary>
		/// A list of all offset/sizes being set by this Animation.
		/// 
		/// Each offset/size corresponds to somewhere within one of the components being edited.
		/// </summary>
		std::vector<size_t> sizes;

		/// <summary>
		/// A list of all values being set by this Animation.
		/// </summary>
		std::vector<Dynamic> values;

		/// <summary>
		/// The compilation of steps within this Animation.
		/// [Entity index: 8 bits][Component index: 8 bits][offset index: 8 bits][size index: 8 bits][value index: 32 bits]
		/// </summary>
		std::vector<std::pair<float, Animation::Step>> steps;
	};
}