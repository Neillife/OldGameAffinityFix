#include "pch.h"

DWORD_PTR GetPCoreAffinityMask()
{
    DWORD length = 0;
    GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &length);

    std::vector<BYTE> buffer(length);
    auto info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data());

    if (!GetLogicalProcessorInformationEx(RelationProcessorCore, info, &length))
        return 0;

    DWORD_PTR mask = 0;
    BYTE* ptr = buffer.data();
    BYTE* end = ptr + length;

    while (ptr < end)
    {
        auto entry = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(ptr);

        if (entry->Relationship == RelationProcessorCore)
        {
            const auto& core = entry->Processor;

            // EfficiencyClass == 0 is P-core (or AMD core)
            if (core.EfficiencyClass == 0)
            {
                for (WORD i = 0; i < core.GroupCount; i++)
                {
                    mask |= core.GroupMask[i].Mask;
                }
            }
        }

        ptr += entry->Size;
    }

    return mask;
}

void Fix() {
    HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());

    if (!hProc) {
        MessageBox(NULL, L"Unable to start the handle for the current process. Please check if the program has sufficient permissions.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    DWORD_PTR mask = GetPCoreAffinityMask();

    if (mask != 0)
    {
        SetProcessAffinityMask(hProc, mask);
    }

    SetPriorityClass(hProc, HIGH_PRIORITY_CLASS);

    CloseHandle(hProc);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        Fix();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
