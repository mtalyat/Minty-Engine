using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Various functions for access to the debugging tools.
    /// </summary>
    public static class Debug
    {
        public enum Color
        {
            Black = 0,
            Red = 1,
            Green = 2,
            Yellow = 3,
            Blue = 4,
            Magenta = 5,
            Cyan = 6,
            White = 7,

            Gray = 8,
            BrightRed = 9,
            BrightGreen = 10,
            BrightYellow = 11,
            BrightBlue = 12,
            BrightMagenta = 13,
            BrightCyan = 14,
            BrightWhite = 15
        };

        public static void Log(object obj)
        {
            Runtime.Console_Log(obj.ToString());
        }

        public static void Log(object obj, Color color)
        {
            Runtime.Console_LogColor(obj.ToString(), (int)color);
        }

        public static void Warn(object obj)
        {
            Runtime.Console_Warn(obj.ToString());
        }
        
        public static void Error(object obj)
        {
            Runtime.Console_Error(obj.ToString());
        }
    }
}
