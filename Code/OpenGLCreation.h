#ifndef _OPENGL_CREATION_HEADER_
#define _OPENGL_CREATION_HEADER_

#include <GL\glcorearb.h>
#include <GL\wglext.h>

bool CreateOpenGL( const bool i_FirstCreation,
                   const HWND i_WindowHandle,
                   HDC&       o_DeviceContext,
                   HGLRC&     o_RenderingContext );

void DestroyOpenGL( const HWND i_WindowHandle,
                    HDC&       io_DeviceContext,
                    HGLRC&     io_RenderingContext );

static bool CreateBaseOpenGL( const HDC, HGLRC& );
static bool SetUpBasePixelFormat( const HDC );
static bool CreateBaseContext( const HDC, HGLRC& );
static bool CreateExtendedOpenGL( const HDC, HGLRC& );
static bool SetUpExtendedPixelFormat( const HDC );
static bool CreateExtendedContext( const HDC, HGLRC& );

#endif // _OPENGL_CREATION_HEADER_