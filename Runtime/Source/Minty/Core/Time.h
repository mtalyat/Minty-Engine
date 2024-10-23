#pragma once

#include "Minty/Core/String.h"
#include "Minty/Core/Types.h"

namespace Minty
{
	struct Time
	{
		/// <summary>
		/// The total time in seconds from start to end.
		/// </summary>
		Float total;

		/// <summary>
		/// The elapsed time in seconds.
		/// </summary>
		Float elapsed;

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
		static Float calculate_duration_seconds(TimePoint const start, TimePoint const end);

		/// <summary>
		/// Gets the duration between start and end in milliseconds.
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <returns></returns>
		static Float calculate_duration_milliseconds(TimePoint const start, TimePoint const end);

		/// <summary>
		/// Gets the current time as a time stamp string.
		/// </summary>
		/// <returns></returns>
		static String timestamp();
	};
}