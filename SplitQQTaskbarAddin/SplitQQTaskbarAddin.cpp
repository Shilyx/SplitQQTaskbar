/*
 *  @file  : SplitQQTaskbarAddin.cpp
 *  @author: shilyx
 *  @date  : 2023-07-14 10:02:22.118
 *  @note  : Generated by SlxTemplates build:20230411
 */

#include "SplitQQTaskbarAddin.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <Shobjidl.h>

HINSTANCE g_hinstDll = NULL;

static void _SetCurrentProcessExplicitAppUserModelID(LPWSTR lpAppID) {
    HMODULE hModule = GetModuleHandleW(L"shell32.dll");
    if (hModule == NULL) {
        hModule = LoadLibraryW(L"shell32.dll");
    }
    if (hModule == NULL) {
        return;
    }

    typedef HRESULT (WINAPI *FuncType)(LPWSTR);
    FuncType func = NULL;

    (PROC &)func = GetProcAddress(hModule, "SetCurrentProcessExplicitAppUserModelID");
    if (func == NULL) {
        return;
    }

    func(lpAppID);
}

int APIENTRY DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        g_hinstDll = hinstDll;
        DisableThreadLibraryCalls(hinstDll);

        WCHAR szExePath[MAX_PATH] = L"";
        GetModuleFileNameW(GetModuleHandleW(NULL), szExePath, RTL_NUMBER_OF(szExePath));
        if (lstrcmpiW(PathFindFileNameW(szExePath), L"qq.exe") == 0) {
            WCHAR szAppID[1024] = L"";
            wnsprintf(szAppID, RTL_NUMBER_OF(szAppID), L"qappid_%lu", GetCurrentProcessId());
            _SetCurrentProcessExplicitAppUserModelID(szAppID);
        }
    } else if (dwReason == DLL_PROCESS_DETACH) {

    } else {
        // we should not be here
    }

    return TRUE;
}

void WINAPI T(HWND hwndStub, HINSTANCE hInstance, LPCSTR lpCmdLine, int nShowCmd) {
    MessageBoxA(hwndStub, lpCmdLine, "rundll32", MB_ICONINFORMATION | MB_TOPMOST);
}

// callback of WH_GETMESSAGE
static LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam) {
    // wParam is PM_NOREMOVE: An application called the PeekMessage function, specifying the PM_NOREMOVE flag.
    // wParam is PM_REMOVE: An application called GetMessage, or it called the PeekMessage function, specifying the PM_REMOVE flag.
    BOOL bRemove = PM_REMOVE == wParam;
    // lParam is a pointer to an MSG structure that contains details about the message. 
    LPMSG lpMsg = (LPMSG)lParam;

    if (code < 0) {
        return CallNextHookEx(NULL, code, wParam, lParam);
    }

    // 
    return CallNextHookEx(NULL, code, wParam, lParam);
}

// 发起消息钩子
HHOOK WINAPI SetHuuk() {
    return SetWindowsHookExW(WH_GETMESSAGE, GetMsgProc, g_hinstDll, 0);
}

// 取消消息钩子
BOOL WINAPI UnhuukHuuk(HHOOK hHook) {
    return UnhookWindowsHookEx(hHook);
}
