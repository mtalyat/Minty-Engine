#pragma once

#include "M_Base.h"

namespace minty
{
	/// <summary>
	/// Holds data to create a new Material.
	/// </summary>
	struct AnimationBuilder
	{
		/// <summary>
		/// The name of the Animation.
		/// </summary>
		String name;

		/// <summary>
		/// The time each frame takes to elapse in seconds.
		/// </summary>
		float frameTime;

		/// <summary>
		/// A list of Sprite IDs for each frame in the animation.
		/// </summary>
		std::vector<ID> frames;
	};
}