#pragma once

/*

Minty Runtime header file.

All of the following files included will allow a user to include just this file, which will allow
access to the entire engine without needing include each file.

*/

// ANIMATION
#include "Minty/Animation/M_Animation.h"
#include "Minty/Animation/M_AnimationSystem.h"
#include "Minty/Animation/M_Animator.h"
#include "Minty/Animation/M_AnimatorComponent.h"

// ASSETS
#include "Minty/Assets/M_Asset.h"
#include "Minty/Assets/M_AssetEngine.h"

// AUDIO
#include "Minty/Audio/M_Audio.h"
#include "Minty/Audio/M_AudioClip.h"
#include "Minty/Audio/M_AudioEngine.h"
#include "Minty/Audio/M_AudioListenerComponent.h"
#include "Minty/Audio/M_AudioSourceComponent.h"
#include "Minty/Audio/M_AudioSystem.h"

// COMPONENTS
#include "Minty/Components/M_Component.h"
#include "Minty/Components/M_DestroyEntityComponent.h"
#include "Minty/Components/M_DirtyComponent.h"
#include "Minty/Components/M_EnabledComponent.h"
#include "Minty/Components/M_NameComponent.h"
#include "Minty/Components/M_RelationshipComponent.h"
#include "Minty/Components/M_TagComponent.h"
#include "Minty/Components/M_TransformComponent.h"

// CORE
#include "Minty/Core/M_Application.h"
#include "Minty/Core/M_Base.h"
#include "Minty/Core/M_Constants.h"
#include "Minty/Core/M_Macros.h"
#include "Minty/Core/M_Engine.h"
#include "Minty/Core/M_Window.h"

// ENTITIES
#include "Minty/Entities/M_Entity.h"
#include "Minty/Entities/M_EntityRegistry.h"

// EVENTS
#include "Minty/Events/M_Event.h"

// FILES
#include "Minty/Files/M_File.h"
#include "Minty/Files/M_PhysicalFile.h"
#include "Minty/Files/M_VirtualFile.h"
#include "Minty/Files/M_Wrap.h"
#include "Minty/Files/M_Wrapper.h"

// INPUT
#include "Minty/Input/M_CursorMode.h"
#include "Minty/Input/M_Gamepad.h"
#include "Minty/Input/M_Key.h"
#include "Minty/Input/M_KeyAction.h"
#include "Minty/Input/M_KeyModifiers.h"
#include "Minty/Input/M_MouseButton.h"

// LAYERS
#include "Minty/Layers/M_Layer.h"
#include "Minty/Layers/M_DefaultLayer.h"

// LIBRARIES
#ifdef MINTY_IMGUI
#include "M_ImGui.h"
#endif

// MATH
#include "Minty/Math/M_Math.h"

// MULTITHREADING
#include "Minty/Multithreading/M_Task.h"

// RENDERING
#include "Minty/Rendering/M_Buffer.h"
#include "Minty/Rendering/M_Builtin.h"
#include "Minty/Rendering/M_Camera.h"
#include "Minty/Rendering/M_CameraComponent.h"
#include "Minty/Rendering/M_CoordinateMode.h"
#include "Minty/Rendering/M_DescriptorSet.h"
#include "Minty/Rendering/M_DrawCallObjectInfo.h"
#include "Minty/Rendering/M_Material.h"
#include "Minty/Rendering/M_MaterialTemplate.h"
#include "Minty/Rendering/M_Mesh.h"
#include "Minty/Rendering/M_MeshComponent.h"
#include "Minty/Rendering/M_PushConstantInfo.h"
#include "Minty/Rendering/M_RenderableComponent.h"
#include "Minty/Rendering/M_RenderEngine.h"
#include "Minty/Rendering/M_RenderObject.h"
#include "Minty/Rendering/M_RenderSystem.h"
#include "Minty/Rendering/M_Shader.h"
#include "Minty/Rendering/M_ShaderPass.h"
#include "Minty/Rendering/M_Sprite.h"
#include "Minty/Rendering/M_SpriteComponent.h"
#include "Minty/Rendering/M_SpritePushData.h"
#include "Minty/Rendering/M_Texture.h"
#include "Minty/Rendering/M_TextureAtlas.h"
#include "Minty/Rendering/M_UniformConstantInfo.h"
#include "Minty/Rendering/M_Viewport.h"

// SCENES
#include "Minty/Scenes/M_Scene.h"
#include "Minty/Scenes/M_SceneManager.h"
#include "Minty/Scenes/M_SceneObject.h"

// SCRIPTING
#include "Minty/Scripting/M_Accessibility.h"
#include "Minty/Scripting/M_ScriptArguments.h"
#include "Minty/Scripting/M_ScriptAssembly.h"
#include "Minty/Scripting/M_ScriptClass.h"
#include "Minty/Scripting/M_ScriptComponent.h"
#include "Minty/Scripting/M_ScriptEngine.h"
#include "Minty/Scripting/M_ScriptObject.h"
#include "Minty/Scripting/M_ScriptSystem.h"

// SERIALIZATION
#include "Minty/Serialization/M_ISerializable.h"
#include "Minty/Serialization/M_Reader.h"
#include "Minty/Serialization/M_SerializationData.h"
#include "Minty/Serialization/M_Writer.h"

// SYSTEMS
#include "Minty/Systems/M_System.h"
#include "Minty/Systems/M_SystemRegistry.h"

// TOOLS
#include "Minty/Tools/M_Compression.h"
#include "Minty/Tools/M_Console.h"
#include "Minty/Tools/M_Debug.h"
#include "Minty/Tools/M_Encoding.h"
#include "Minty/Tools/M_Operations.h"
#include "Minty/Tools/M_Parse.h"
#include "Minty/Tools/M_Text.h"

// TYPES
#include "Minty/Types/M_Color.h"
#include "Minty/Types/M_CommandLineParser.h"
#include "Minty/Types/M_Dynamic.h"
#include "Minty/Types/M_FiniteStateMachine.h"
#include "Minty/Types/M_Logger.h"
#include "Minty/Types/M_Matrix.h"
#include "Minty/Types/M_Node.h"
#include "Minty/Types/M_Object.h"
#include "Minty/Types/M_Quaternion.h"
#include "Minty/Types/M_Rect.h"
#include "Minty/Types/M_RectF.h"
#include "Minty/Types/M_Stopwatch.h"
#include "Minty/Types/M_Time.h"
#include "Minty/Types/M_TypeRegister.h"
#include "Minty/Types/M_Types.h"
#include "Minty/Types/M_UUID.h"
#include "Minty/Types/M_Vector.h"

// UI
#include "Minty/UI/M_CanvasComponent.h"
#include "Minty/UI/M_UIPushData.h"
#include "Minty/UI/M_UISystem.h"
#include "Minty/UI/M_UITransformComponent.h"