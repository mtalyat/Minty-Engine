#pragma once

namespace Minty
{
	/// <summary>
	/// Determines how spatial audio scales based on the distance from the source to the listener.
	/// </summary>
	enum class Attenuation
	{
		None,

		/// <summary>
		/// The sound gets linearly quieter as the source approaches the listener?
		/// </summary>
		InverseDistance,

		/// <summary>
		/// The sound gets linearly louder as the source approaches the listener.
		/// </summary>
		LinearDistance,

		/// <summary>
		/// The sound gets exponentially louder as the source approaches the listener.
		/// </summary>
		ExponentialDistance
	};
}