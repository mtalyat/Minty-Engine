using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public enum CursorMode : int
    {
        Normal,
        Hidden,
        Disabled,
    }

    /// <summary>
    /// Represents the mouse cursor on the screen.
    /// </summary>
    public static class Cursor
    {
        public static CursorMode Mode
        {
            get => Runtime.Cursor_GetMode();
            set => Runtime.Cursor_SetMode(value);
        }
    }
}
