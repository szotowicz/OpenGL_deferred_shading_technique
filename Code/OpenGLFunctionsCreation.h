#ifndef _OPENGL_FUNCTIONS_CREATION_HEADER_
#define _OPENGL_FUNCTIONS_CREATION_HEADER_

#include <Windows.h>

bool LoadOpenGLFunctions( const HDC i_DeviceContext );

static bool CheckExtension( const char*, const char* );
static void* GetFunctionAddress( const HMODULE, const char* );

#endif // _OPENGL_FUNCTIONS_CREATION_HEADER_