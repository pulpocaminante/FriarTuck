// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "hook.h"
#include "detours.h"

static DWORD WINAPI FriarTuck(HMODULE hModule)
{
#ifdef _DEBUG
    AllocConsole();
    FILE* newstdin = nullptr;
    FILE* newstdout = nullptr;
    FILE* newstderr = nullptr;

    freopen_s(&newstdin, "CONIN$", "r", stdin);
    freopen_s(&newstdout, "CONOUT$", "w", stdout);
    freopen_s(&newstderr, "CONOUT$", "w", stderr);
    std::cout << "Friar tuck is waiting" << std::endl;
#endif

    // Define event handles
    ULONG uTimer = 0;

     // Robin's Heartbeat
    const HANDLE hHeartbeatExe = CreateEvent(NULL, FALSE, FALSE, TEXT("RobinsAlive"));
    if (!hHeartbeatExe) std::cout << "[!] Failed to create event!" << std::endl;

    // Friar's Heartbeat
    const HANDLE hHeartbeatDLL = CreateEvent(NULL, FALSE, FALSE, TEXT("FriarsAlive"));
    if (!hHeartbeatDLL) std::cout << "[!] Failed to create event!" << std::endl;

    // Need EXE to be saved
    const HANDLE hExeTerminated = CreateEvent(NULL, FALSE, FALSE, TEXT("HelpMeFriarTuck"));
    if (!hExeTerminated) std::cout << "[!] Failed to create event!" << std::endl;

    // Need to save DLL
    const HANDLE hDLLDetached = CreateEvent(NULL, FALSE, FALSE, TEXT("HelpMeRobinHood"));
    if (!hDLLDetached) std::cout << "[!] Failed to create event!" << std::endl;

    // EXE saved confirmation
    const HANDLE hExeSaved = CreateEvent(NULL, FALSE, FALSE, TEXT("RobinHoodSaved"));
    if (!hExeSaved) std::cout << "[!] Failed to create event!" << std::endl;

    // DLL saved confirmation
    const HANDLE hDLLSaved = CreateEvent(NULL, FALSE, FALSE, TEXT("FriarTuckSaved"));
    if (!hDLLSaved) std::cout << "[!] Failed to create event!" << std::endl;

    // Die
    const HANDLE hKillSelf = CreateEvent(NULL, FALSE, FALSE, TEXT("killyourself"));
    if (!hKillSelf) std::cout << "[!] Failed to create event!" << std::endl;


    const HANDLE hAllHandles[] = { hHeartbeatExe, hExeTerminated, hDLLDetached, hExeSaved, hDLLSaved, hKillSelf };


    while (true)
    {
        switch (MsgWaitForMultipleObjects(_countof(hAllHandles), hAllHandles, FALSE, 500, QS_ALLEVENTS))
        {
        case WAIT_OBJECT_0: // Heartbeat
            std::cout << "%[Robin] I'm doin' dandy!" << std::endl;
            ResetEvent(hHeartbeatExe);
            break;
        case WAIT_OBJECT_0 + 1: // EXE terminated
            std::cout << "%[Robin] Friar Tuck... I am under attack! Pray save me!" << std::endl;
            break;
        case WAIT_OBJECT_0 + 2: // DLL detached
            std::cout << "%[~Friar] Robin Hood... I am under attack!  Pray save me!" << std::endl;
            break;
        case WAIT_OBJECT_0 + 3: // EXE restarted
            std::cout << "%[Robin] Thank you, my good fellow!" << std::endl;
            break;
        case WAIT_OBJECT_0 + 4: // DLL reinjected
            std::cout << "%[~Friar] Thank you, my good fellow!" << std::endl;
            break;
        case WAIT_OBJECT_0 + 5:
            std::cout << "[!] Offin' myself!" << std::endl;
            FreeLibraryAndExitThread(hModule, 0);
        case WAIT_TIMEOUT:
            SetEvent(hHeartbeatDLL);
            break;
        case WAIT_FAILED:
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        uTimer += 500;
        if (uTimer > 1000)
        {
            std::cout << "Fear not, friend Robin! I shall rout the Sheriff of Nottingham's men!" << std::endl;
        }
    }
    HANDLE hHelp = CreateEvent(NULL, FALSE, TRUE, TEXT("HelpMeRobinHood"));
    SetEvent(hHelp);
    return 0;
}

static DWORD WINAPI MainThread(HMODULE hModule)
{
    HANDLE hFriarTuck = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FriarTuck, hModule, 0, nullptr);
    HANDLE hHookMain = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)hook::hookMain, hModule, 0, nullptr);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr));
        return TRUE;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

