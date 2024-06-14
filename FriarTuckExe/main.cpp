#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>


int SaveRobin()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcess(L"C:\\Users\\amnesia\\source\\repos\\UltronUI\\x64\\Debug\\UltronUI.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) != 0)
    {
        std::cout << "IN [~Robin] Thank you, my good fellow!" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "[-] Failed to start Robin! " << GetLastError() << std::endl;
    }
    return 0;
}

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
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
        switch (MsgWaitForMultipleObjects(_countof(hAllHandles), hAllHandles, FALSE, 50, QS_ALLEVENTS))
        {
        case WAIT_OBJECT_0: // Heartbeat
            std::cout << "IN  [Robin] Tally ho!" << std::endl;
            uTimer = 0;
            ResetEvent(hHeartbeatExe);
            break;
        case WAIT_OBJECT_0 + 1: // EXE terminated
            std::cout << "IN  [Robin] Friar Tuck... I am under attack! Pray save me!" << std::endl;
            SaveRobin();
            uTimer = 0;
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
            return 0;
        case WAIT_TIMEOUT:
            SetEvent(hHeartbeatDLL);
            break;
        case WAIT_FAILED:
            break;
        }
        uTimer += 50;
        if (uTimer > 100)
        {
            std::cout << "IN  [Robin] Friar Tuck... I am under attack! Pray save me!" << std::endl;
            std::cout << "OUT [Friar] Fear not, friend Robin! I shall rout the Sheriff of Nottingham's men!" << std::endl;
            SaveRobin();
            uTimer = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
    HANDLE hHelp = CreateEvent(NULL, FALSE, TRUE, TEXT("HelpMeRobinHood"));
    SetEvent(hHelp);
    return 0;
}
