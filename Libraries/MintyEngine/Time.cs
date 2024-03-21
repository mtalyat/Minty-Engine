using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Provides an interface to get time information from the Minty Runtime.
    /// </summary>
    public static class Time
    {
        public static float TotalTime
        {
            get => Runtime.Time_GetTotalTime();
        }

        public static float ElapsedTime
        {
            get => Runtime.Time_GetElapsedTime();
        }
    }
}
