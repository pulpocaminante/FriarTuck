#include "pch.h"
#include "hook.h"

// Function addresses for hooking
static void(WINAPI* originalExitProcess)(UINT uExitCode) = ExitProcess;
static BOOL(WINAPI* originalGetMessageW)(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax) = GetMessageW;
static BOOL(WINAPI* originalPeekMessageW)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) = PeekMessageW;
static BOOL(WINAPI* originalGetMessageA)(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax) = GetMessageA;
static BOOL(WINAPI* originalPeekMessageA)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) = PeekMessageA;

VOID CALLBACK hookedExit(UINT exitCode)
{
	const HANDLE hDLLDetached = CreateEvent(NULL, FALSE, FALSE, TEXT("HelpMeRobinHood"));
	SetEvent(hDLLDetached);
	originalExitProcess(exitCode);
}

BOOL CALLBACK hookedGetMessageW(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax)
{
	HANDLE hDLLDetached;
	switch (lpMsg->message)
	{
	case WM_CLOSE:
		hDLLDetached = CreateEvent(NULL, FALSE, FALSE, TEXT("HelpMeRobinHood"));
		SetEvent(hDLLDetached);
		break;
	}
	return originalGetMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

BOOL CALLBACK hookedPeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	HANDLE hDLLDetached;
	switch (lpMsg->message)
	{
	case WM_CLOSE:
		hDLLDetached = CreateEvent(NULL, FALSE, FALSE, TEXT("HelpMeRobinHood"));
		SetEvent(hDLLDetached);
		break;
	}
	return originalPeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

BOOL CALLBACK hookedGetMessageA(LPMSG lpMsg, HWND  hWnd, UINT  wMsgFilterMin, UINT  wMsgFilterMax)
{
	HANDLE hDLLDetached;
	switch (lpMsg->message)
	{
	case WM_CLOSE:
		hDLLDetached = CreateEvent(NULL, FALSE, FALSE, TEXT("HelpMeRobinHood"));
		SetEvent(hDLLDetached);
		break;
	}
	return originalGetMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
}

BOOL CALLBACK hookedPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg)
{
	HANDLE hDLLDetached;
	switch (lpMsg->message)
	{
	case WM_CLOSE:
		hDLLDetached = CreateEvent(NULL, FALSE, FALSE, TEXT("HelpMeRobinHood"));
		SetEvent(hDLLDetached);
		break;
	}
	return originalPeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

DWORD WINAPI hook::hookMain(HANDLE hModule)
{
#ifdef _DEBUG
	AllocConsole();
	FILE* newstdin = nullptr;
	FILE* newstdout = nullptr;
	FILE* newstderr = nullptr;

	freopen_s(&newstdin, "CONIN$", "r", stdin);
	freopen_s(&newstdout, "CONOUT$", "w", stdout);
	freopen_s(&newstderr, "CONOUT$", "w", stderr);
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)originalExitProcess, hookedExit);
	DetourAttach(&(PVOID&)originalPeekMessageW, hookedPeekMessageW);
	DetourAttach(&(PVOID&)originalGetMessageW, hookedGetMessageW);
	DetourAttach(&(PVOID&)originalPeekMessageA, hookedPeekMessageA);
	DetourAttach(&(PVOID&)originalGetMessageA, hookedGetMessageA);
	LONG err = DetourTransactionCommit();
#ifdef _DEBUG
	if (err == NO_ERROR) std::cout << "[Hook] All hooks installed" << std::endl;
	else if (err == ERROR_INVALID_DATA) std::cout << "[Hook] Hooks failed: Target function was changed by third party between steps of the transaction " << err << std::endl;
	else if (err == ERROR_INVALID_OPERATION) std::cout << "[Hook] Hooks failed: No pending transaction exists" << std::endl;
	else std::cout << "[Hook] Hooks failed: " << err << std::endl;
#endif
	return 0;
}
