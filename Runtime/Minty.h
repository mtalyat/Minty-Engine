#pragma once

/*

Minty Runtime header file.

All of the following files included will allow a user to include just this file, which will allow
access to the entire engine without needing include each file.

*/

// LIBRARIES
#include "vulkan.h"
#include "glm.hpp"

// ANIMATION
#include "M_Animation.h"
#include "M_AnimationBuilder.h"
#include "M_AnimationSystem.h"
#include "M_Animator.h"
#include "M_AnimatorBuilder.h"
#include "M_AnimatorComponent.h"

// AUDIO
#include "M_Audio.h"
#include "M_AudioClip.h"
#include "M_AudioEngine.h"
#include "M_AudioListenerComponent.h"
#include "M_AudioSourceComponent.h"
#include "M_AudioSystem.h"

// BUILTIN
#include "M_Builtin.h"

// COMPONENTS
#include "M_Component.h"

#include "M_CameraComponent.h"
#include "M_DirtyComponent.h"
#include "M_MeshComponent.h"
#include "M_NameComponent.h"
#include "M_RelationshipComponent.h"
#include "M_UITransformComponent.h"

// ENTITIES
#include "M_EntityRegistry.h"

// FILES
#include "M_Asset.h"
#include "M_File.h"
#include "M_PhysicalFile.h"
#include "M_VirtualFile.h"
#include "M_Wrap.h"
#include "M_Wrapper.h"

// INPUT
#include "M_CursorMode.h"
#include "M_InputMap.h"
#include "M_Key.h"
#include "M_KeyAction.h"
#include "M_KeyModifiers.h"
#include "M_MouseButton.h"

// RENDERING
#include "M_CoordinateMode.h"
#include "M_Material.h"
#include "M_MaterialTemplate.h"
#include "M_Mesh.h"
#include "M_RenderableComponent.h"
#include "M_RenderEngine.h"
#include "M_Buffer.h"
#include "M_DescriptorSet.h"
#include "M_DrawCallObjectInfo.h"
#include "M_MaterialBuilder.h"
#include "M_MaterialTemplateBuilder.h"
#include "M_RenderObject.h"
#include "M_PushConstantInfo.h"
#include "M_RendererBuilder.h"
#include "M_ShaderBuilder.h"
#include "M_ShaderPassBuilder.h"
#include "M_SpriteBuilder.h"
#include "M_TextureAtlasBuilder.h"
#include "M_TextureBuilder.h"
#include "M_UniformConstantInfo.h"
#include "M_RenderSystem.h"
#include "M_Shader.h"
#include "M_ShaderPass.h"
#include "M_Sprite.h"
#include "M_SpriteComponent.h"
#include "M_Texture.h"
#include "M_TextureAtlas.h"

// SCENES
#include "M_Scene.h"
#include "M_SceneManager.h"

// SERIALIZATION
#include "M_ISerializable.h"
#include "M_Node.h"
#include "M_Reader.h"
#include "M_SerializationData.h"
#include "M_Writer.h"

// SYSTEMS
#include "M_System.h"
#include "M_SystemRegistry.h"
#include "M_UISystem.h"

// TRANSFORM
#include "M_TransformComponent.h"

// GENERAL
#include "M_Base.h"
#include "M_Color.h"
#include "M_CommandLineParser.h"
#include "M_Compression.h"
#include "M_Console.h"
#include "M_Constants.h"
#include "M_Dynamic.h"
#include "M_Encoding.h"
#include "M_Engine.h"
#include "M_Error.h"
#include "M_Event.h"
#include "M_FiniteStateMachine.h"
#include "M_Info.h"
#include "M_Math.h"
#include "M_Matrix.h"
#include "M_Object.h"
#include "M_Operations.h"
#include "M_Parse.h"
#include "M_Quaternion.h"
#include "M_Rect.h"
#include "M_RectF.h"
#include "M_Register.h"
#include "M_Stopwatch.h"
#include "M_Text.h"
#include "M_Types.h"
#include "M_Vector.h"
#include "M_Window.h"