#pragma once

#include "Minty/Core/Type.h"
#include "Minty/Core/Time.h"

namespace Minty
{
    constexpr TimeElapsed ONE_MILLISECOND = 1000000ll;
    constexpr TimeElapsed ONE_SECOND = ONE_MILLISECOND * 1000ll;
    constexpr TimeElapsed ONE_MINUTE = ONE_SECOND * 60ll;
    constexpr TimeElapsed ONE_HOUR = ONE_MINUTE * 60ll;

    /// <summary>
    /// Holds data for time elapsed over a time period recorded.
    /// </summary>
    class Stopwatch
    {
    private:
        /// <summary>
        /// The start time in nanoseconds, when this Stopwatch was started.
        /// </summary>
        TimePoint m_start;

        /// <summary>
        /// The total amount of time in nanoseconds that have passed.
        /// </summary>
        TimeElapsed m_elapsed;

        /// <summary>
        /// Is the Stopwatch currently recording time?
        /// </summary>
        Bool m_running;

    public:
        /// <summary>
        /// Creates a new Stopwatch.
        /// </summary>
        Stopwatch();

        /// <summary>
        /// Start recording time.
        /// </summary>
        void start();

        /// <summary>
        /// Stop recording time.
        /// </summary>
        void stop();

        /// <summary>
        /// Reset the time recorded (elapsed time to zero).
        /// </summary>
        void reset();

        /// <summary>
        /// Checks if this Stopwatch is recording time.
        /// </summary>
        /// <returns></returns>
        constexpr Bool running() const { return m_running; }

        /// <summary>
        /// Attempts to lap. Returns the number of laps that have occured.
        /// </summary>
        /// <param name="mod"></param>
        /// <returns>The maximum number of laps.</returns>
        TimeElapsed lap(TimeElapsed const mod);

        /// <summary>
        /// Stop, reset and start.
        /// </summary>
        inline void restart() { stop(); reset(); start(); }

        /// <summary>
        /// How much time has elapsed since the start?
        /// </summary>
        /// <returns>Time elapsed in nanoseconds.</returns>
        TimeElapsed elapsed() const;

        /// <summary>
        /// How much time (in milliseconds) have elapsed since the start?
        /// </summary>
        /// <returns></returns>
        TimeElapsed elapsed_ms() const;

        /// <summary>
        /// How much time (in seconds) have elapsed since the start?
        /// </summary>
        /// <returns></returns>
        float elapsed_s() const;

        /// <summary>
        /// How much time has elapsed since the start, as a string.
        /// </summary>
        /// <returns></returns>
        String elapsed_string() const;

        /// <summary>
        /// Creates a new Stopwatch, and starts it immediately after.
        /// </summary>
        /// <returns></returns>
        static Stopwatch start_new() { Stopwatch watch; watch.start(); return watch; }
    };
}
