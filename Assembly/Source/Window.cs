using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using UUID = System.UInt64;

namespace MintyEngine
{
    /// <summary>
    /// Represents a Window on the screen that the engine is rendering to.
    /// </summary>
    public class Window
    {
        internal readonly UUID ID;

        public static Window Main
        {
            get => (Window)Runtime.Window_GetMain();
        }

        public string Title
        {
            get => Runtime.Window_GetTitle(ID);
            set => Runtime.Window_SetTitle(ID, value);
        }

        public bool IsOpen
        {
            get => Runtime.Window_IsOpen(ID);
        }

        internal Window(uint id)
        {
            ID = id;
        }

        public void SetIcon(string path)
        {
            Runtime.Window_SetIcon(ID, path);
        }

        public void Close()
        {
            Runtime.Window_Close(ID);
        }

        /// <summary>
        /// Maximizes the Window.
        /// </summary>
        public void Maximize()
        {
            Runtime.Window_Maximize(ID);
        }

        /// <summary>
        /// Minimizes the window.
        /// </summary>
        public void Minimize()
        {
            Runtime.Window_Minimize(ID);
        }

        /// <summary>
        /// Returns the window from minimized or maximized back to a regular window state.
        /// </summary>
        public void Restore()
        {
            Runtime.Window_Restore(ID);
        }
    }
}
