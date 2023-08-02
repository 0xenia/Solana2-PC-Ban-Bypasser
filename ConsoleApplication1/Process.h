#pragma once
class Process
{
public:
    static DWORD PID;
    static wchar_t ProcessName[255];
};

DWORD Process::PID = NULL;
wchar_t Process::ProcessName[255] = L"";

    
