#pragma once

#include "types/M_Types.h"

namespace minty
{
	struct Time
	{
		/// <summary>
		/// The beginning point in time.
		/// </summary>
		TimePoint start;

		/// <summary>
		/// The most recent time recorded.
		/// </summary>
		TimePoint end;

		/// <summary>
		/// The total time in seconds from start to end.
		/// </summary>
		float total;
		
		/// <summary>
		/// The elapsed time in seconds.
		/// </summary>
		float elapsed;

		/// <summary>
		/// Gets the total time in seconds right at this instant.
		/// </summary>
		/// <returns></returns>
		float get_time_now() const;

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