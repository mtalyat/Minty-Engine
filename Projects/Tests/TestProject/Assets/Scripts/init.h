#pragma once

#include <Minty.h>
#include "types.h"

// called when the engine is initialized
int init(minty::Runtime& runtime);

// called when the engine is destroyed
int destroy(minty::Runtime& runtime);