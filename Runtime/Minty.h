#pragma once

/*

Minty Runtime header file.

All of the following files included will allow a user to include just this file, which will allow
access to the entire engine without needing to know the names for each file.

ONLY include files that somebody writing a game would use.

*/

#include "engine/M_Color.h"
#include "engine/M_CommandLineParser.h"
#include "engine/M_Console.h"
#include "engine/M_Engine.h"
#include "engine/M_Event.h"
#include "engine/M_File.h"
#include "engine/M_InputMap.h"
#include "engine/M_Key.h"
#include "engine/M_KeyAction.h"
#include "engine/M_KeyModifiers.h"
#include "engine/M_Material.h"
#include "engine/M_Math.h"
#include "engine/M_Mesh.h"
#include "engine/M_MouseButton.h"
#include "engine/M_Object.h"
#include "engine/M_Project.h"
#include "engine/M_Rect.h"
#include "engine/M_RectF.h"
#include "engine/M_Runtime.h"
#include "engine/M_Stopwatch.h"
#include "engine/M_Texture.h"
#include "engine/M_Types.h"
#include "engine/M_Vector2.h"
#include "engine/M_Vector2Int.h"
#include "engine/M_Vector3.h"
#include "engine/M_Vector3Int.h"
#include "engine/M_Window.h"