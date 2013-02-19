// kratky subor s funkciami na upravu stringov
#include "stdafx.h"
#include "StringHelper.h"

std::string MarshalString (System::String ^ s)
{
	using namespace System::Runtime::InteropServices;
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	std::string result = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
	return result;
}

std::string str_format (const std::string fmt, ...)
{
	int size=100;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size=n+1;
        else
            size*=2;
    }
}