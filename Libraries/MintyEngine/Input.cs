using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    /// <summary>
	/// A key on the keyboard.
	/// </summary>
	public enum Key : int
    {
        Unknown = -1,
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        D0 = 48,
        D1 = 49,
        D2 = 50,
        D3 = 51,
        D4 = 52,
        D5 = 53,
        D6 = 54,
        D7 = 55,
        D8 = 56,
        D9 = 57,
        Semicolon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        Grave = 96,

        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KeyPad0 = 320,
        KeyPad1 = 321,
        KeyPad2 = 322,
        KeyPad3 = 323,
        KeyPad4 = 324,
        KeyPad5 = 325,
        KeyPad6 = 326,
        KeyPad7 = 327,
        KeyPad8 = 328,
        KeyPad9 = 329,
        KeyPadDecimal = 330,
        KeyPadDivide = 331,
        KeyPadMultiply = 332,
        KeyPadSubtract = 333,
        KeyPadAdd = 334,
        KeyPadEnter = 335,
        KeyPadEqual = 336,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348,
    };

    /// <summary>
    /// The modifer keys that can be held.
    /// </summary>
    public enum KeyModifiers : int
    {
        Shift = 0b000001,
        Control = 0b000010,
        Alt = 0b000100,
        Super = 0b001000,
        CapsLock = 0b010000,
        NumLock = 0b100000
    };

    /// <summary>
    /// The state of a key or button.
    /// </summary>
    public enum ButtonAction : int
    {
        Up = 0,
        Down = 1
    };

    internal static class ButtonActionExtensions
    {
        internal const int BUTTON_ACTION_COUNT = 2;
    }

    /// <summary>
    /// A button on the mouse.
    /// </summary>
    public enum MouseButton : int
    {
        Button1 = 0,
        Button2 = 1,
        Button3 = 2,
        Button4 = 3,
        Button5 = 4,
        Button6 = 5,
        Button7 = 6,
        Button8 = 7,

        Left = Button1,
        Right = Button2,
        Middle = Button3,
    };

    public class KeyPressEventArgs : EventArgs
    {
        private Key _key;
        public Key Key => _key;

        private ButtonAction _action;
        public ButtonAction Action => _action;

        private KeyModifiers _mods;
        public KeyModifiers Modifiers => _mods;

        public KeyPressEventArgs(Key key, ButtonAction action, KeyModifiers mods)
        {
            _key = key;
            _action = action;
            _mods = mods;
        }
    }

    public class MouseClickEventArgs : EventArgs
    {
        private MouseButton _button;
        public MouseButton Button => _button;

        private ButtonAction _action;
        public ButtonAction Action => _action;

        private KeyModifiers _modifiers;
        public KeyModifiers Modifiers => _modifiers;

        private Vector2 _position;
        public Vector2 Position => _position;

        public MouseClickEventArgs(MouseButton button, ButtonAction action, KeyModifiers modifiers, Vector2 position)
        {
            _button = button;
            _action = action;
            _modifiers = modifiers;
            _position = position;
        }
    }

    public class MouseMoveEventArgs : EventArgs
    {
        private Vector2 _newPosition;
        public Vector2 NewPosition => _newPosition;

        private Vector2 _deltaPosition;
        public Vector2 DeltaPosition => _deltaPosition;

        private Vector2 _oldPosition;
        public Vector2 OldPosition => _oldPosition;

        public MouseMoveEventArgs(Vector2 newPosition, Vector2 oldPosition)
        {
            _newPosition = newPosition;
            _deltaPosition = newPosition - oldPosition;
            _oldPosition = oldPosition;
        }
    }

    public class MouseScrollEventArgs : EventArgs
    {
        private Vector2 _deltaScroll;
        public Vector2 DeltaScroll => _deltaScroll;

        public MouseScrollEventArgs(Vector2 deltaScroll)
        {
            _deltaScroll = deltaScroll;
        }
    }

    public delegate void KeyPressEventHandler(object sender, KeyPressEventArgs e);
    public delegate void MouseClickEventHandler(object sender, MouseClickEventArgs e);
    public delegate void MouseMoveEventHandler(object sender, MouseMoveEventArgs e);
    public delegate void MouseScrollEventHandler(object sender, MouseScrollEventArgs e);

    public class InputMap
    {
        private EventHandlerCollection<Key, KeyPressEventHandler, KeyPressEventArgs>[] _keyEvents = new EventHandlerCollection<Key, KeyPressEventHandler, KeyPressEventArgs>[] { new EventHandlerCollection<Key, KeyPressEventHandler, KeyPressEventArgs>(), new EventHandlerCollection<Key, KeyPressEventHandler, KeyPressEventArgs>() };
        public EventHandlerCollection<Key, KeyPressEventHandler, KeyPressEventArgs> OnKeyUp => _keyEvents[(int)ButtonAction.Up];
        public EventHandlerCollection<Key, KeyPressEventHandler, KeyPressEventArgs> OnKeyDown => _keyEvents[(int)ButtonAction.Down];

        private EventHandlerCollection<MouseButton, MouseClickEventHandler, MouseClickEventArgs>[] _mouseEvents = new EventHandlerCollection<MouseButton, MouseClickEventHandler, MouseClickEventArgs>[] { new EventHandlerCollection<MouseButton, MouseClickEventHandler, MouseClickEventArgs>(), new EventHandlerCollection<MouseButton, MouseClickEventHandler, MouseClickEventArgs>() };
        public EventHandlerCollection<MouseButton, MouseClickEventHandler, MouseClickEventArgs> OnMouseUp => _mouseEvents[(int)ButtonAction.Up];
        public EventHandlerCollection<MouseButton, MouseClickEventHandler, MouseClickEventArgs> OnMouseDown => _mouseEvents[(int)ButtonAction.Down];

        private event MouseMoveEventHandler _mouseMove;
        public MouseMoveEventHandler OnMouseMove
        {
            get => _mouseMove;
            set => _mouseMove = value;
        }

        private event MouseScrollEventHandler _mouseScroll = null;
        public MouseScrollEventHandler OnMouseScroll
        {
            get => _mouseScroll;
            set => _mouseScroll = value;
        }

        public InputMap()
        {

        }

        #region Trigger

        internal void TriggerKey(KeyPressEventArgs e)
        {
            _keyEvents[(int)e.Action]?.Invoke(e.Key, this, e);
        }

        internal void TriggerMouseClick(MouseClickEventArgs e)
        {
            _mouseEvents[(int)e.Action]?.Invoke(e.Button, this, e);
        }

        internal void TriggerMouseMove(MouseMoveEventArgs e)
        {
            _mouseMove?.Invoke(this, e);
        }

        internal void TriggerMouseScroll(MouseScrollEventArgs e)
        {
            _mouseScroll?.Invoke(this, e);
        }

        #endregion
    }

    /// <summary>
    /// Handles all input from the user into the device, including, but not limited to:
    /// keyboard input, gamepad input, mouse inputs, etc.
    /// </summary>
    public static class Input
    {
        private static InputMap _activeMap = null;

        private static HashSet<Key> _keyStates = new HashSet<Key>(); // all down keys
        private static HashSet<MouseButton> _buttonStates = new HashSet<MouseButton>(); // all down mouse buttons

        private static Vector2 _mousePosition;

        public static void SetInputMap(InputMap inputMap)
        {
            _activeMap = inputMap;
        }

        #region Key Checking

        public static bool IsKeyDown(Key key)
        {
            return _keyStates.Contains(key);
        }

        public static bool IsMouseButtonDown(MouseButton button)
        {
            return _buttonStates.Contains(button);
        }

        #endregion

        #region Trigger

        private static void TriggerKey(Key key, ButtonAction action, KeyModifiers mods)
        {
            switch (action)
            {
                case ButtonAction.Up:
                    _keyStates.Remove(key);
                    break;
                case ButtonAction.Down:
                    _keyStates.Add(key);
                    break;
            }

            if (_activeMap != null)
            {
                _activeMap.TriggerKey(new KeyPressEventArgs(key, action, mods));
            }
        }

        private static void TriggerMouseClick(MouseButton button, ButtonAction action, KeyModifiers mods)
        {
            switch (action)
            {
                case ButtonAction.Up:
                    _buttonStates.Remove(button);
                    break;
                case ButtonAction.Down:
                    _buttonStates.Add(button);
                    break;
            }

            if (_activeMap != null)
            {
                _activeMap.TriggerMouseClick(new MouseClickEventArgs(button, action, mods, _mousePosition));
            }
        }

        private static void TriggerMouseMove(float x, float y)
        {
            Vector2 newPosition = new Vector2(x, y);
            if (_activeMap != null)
            {
                _activeMap.TriggerMouseMove(new MouseMoveEventArgs(newPosition, _mousePosition));
            }
            _mousePosition = newPosition;
        }

        private static void TriggerMouseScroll(float dx, float dy)
        {
            if (_activeMap != null)
            {
                _activeMap.TriggerMouseScroll(new MouseScrollEventArgs(new Vector2(dx, dy)));
            }
        }

        #endregion
    }
}
