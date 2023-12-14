#pragma once

#include <Minty.h>
#include "types.h"

// called when the engine is initialized
void init(minty::Runtime& runtime);

// called when the engine is destroyed
void destroy(minty::Runtime& runtime);