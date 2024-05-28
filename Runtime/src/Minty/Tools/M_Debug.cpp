#include "pch.h"
#include "Minty/Tools/M_Debug.h"

#include "Minty/Tools/M_Console.h"
#include "Minty/Types/M_Logger.h"

#ifdef MINTY_WINDOWS
#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
#pragma comment(lib, "Dbghelp.lib")
#endif

using namespace Minty;

static Logger _logger("Log.txt", true);

void Minty::Debug::log(String const& text)
{
	Console::log(text);
	_logger.log(text);
}

void Minty::Debug::log_warning(String const& text)
{
	Console::warn(text);
	_logger.log_warning(text);
}

void Minty::Debug::log_error(String const& text)
{
	Console::error(text);
	_logger.log_error(text);
}

void Minty::Debug::log_info(String const& text)
{
	Console::info(text);
	_logger.log_info(text);
}

void Minty::Debug::log_todo(String const& text)
{
	Console::todo(text);
	_logger.log_todo(text);
}

void Minty::Debug::log_stack_trace()
{
#ifdef MINTY_WINDOWS
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    CONTEXT context;
    memset(&context, 0, sizeof(CONTEXT));
    context.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&context);

    SymInitialize(process, NULL, TRUE);

    STACKFRAME64 stackframe;
    memset(&stackframe, 0, sizeof(STACKFRAME64));
    DWORD machineType;

#ifdef _M_IX86
    machineType = IMAGE_FILE_MACHINE_I386;
    stackframe.AddrPC.Offset = context.Eip;
    stackframe.AddrPC.Mode = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.Ebp;
    stackframe.AddrFrame.Mode = AddrModeFlat;
    stackframe.AddrStack.Offset = context.Esp;
    stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
    machineType = IMAGE_FILE_MACHINE_AMD64;
    stackframe.AddrPC.Offset = context.Rip;
    stackframe.AddrPC.Mode = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.Rsp;
    stackframe.AddrFrame.Mode = AddrModeFlat;
    stackframe.AddrStack.Offset = context.Rsp;
    stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
    machineType = IMAGE_FILE_MACHINE_IA64;
    stackframe.AddrPC.Offset = context.StIIP;
    stackframe.AddrPC.Mode = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.IntSp;
    stackframe.AddrFrame.Mode = AddrModeFlat;
    stackframe.AddrBStore.Offset = context.RsBSP;
    stackframe.AddrBStore.Mode = AddrModeFlat;
    stackframe.AddrStack.Offset = context.IntSp;
    stackframe.AddrStack.Mode = AddrModeFlat;
#endif

    for (int i = 0; i < 25; i++) {
        if (!StackWalk64(machineType, process, thread, &stackframe, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
            break;
        }

        if (stackframe.AddrPC.Offset == 0) {
            break;
        }

        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64 displacement = 0;
        if (SymFromAddr(process, stackframe.AddrPC.Offset, &displacement, symbol)) {
            printf("%s\n", symbol->Name);
        }
        else {
            printf("0x%llx\n", stackframe.AddrPC.Offset);
        }
    }

    SymCleanup(process);
#endif
}
