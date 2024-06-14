#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#pragma comment(lib, "detours.lib")
#include "detours.h"
#include "wil/resource.h"
#include "wil/token_helpers.h"
#include <chrono>
#include <thread>