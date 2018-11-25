#include "OpenGLFunctionsCreation.h"
#include "GlobalVariables.h"
#include "OpenGLFunctions.h"
#include "ErrorMessage.h"

#define CHECKEXTENSION( x, y, z ) x = CheckExtension( #y, z )
#define GETFUNCTIONADDRESS( x, y ) y = (x)GetFunctionAddress( handle, #y )

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadOpenGLFunctions                                                                                       //
//                                                                                                           //
// Checks if specific extensions are present and loads all necessary functions                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool LoadOpenGLFunctions( const HDC i_DeviceContext ) {
    // Get the handle of the OpenGL library from which functions will be loaded
    HMODULE handle = GetModuleHandle( "OpenGL32.dll" );

    if( !(GETFUNCTIONADDRESS( PFNWGLGETEXTENSIONSSTRINGARBPROC, wglGetExtensionsStringARB )) ) {
        return false;
    }

    // Get all extensions supported on this computer
    const char* wglExtensions = wglGetExtensionsStringARB( i_DeviceContext );

    if( !(CHECKEXTENSION( ARB_extensions_string_present, WGL_ARB_extensions_string, wglExtensions )) ) {
        return false;
    }
    if( CHECKEXTENSION( ARB_pixel_format_present, WGL_ARB_pixel_format, wglExtensions ) ) {
        GETFUNCTIONADDRESS( PFNWGLCHOOSEPIXELFORMATARBPROC, wglChoosePixelFormatARB );
    }
    if( CHECKEXTENSION( ARB_create_context_present, WGL_ARB_create_context, wglExtensions ) ) {
        GETFUNCTIONADDRESS( PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB );
    }

    CHECKEXTENSION( ARB_create_context_profile_present, WGL_ARB_create_context_profile, wglExtensions );

    if( !(GETFUNCTIONADDRESS( PFNGLGETINTEGERVPROC, glGetIntegerv )) ) {
        return false;
    }

    int MajorVersion, MinorVersion;
    glGetIntegerv( GL_MAJOR_VERSION, &MajorVersion );
    glGetIntegerv( GL_MINOR_VERSION, &MinorVersion );

    // Application is designed for 3.0+ OGL version and forward compatible mode
    // so if version is earlier than 3.0 there is no need to get entry points for other functions
    if( MajorVersion < 3 ) {
        return true;
    }

    // Acquire addresses of all core OpenGL functions
    if( !(GETFUNCTIONADDRESS( PFNGLGETSTRINGPROC, glGetString )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETSTRINGIPROC, glGetStringi )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETERRORPROC, glGetError )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLCLEARCOLORPROC, glClearColor)) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLCLEARPROC, glClear )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLVIEWPORTPROC, glViewport )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLCLEARDEPTHPROC, glClearDepth )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDEPTHFUNCPROC, glDepthFunc )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDEPTHMASKPROC, glDepthMask )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBLENDFUNCPROC, glBlendFunc )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLENABLEPROC, glEnable )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDISABLEPROC, glDisable )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLFRONTFACEPROC, glFrontFace )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLCULLFACEPROC, glCullFace )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLPOLYGONMODEPROC, glPolygonMode )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLSAMPLECOVERAGEPROC, glSampleCoverage )) )
        return false;

    // Shaders
    if( !(GETFUNCTIONADDRESS( PFNGLCREATESHADERPROC, glCreateShader )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLSHADERSOURCEPROC, glShaderSource )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLCOMPILESHADERPROC, glCompileShader )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETSHADERIVPROC, glGetShaderiv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDELETESHADERPROC, glDeleteShader )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLCREATEPROGRAMPROC, glCreateProgram )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLATTACHSHADERPROC, glAttachShader )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLLINKPROGRAMPROC, glLinkProgram )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETPROGRAMIVPROC, glGetProgramiv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUSEPROGRAMPROC, glUseProgram )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDETACHSHADERPROC, glDetachShader )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDELETEPROGRAMPROC, glDeleteProgram )) )
        return false;

    // Vertex arrays
    if( !(GETFUNCTIONADDRESS( PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBINDVERTEXARRAYPROC, glBindVertexArray )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays )) )
        return false;

    // Buffers
    if( !(GETFUNCTIONADDRESS( PFNGLGENBUFFERSPROC, glGenBuffers )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBINDBUFFERPROC, glBindBuffer )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBUFFERDATAPROC, glBufferData )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBUFFERSUBDATAPROC, glBufferSubData )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDELETEBUFFERSPROC, glDeleteBuffers )) )
        return false;

    // Framebuffers
    if( !(GETFUNCTIONADDRESS( PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDRAWBUFFERPROC, glDrawBuffer )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDRAWBUFFERSPROC, glDrawBuffers )) )
        return false;


    // Vertex attribs
    if( !(GETFUNCTIONADDRESS( PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBINDFRAGDATALOCATIONPROC, glBindFragDataLocation )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETACTIVEATTRIBPROC, glGetActiveAttrib )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETVERTEXATTRIBIVPROC, glGetVertexAttribiv )) )
        return false;

    // Textures
    if( !(GETFUNCTIONADDRESS( PFNGLGENTEXTURESPROC, glGenTextures )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDELETETEXTURESPROC, glDeleteTextures )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLBINDTEXTUREPROC, glBindTexture )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLACTIVETEXTUREPROC, glActiveTexture )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLTEXPARAMETERIPROC, glTexParameteri )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLTEXIMAGE2DPROC, glTexImage2D )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGENERATEMIPMAPPROC, glGenerateMipmap )) )
        return false;

    // Uniform paramters
    if( !(GETFUNCTIONADDRESS( PFNGLGETACTIVEUNIFORMPROC, glGetActiveUniform )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM1FVPROC, glUniform1fv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM2FVPROC, glUniform2fv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM3FVPROC, glUniform3fv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM4FVPROC, glUniform4fv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM1IVPROC, glUniform1iv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM2IVPROC, glUniform2iv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM3IVPROC, glUniform3iv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM4IVPROC, glUniform4iv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORM1IPROC, glUniform1i )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv )) )
        return false;

    // Drawing
    if( !(GETFUNCTIONADDRESS( PFNGLDRAWARRAYSPROC, glDrawArrays )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLDRAWELEMENTSPROC, glDrawElements )) )
        return false;
    if( !(GETFUNCTIONADDRESS( PFNGLGETERRORPROC, glGetError )) )
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CheckExtension                                                                                            //
//                                                                                                           //
// Checks whether given extension string is present in a string with all available extensions                //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool CheckExtension( const char* i_Extension,
                            const char* i_AllExtensions ) {
    // Check if given extension string is present
    const char* start = strstr( i_AllExtensions, i_Extension );
    if( start == nullptr ) {
        return false;
    }

    size_t i=0;
    size_t length = strlen( i_Extension );

    // Make sure that string matches exactly the one provided
    for( ; i<length; ++i ) {
        if( i_Extension[i] != start[i] ) {
            return false;
        }
    }
    // And check if it is not another extension which name contains the one we are searching for
    if( (start[i] != ' ') && (start[i] != '\0') ) {
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetFunctionAddress                                                                                        //
//                                                                                                           //
// Acquires address of a given OpenGL function                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* GetFunctionAddress( const HMODULE i_Handle,
                                 const char*   i_ProcedureName ) {
    // Get the address of a given function (for OpenGL versions 1.2+)
    PROC address = wglGetProcAddress( i_ProcedureName );
    if( !address ) {
        // Get the address of a function from OpenGL32.dll (versions 1.0 and 1.1)
        address = GetProcAddress( i_Handle, i_ProcedureName );
        if( !address ) {
            ErrorMessage( "Error Getting Function Address", i_ProcedureName );
        }
    }
    return address;
}
