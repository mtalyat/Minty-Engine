using System;
using System.Runtime.CompilerServices;

namespace MintyEngine
{
    /// <summary>
    /// Holds functionality for interacting with the Minty Runtime.
    /// </summary>
    internal static class Runtime
    {
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Log(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_LogColor(string message, int color);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Warn(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Error(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Assert(string message);
    }
}
