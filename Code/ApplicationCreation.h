#ifndef _APPLICATION_CREATION_HEADER_
#define _APPLICATION_CREATION_HEADER_

#include <Windows.h>
#include <GL\glcorearb.h>
#include <GL\wglext.h>
#include "ErrorMessage.h"
#include "OpenGLCreation.h"

bool CreateApplication( const HINSTANCE i_ApplicationInstance,
                        const char*     i_ApplicationClassName,
                        const WNDPROC   i_WindowProcedure,
                        HWND&           o_WindowHandle,
                        HDC&            o_DeviceContext,
                        HGLRC&          o_RenderingContext );

void DestroyApplication( HWND&  io_WindowHandle,
                         HDC&   io_DeviceContext,
                         HGLRC& io_RenderingContext );

static bool RegisterApplicationClass( const HINSTANCE, const char*, const WNDPROC );
static bool CreateApplicationWindow( const HINSTANCE, const char*, HWND& );
static void DestroyApplicationWindow( HWND& );
static void UnregisterApplicationClass( const char*, HINSTANCE& );

#endif // _APPLICATION_CREATION_HEADER_