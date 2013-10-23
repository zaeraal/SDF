// kratky subor s funkciami na upravu stringov
#pragma once

std::string MarshalString (System::String ^ s);
//std::string str_format (const std::string fmt, ...);
char* oclLoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength);