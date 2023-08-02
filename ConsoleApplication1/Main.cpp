#include <iostream>
#include <Windows.h>
#include <string>
#include <Psapi.h>
#include "Process.h"
#include "MiscExtension.h"
#include "proc.h"

char Buffer[17];
char NewBuffer[17];

int main() 
{
    if (!MiscExtension::IsRunAsAdmin())
    {
        printf("Please run as administrator to be able to use it.\n");
        system("pause");
        return 0;
    }


    MiscExtension::EnableDebugPrivilege(true);

    std::string RandomMac = MiscExtension::GenerateRandomMac();

    for (int i = 0; i < 17; i++) 
    {
        NewBuffer[i] = RandomMac[i];
    }

    printf("Press Insert after u click the game window.\n");


    while (Process::PID == 0) 
    {
        if (MiscExtension::isKeyPressed()) 
        {
            system("cls");
            HWND windowHandle = GetForegroundWindow();
            GetWindowThreadProcessId(windowHandle, &Process::PID);

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, Process::PID);
            if (hProcess)
            {
                if (GetModuleBaseName(hProcess, nullptr, Process::ProcessName, MAX_PATH))
                {
                    printf("pid : %d\n", Process::PID);

                    uintptr_t moduleBase = GetModuleBaseAddress(Process::PID, Process::ProcessName);
                    printf("moduleBase: %p\n", moduleBase);
                    if (hProcess == NULL) 
                    {
                        std::cout << "process not exists" << std::endl;
                        return 1;
                    }

                    uintptr_t macptr = moduleBase + 0x69EC78;
                    uintptr_t macaddr = FindDMAAddy(hProcess, macptr, { 0x0 });

                    ReadProcessMemory(hProcess, (BYTE*)macaddr, Buffer, sizeof(Buffer), nullptr);
                    printf("Old MAC: % s\n", Buffer);
                    if (Buffer == "")
                    {
                        printf("Probably pointer is wrong.\n");
                        system("pause");
                        return 0;
                    }

                    WriteProcessMemory(hProcess, (BYTE*)macaddr, NewBuffer, sizeof(NewBuffer), nullptr);
                    printf("New MAC: % s\n", NewBuffer);

                    printf("Done.\n");
                    CloseHandle(hProcess);
                    system("pause");
                    break;
                }
                else 
                {
                    std::cerr << "Failed to get process name." << std::endl;
                }
                CloseHandle(hProcess);
            }
            else 
            {
                std::cerr << "Failed to open process." << std::endl;
            }

        }

        Sleep(100);
    }

    return 0;
}

