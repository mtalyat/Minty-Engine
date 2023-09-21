#pragma once

#include "M_Object.h"
#include <chrono>

namespace minty
{
    typedef long long elapsed_t;

    constexpr elapsed_t ONE_MILLISECOND = 1000000ll;
    constexpr elapsed_t ONE_SECOND = ONE_MILLISECOND * 1000ll;
    constexpr elapsed_t ONE_MINUTE = ONE_SECOND * 60ll;
    constexpr elapsed_t ONE_HOUR = ONE_MINUTE * 60ll;

    /// <summary>
    /// Holds data for time elapsed over a time period recorded.
    /// </summary>
    class Stopwatch :
        public Object
    {
    private:
        /// <summary>
        /// The start time in nanoseconds, when this Stopwatch was started.
        /// </summary>
        std::chrono::steady_clock::time_point m_start;

        /// <summary>
        /// The total amount of time in nanoseconds that have passed.
        /// </summary>
        elapsed_t m_elapsed;

        /// <summary>
        /// Is the Stopwatch currently recording time?
        /// </summary>
        bool m_running;

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
        constexpr bool isRunning() const { return m_running; }

        /// <summary>
        /// Attempts to lap. Returns the number of laps that have occured.
        /// </summary>
        /// <param name="mod"></param>
        /// <returns>The maximum number of laps.</returns>
        elapsed_t lap(elapsed_t const mod);

        /// <summary>
        /// Stop, reset and start.
        /// </summary>
        inline void restart() { stop(); reset(); start(); }

        /// <summary>
        /// How much time has elapsed since the start?
        /// </summary>
        /// <returns>Time elapsed in nanoseconds.</returns>
        elapsed_t elapsed() const;

        /// <summary>
        /// How much time (in milliseconds) have elapsed since the start?
        /// </summary>
        /// <returns></returns>
        elapsed_t elapsed_ms() const;

        /// <summary>
        /// How much time (in seconds) have elapsed since the start?
        /// </summary>
        /// <returns></returns>
        float elapsed_s() const;

        /// <summary>
        /// Creates a new Stopwatch, and starts it immediately after.
        /// </summary>
        /// <returns></returns>
        inline static Stopwatch startNew() { Stopwatch watch; watch.start(); return watch; }

        std::string const toString() const override;
    };
}
