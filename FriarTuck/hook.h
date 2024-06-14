#pragma once
#include "pch.h"
#include <processthreadsapi.h>
#include <iostream>

VOID CALLBACK hookedExit(UINT exitCode);

class hook 
{
public:
	static DWORD WINAPI hookMain(HANDLE hModule);
	BOOL unloadAndExit();
};