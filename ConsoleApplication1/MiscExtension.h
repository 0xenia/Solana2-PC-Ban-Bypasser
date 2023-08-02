#pragma once
class MiscExtension
{
public:
    static std::string GenerateRandomMac()
    {
        const std::string charset = "0123456789ABCDEF";
        const int charsetSize = charset.size();
        srand(static_cast<unsigned int>(time(0)));

        std::string result;
        for (int i = 0; i < 17; i++) {
            if (i % 3 == 2 && i != 16) {
                result.push_back('-');
            }
            else
            {
                result.push_back(charset[rand() % charsetSize]);
            }
        }

        return result;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    static BOOL EnableDebugPrivilege(BOOL bEnable)
    {
        HANDLE hToken = nullptr;
        LUID luid;

        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) return FALSE;
        if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) return FALSE;

        TOKEN_PRIVILEGES tokenPriv;
        tokenPriv.PrivilegeCount = 1;
        tokenPriv.Privileges[0].Luid = luid;
        tokenPriv.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

        if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) return FALSE;

        return TRUE;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    static bool IsRunAsAdmin()
    {
        BOOL isAdmin = FALSE;
        PSID adminGroup;
        SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

        if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0, &adminGroup))
        {
            if (!CheckTokenMembership(NULL, adminGroup, &isAdmin))
            {
                isAdmin = FALSE;
            }
            FreeSid(adminGroup);
        }

        return isAdmin != FALSE;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    static bool isTabKeyPressed()
    {
        return GetAsyncKeyState(VK_INSERT) & 0x8000;
    }
    ////////////////////////////////////////////////////////////////////////////////////
};
