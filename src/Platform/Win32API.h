#pragma once

// Force Unicode Character Set
// UNICODE targets native Windows SDK header files (e.g. SetWindowText -> SetWindowTextW)
#ifndef UNICODE
#define UNICODE
#endif
// _UNICODE targets the standard C Runtime / CRT headers (e.g. _tcslen -> wcslen)
#ifndef _UNICODE
#define _UNICODE
#endif

// Target Windows 10 or later
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#ifndef WINVER
#define WINVER 0x0A00
#endif

// Strip Windows Bloat
#define WIN32_LEAN_AND_MEAN // Excludes rarely-used services (cryptography, DDE, etc.)
#define NOMINMAX            // Prevents Windows from defining min/max macros (breaks std::min/max)
#define NODRAWTEXT          // Removes legacy GDI DrawText macros
#define NOGDI               // Removes legacy Graphics Device Interface (DirectX handles this)
#define NOBITMAP            // Removes GDI Bitmap types
#define NOMCX               // Excludes Modem Configuration Extensions
#define NOSERVICE           // Excludes Windows Service Code
#define NOHELP              // Excludes legacy Help engine code

// Include Core Windows Headers
#include <Windows.h>

#ifdef APP_ENTRY_POINT
#define main() wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
#endif