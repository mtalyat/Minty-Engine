using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
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
