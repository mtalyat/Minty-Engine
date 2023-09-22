#pragma once

/*

Minty Runtime header file.

All of the following files included will allow a user to include just this file, which will allow
access to the entire engine without needing include each file.

*/

// COMPONENTS
#include "M_Component.h"

#include "M_NameComponent.h"
#include "M_TransformComponent.h"

// ENTITIES
#include "M_EntityRegistry.h"

// SCENES
#include "M_Scene.h"
#include "M_SceneManager.h"

// SERIALIZATION
#include "M_ISerializable.h"
#include "M_SerializedNode.h"
#include "M_Serializer.h"

// SYSTEMS
#include "M_System.h"
#include "M_SystemRegistry.h"

#include "M_RendererSystem.h"

// GENERAL
#include "M_Color.h"
#include "M_CommandLineParser.h"
#include "M_Console.h"
#include "M_Engine.h"
#include "M_Event.h"
#include "M_File.h"
#include "M_InputMap.h"
#include "M_Key.h"
#include "M_KeyAction.h"
#include "M_KeyModifiers.h"
#include "M_Material.h"
#include "M_Math.h"
#include "M_Mesh.h"
#include "M_MouseButton.h"
#include "M_Object.h"
#include "M_Project.h"
#include "M_Rect.h"
#include "M_RectF.h"
#include "M_Renderer.h"
#include "M_Runtime.h"
#include "M_Stopwatch.h"
#include "M_Texture.h"
#include "M_Types.h"
#include "M_Vector2.h"
#include "M_Vector2Int.h"
#include "M_Vector3.h"
#include "M_Vector3Int.h"
#include "M_Window.h"