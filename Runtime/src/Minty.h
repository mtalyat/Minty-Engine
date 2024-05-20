#pragma once

/*

Minty Runtime header file.

All of the following files included will allow a user to include just this file, which will allow
access to the entire engine without needing include each file.

*/

// ANIMATION
#include "animation/M_Animation.h"
#include "animation/M_AnimationSystem.h"
#include "animation/M_Animator.h"
#include "animation/M_AnimatorComponent.h"

// ASSETS
#include "assets/M_Asset.h"
#include "assets/M_AssetEngine.h"
#include "assets/M_Text.h"

// AUDIO
#include "audio/M_Audio.h"
#include "audio/M_AudioClip.h"
#include "audio/M_AudioEngine.h"
#include "audio/M_AudioListenerComponent.h"
#include "audio/M_AudioObject.h"
#include "audio/M_AudioSourceComponent.h"
#include "audio/M_AudioSystem.h"

// COMPONENTS
#include "components/M_Component.h"
#include "components/M_DestroyEntityComponent.h"
#include "components/M_DirtyComponent.h"
#include "components/M_EnabledComponent.h"
#include "components/M_NameComponent.h"
#include "components/M_RelationshipComponent.h"
#include "components/M_TagComponent.h"
#include "components/M_TransformComponent.h"

// CORE
#include "core/M_Base.h"
#include "core/M_Constants.h"
#include "core/M_Macros.h"

// ENTITIES
#include "entities/M_Entity.h"
#include "entities/M_EntityRegistry.h"

// FILES
#include "files/M_File.h"
#include "files/M_PhysicalFile.h"
#include "files/M_VirtualFile.h"
#include "files/M_Wrap.h"
#include "files/M_Wrapper.h"

// INPUT
#include "input/M_CursorMode.h"
#include "input/M_Gamepad.h"
#include "input/M_InputMap.h"
#include "input/M_Key.h"
#include "input/M_KeyAction.h"
#include "input/M_KeyModifiers.h"
#include "input/M_MouseButton.h"

// LIBRARIES
#ifdef MINTY_IMGUI
#include "M_ImGui.h"
#endif

// MATH
#include "math/M_Math.h"

// MULTITHREADING
#include "multithreading/M_Task.h"

// RENDERING
#include "rendering/M_Buffer.h"
#include "rendering/M_Builtin.h"
#include "rendering/M_Camera.h"
#include "rendering/M_CameraComponent.h"
#include "rendering/M_CoordinateMode.h"
#include "rendering/M_DescriptorSet.h"
#include "rendering/M_DrawCallObjectInfo.h"
#include "rendering/M_Material.h"
#include "rendering/M_MaterialTemplate.h"
#include "rendering/M_Mesh.h"
#include "rendering/M_MeshComponent.h"
#include "rendering/M_PushConstantInfo.h"
#include "rendering/M_RenderableComponent.h"
#include "rendering/M_RenderEngine.h"
#include "rendering/M_RenderObject.h"
#include "rendering/M_RenderSystem.h"
#include "rendering/M_Shader.h"
#include "rendering/M_ShaderPass.h"
#include "rendering/M_Sprite.h"
#include "rendering/M_SpriteComponent.h"
#include "rendering/M_SpritePushData.h"
#include "rendering/M_Texture.h"
#include "rendering/M_TextureAtlas.h"
#include "rendering/M_UniformConstantInfo.h"
#include "rendering/M_Viewport.h"
#include "rendering/M_Window.h"

// RUNTIME
#include "runtime/M_Application.h"
#include "runtime/M_Engine.h"
#include "runtime/M_Info.h"
#include "runtime/M_RunMode.h"
#include "runtime/M_Runtime.h"
#include "runtime/M_RuntimeObject.h"

// SCENES
#include "scenes/M_Scene.h"
#include "scenes/M_SceneManager.h"
#include "scenes/M_SceneObject.h"

// SCRIPTING
#include "scripting/M_Accessibility.h"
#include "scripting/M_ScriptArguments.h"
#include "scripting/M_ScriptAssembly.h"
#include "scripting/M_ScriptClass.h"
#include "scripting/M_ScriptComponent.h"
#include "scripting/M_ScriptEngine.h"
#include "scripting/M_ScriptObject.h"
#include "scripting/M_ScriptSystem.h"

// SERIALIZATION
#include "serialization/M_ISerializable.h"
#include "serialization/M_Reader.h"
#include "serialization/M_SerializationData.h"
#include "serialization/M_Writer.h"

// SYSTEMS
#include "systems/M_System.h"
#include "systems/M_SystemRegistry.h"

// TOOLS
#include "tools/M_Compression.h"
#include "tools/M_Console.h"
#include "tools/M_Debug.h"
#include "tools/M_Encoding.h"
#include "tools/M_Operations.h"
#include "tools/M_Parse.h"

// TYPES
#include "types/M_Color.h"
#include "types/M_CommandLineParser.h"
#include "types/M_Dynamic.h"
#include "types/M_Event.h"
#include "types/M_FiniteStateMachine.h"
#include "types/M_Logger.h"
#include "types/M_Matrix.h"
#include "types/M_Node.h"
#include "types/M_Object.h"
#include "types/M_Quaternion.h"
#include "types/M_Rect.h"
#include "types/M_RectF.h"
#include "types/M_Stopwatch.h"
#include "types/M_Time.h"
#include "types/M_TypeRegister.h"
#include "types/M_Types.h"
#include "types/M_UUID.h"
#include "types/M_Vector.h"

// UI
#include "ui/M_CanvasComponent.h"
#include "ui/M_UIPushData.h"
#include "ui/M_UISystem.h"
#include "ui/M_UITransformComponent.h"