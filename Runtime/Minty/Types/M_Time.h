#pragma once

#include "Minty/Types/M_Types.h"

namespace Minty
{
	struct Time
	{
		/// <summary>
		/// The total time in seconds from start to end.
		/// </summary>
		float total;
		
		/// <summary>
		/// The elapsed time in seconds.
		/// </summary>
		float elapsed;

		/// <summary>
		/// Gets the TimePoint for now.
		/// </summary>
		/// <returns></returns>
		static TimePoint now();

		/// <summary>
		/// Gets the duration between start and end in seconds.
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <returns></returns>
		static float calculate_duration_seconds(TimePoint const start, TimePoint const end);

		/// <summary>
		/// Gets the current time as a time stamp string.
		/// </summary>
		/// <returns></returns>
		static String timestamp();
	};
}