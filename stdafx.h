// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <shellapi.h>


// TODO: reference additional headers your program requires here
#include "glm\glm.hpp"
#include <math.h>
#include <time.h>

#include "gas_model.h"
#include "draw.h"
#include "communication.h"
#include "common.h"
#include "config.h"