using System;
using System.Runtime.CompilerServices;

using UUID = System.UInt64;

namespace MintyEngine
{
    /// <summary>
    /// Holds functionality for interacting with the Minty Runtime.
    /// </summary>
    public static class Runtime
    {
        // [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Class_Function();

        #region Components

        #region Camera
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static int Camera_GetPerspective(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetPerspective(UUID id, int perspective);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Camera_GetFov(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetFov(UUID id, float perspective);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Camera_GetNear(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetNear(UUID id, float perspective);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Camera_GetFar(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetFar(UUID id, float perspective);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Camera_GetMain();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetMain(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetColor(UUID id, int color);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static int Camera_GetColor(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Camera_SetRenderTarget(UUID id, UUID renderTarget);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Camera_GetRenderTarget(UUID id);
        #endregion

        #region MeshRenderer
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static int MeshRenderer_GetType(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void MeshRenderer_SetType(UUID id, int type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object MeshRenderer_GetMesh(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void MeshRenderer_SetMesh(UUID id, UUID meshId);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object MeshRenderer_GetMaterial(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void MeshRenderer_SetMaterial(UUID id, UUID materialId);
        #endregion

        #region Transform
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetLocalPosition(UUID id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetLocalPosition(UUID id, in Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetLocalRotation(UUID id, out Vector4 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetLocalRotation(UUID id, in Vector4 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetLocalScale(UUID id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetLocalScale(UUID id, in Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetGlobalPosition(UUID id, out Vector3 position);
        //[MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetGlobalPosition(UUID id, in Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetGlobalRotation(UUID id, out Vector4 position);
        //[MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetGlobalRotation(UUID id, in Vector4 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetGlobalScale(UUID id, out Vector3 position);
        //[MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_SetGlobalScale(UUID id, in Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetRight(UUID id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetUp(UUID id, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Transform_GetForward(UUID id, out Vector3 position);
        #endregion

        #endregion

        #region Core

        #region Time
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Time_GetTotalTime();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Time_GetElapsedTime();
        #endregion

        #region Console
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Log(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_LogColor(string message, int color);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Warn(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Console_Error(string message);
        #endregion

        #endregion

        #region Input

        #region Cursor
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static CursorMode Cursor_GetMode();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Cursor_SetMode(CursorMode mode);
        #endregion

        #endregion

        #region Object

        #region Entity
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_Find(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string Entity_GetName(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetName(UUID id, string name);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string Entity_GetTag(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetTag(UUID id, string tag);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static uint Entity_GetLayer(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetLayer(UUID id, uint layer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Entity_GetEnabled(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetEnabled(UUID id, bool enabled);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_AddComponent(UUID id, System.Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetComponent(UUID id, System.Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Entity_RemoveComponent(UUID id, System.Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetParent(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Entity_SetParent(UUID id, UUID parentId);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static int Entity_GetChildCount(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_GetChild(UUID id, int index);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Entity_Clone(UUID id);
        #endregion

        #region Object
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Object_DestroyEntity(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Object_DestroyImmediateEntity(UUID id);
        #endregion

        #endregion

        #region Render

        #region Image
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Image_Create(int format, int type, int tiling, int aspect, int usage, uint width, uint height, bool immutable);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Image_CreateBasic(uint width, uint height);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static uint Image_GetWidth(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static uint Image_GetHeight(UUID id);
        #endregion

        #region Material
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Material_Create(string path);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Material_GetTemplate(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Material_Set(UUID id, string name, int type, string value);
        #endregion

        #region MaterialTemplate
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID MaterialTemplate_Create(string path);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object MaterialTemplate_GetShader(UUID id);
        #endregion

        #region Mesh
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Mesh_Create(string path);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Mesh_CreateFromType(int type);
        #endregion

        #region Renderer

        #endregion

        //#region RenderPass
        //[MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID RenderPass_Create();
        //[MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool RenderPass_UpdateAttachment(UUID id, int type, int format, int loadOp, int storeOp, int initialLayout, int finalLayout);
        //[MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool RenderPass_Apply(UUID id);
        //#endregion

        #region RenderTarget
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID RenderTarget_Create(bool framebuffer);
        #endregion

        #region Shader
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Shader_Create(string path);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Shader_GetViewport(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Shader_SetViewport(UUID id, UUID viewportId);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Shader_GetScissor(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Shader_SetScissor(UUID id, UUID scissorId);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Shader_HasInput(UUID id, string name);
        #endregion

        #region ShaderModule
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID ShaderModule_Create(string path);
        #endregion

        #region Scissor
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Scissor_Create(float x, float y, float width, float height);
        #endregion

        #region Screen
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static uint Screen_GetWidth();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static uint Screen_GetHeight();
        #endregion

        #region Texture
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Texture_Create(UUID imageId, Filter filter, ImageAddressMode addressMode, bool normalizedCoordinates);
        #endregion

        #region Viewport
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static UUID Viewport_Create(float x, float y, float width, float height, float minDepth, float maxDepth);
        #endregion

        #endregion

        #region Scene

        #region SceneManager
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void SceneManager_Load(string path);
        #endregion

        #endregion

        #region Window

        #region Window
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static object Window_GetMain();
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string Window_GetTitle(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_SetTitle(UUID id, string title);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_SetIcon(UUID id, string path);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Window_IsOpen(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Close(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Maximize(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Minimize(UUID id);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Window_Restore(UUID id);
        #endregion

        #endregion
    }
}
