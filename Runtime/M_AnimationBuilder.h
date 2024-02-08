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
		/// A list of all variable names being set by this Animation.
		/// </summary>
		std::vector<String> variables;

		/// <summary>
		/// A list of all values being set by this Animation.
		/// </summary>
		std::vector<Node> values;

		/// <summary>
		/// The steps within this Animation.
		/// </summary>
		std::vector<std::pair<float, Animation::Step>> steps;

		/// <summary>
		/// The steps taken when resetting this Animation.
		/// </summary>
		std::vector<Animation::Step> resetSteps;
	};
}