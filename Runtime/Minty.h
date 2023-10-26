#pragma once

/*

Minty Runtime header file.

All of the following files included will allow a user to include just this file, which will allow
access to the entire engine without needing include each file.

*/

// LIBRARIES
#include <vulkan/vulkan.h>
#include "glm.hpp"

// BASIC
#include "M_Basic.h"
#include "M_Basic_Vertex.h"

// COMPONENTS
#include "M_Component.h"

#include "M_CameraComponent.h"
#include "M_DirtyTag.h"
#include "M_MeshComponent.h"
#include "M_NameComponent.h"
#include "M_RelationshipComponent.h"
#include "M_RenderableComponent.h"
#include "M_SpriteComponent.h"
#include "M_TransformComponent.h"
#include "M_UIOnMouseEnterComponent.h"
#include "M_UITransformComponent.h"

// ENTITIES
#include "M_EntityRegistry.h"

// INPUT
#include "M_InputMap.h"
#include "M_Key.h"
#include "M_KeyAction.h"
#include "M_KeyModifiers.h"
#include "M_MouseButton.h"

// RENDERING
#include "M_Material.h"
#include "M_Mesh.h"
#include "M_Renderer.h"
#include "M_Rendering_MaterialBuilder.h"
#include "M_Rendering_Object.h"
#include "M_Rendering_RendererBuilder.h"
#include "M_Rendering_TextureBuilder.h"
#include "M_Rendering_ShaderBuilder.h"
#include "M_Shader.h"
#include "M_Texture.h"

// SCENES
#include "M_Scene.h"
#include "M_SceneManager.h"

// SERIALIZATION
#include "M_ISerializable.h"
#include "M_Reader.h"
#include "M_SerializedNode.h"
#include "M_Writer.h"

// SYSTEMS
#include "M_System.h"
#include "M_SystemRegistry.h"
#include "M_UISystem.h"


// GENERAL
#include "M_Color.h"
#include "M_CommandLineParser.h"
#include "M_Console.h"
#include "M_Constants.h"
#include "M_Encoding.h"
#include "M_Engine.h"
#include "M_Event.h"
#include "M_File.h"
#include "M_Info.h"
#include "M_Math.h"
#include "M_Matrix.h"
#include "M_Object.h"
#include "M_Parse.h"
#include "M_Quaternion.h"
#include "M_Rect.h"
#include "M_RectF.h"
#include "M_Runtime.h"
#include "M_Stopwatch.h"
#include "M_Types.h"
#include "M_Vector.h"
#include "M_Window.h"