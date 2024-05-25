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
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string Entity_GetTag(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetTag(ulong id, string tag);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Entity_GetEnabled(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetEnabled(ulong id, bool enabled);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_AddComponent(ulong id, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetComponent(ulong id, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Entity_RemoveComponent(ulong id, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetParent(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetParent(ulong id, ulong parentId);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static int Entity_GetChildCount(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetChild(ulong id, int index);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_Clone(ulong id);
        #endregion

        #region Window
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Window_GetMain();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string Window_GetTitle(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_SetTitle(ulong id, string title);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_SetIcon(ulong id, string path);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Window_IsOpen(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Close(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Maximize(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Minimize(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Restore(ulong id);
        #endregion

        #region Components

        #region Camera
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static int Camera_GetPerspective(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetPerspective(ulong id, int perspective);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Camera_GetFov(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetFov(ulong id, float perspective);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Camera_GetNear(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetNear(ulong id, float perspective);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Camera_GetFar(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetFar(ulong id, float perspective);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetAsMain(ulong id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetColor(ulong id, int color);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static int Camera_GetColor(ulong id);
        #endregion

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
