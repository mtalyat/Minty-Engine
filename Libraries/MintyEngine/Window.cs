using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
    /// Represents a Window on the screen that the engine is rendering to.
    /// </summary>
    public static class Window
    {
        public static string Title
        {
            get => Runtime.Window_GetTitle();
            set => Runtime.Window_SetTitle(value);
        }

        public static bool IsOpen
        {
            get => Runtime.Window_IsOpen();
        }

        public static void SetIcon(string path)
        {
            Runtime.Window_SetIcon(path);
        }

        public static void Close()
        {
            Runtime.Window_Close();
        }

        /// <summary>
        /// Maximizes the Window.
        /// </summary>
        public static void Maximize()
        {
            Runtime.Window_Maximize();
        }

        /// <summary>
        /// Minimizes the window.
        /// </summary>
        public static void Minimize()
        {
            Runtime.Window_Minimize();
        }

        /// <summary>
        /// Returns the window from minimized or maximized back to a regular window state.
        /// </summary>
        public static void Restore()
        {
            Runtime.Window_Restore();
        }
    }
}
