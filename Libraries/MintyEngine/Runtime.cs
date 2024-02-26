using System;
using System.Runtime.CompilerServices;

namespace MintyEngine
{
    /// <summary>
    /// Holds functionality for interacting with the Minty Runtime.
    /// </summary>
    public static class Runtime
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

        #region Cursor
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static CursorMode Cursor_GetMode();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Cursor_SetMode(CursorMode mode);
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
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetParent(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetParent(ulong id, ulong parentId);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static int Entity_GetChildCount(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetChild(ulong id, int index);
        #endregion

        #region Window
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string Window_GetTitle();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_SetTitle(string title);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_SetIcon(string path);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Window_IsOpen();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Close();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Maximize();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Minimize();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Restore();
        #endregion

        #region Components

        #region Transform
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetLocalPosition(ulong id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetLocalPosition(ulong id, in Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetLocalRotation(ulong id, out Vector4 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetLocalRotation(ulong id, in Vector4 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetLocalScale(ulong id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetLocalScale(ulong id, in Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetRight(ulong id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetUp(ulong id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetForward(ulong id, out Vector3 position);
        #endregion

        #endregion
    }
}
