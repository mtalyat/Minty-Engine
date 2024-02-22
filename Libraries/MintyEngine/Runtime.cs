using System;
using System.Runtime.CompilerServices;

namespace MintyEngine
{
    /// <summary>
    /// Holds functionality for interacting with the Minty Runtime.
    /// </summary>
    internal static class Runtime
    {
        // [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Class_Function();

        #region Time
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Time_GetTotalTime();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Time_GetElapsedTime();
        #endregion

        #region Console
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Log(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_LogColor(string message, int color);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Warn(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Error(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Assert(string message);
        #endregion

        #region Object
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Object_DestroyEntity(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Object_DestroyImmediateEntity(ulong id);
        #endregion

        #region Entity
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string Entity_GetName(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetName(ulong id, string name);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Entity_GetEnabled(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetEnabled(ulong id, bool enabled);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_AddComponent(ulong id, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetComponent(ulong id, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Entity_RemoveComponent(ulong id, Type type);
        #endregion

        #region Components

        #region Transform
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static Vector3 Transform_GetLocalPosition(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetLocalPosition(ulong id, Vector3 position);
        #endregion

        #endregion
    }
}
