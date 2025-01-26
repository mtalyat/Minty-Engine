using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents the Screen being rendered to.
    /// </summary>
    public static class Screen
    {
        public static uint Width => Runtime.Screen_GetWidth();
        public static uint Height => Runtime.Screen_GetHeight();
    }
}
