#include "OpenGLFunctionsCreation.h"
#include "GlobalVariables.h"
#include "OpenGLFunctions.h"
#include "OpenGLCreation.h"
#include "ErrorMessage.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateOpenGL                                                                                              //
//                                                                                                           //
// Creates OpenGL context with default, Windows routines (1st time)                                          //
// or with extended, OpenGL's methods (2nd time)                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CreateOpenGL( const bool i_FirstCreation,
                   const HWND i_WindowHandle,
                   HDC&       o_DeviceContext,
                   HGLRC&     o_RenderingContext ) {
    // Get context of a device associated with created window
    HDC device_context = GetDC( i_WindowHandle );
    if( !device_context ) {
        ErrorMessage( "OpenGL Creation Error", "Could not get the device context." );
        return false;
    }

    HGLRC rendering_context;

    // Create OpenGL's rendering context
    if( i_FirstCreation ) {
        if( !CreateBaseOpenGL( device_context, rendering_context ) ) {
            return false;
        }
    } else {
        if( !CreateExtendedOpenGL( device_context, rendering_context ) ) {
            return false;
        }
    }
    if( !rendering_context ) {
        return false;
    }

    o_DeviceContext     = device_context;
    o_RenderingContext  = rendering_context;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateBaseOpenGL                                                                                          //
//                                                                                                           //
// Creates OpenGL context with default, Windows routines                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool CreateBaseOpenGL( const HDC i_DeviceContext,
                              HGLRC&    o_RenderingContext ) {
    HGLRC rendering_context;

    // Set the description of drawing buffers
    if( !SetUpBasePixelFormat( i_DeviceContext ) ) {
        return false;
    }
    // Create rendering context
    if( !CreateBaseContext( i_DeviceContext, rendering_context ) ) {
        return false;
    }
    // Check OpenGL's extensions and get addresses of OpenGL's functions beyond version 1.1
    if( !LoadOpenGLFunctions( i_DeviceContext ) ) {
        return false;
    }

    o_RenderingContext = rendering_context;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SetUpBasePixelFormat                                                                                      //
//                                                                                                           //
// Sets up a pixel format - a description of a drawing buffers - using old, Windows routines                 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool SetUpBasePixelFormat( const HDC i_DeviceContext ) {
    // Fill the structure describing our desired format of drawing buffers
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),		// Size of the Pixel Format Descriptor
        1,									// Version number
        PFD_DRAW_TO_WINDOW |				// Support for rendering in a window
        PFD_SUPPORT_OPENGL |				// Support for OpenGL
        PFD_DOUBLEBUFFER,					// Support for double buffering
        PFD_TYPE_RGBA,						// RGBA color format (no palette)
        32,									// Number of color bits
        0, 0, 0, 0, 0, 0,					// Color bits and bit shifts for all color components...
        0, 0,								// ... and for alpha channel (ignored here - general information about 32 color bits is enough)
        0,									// No accumulation buffer
        0, 0, 0, 0,							// Accumulation bits (ignored)
        24,									// 24 bit depth buffer (Z-Buffer)
        8,									// 8 bit stencil buffer (not used but some graphics cards have problems creating drawing buffer without stencil)
        0,									// No auxiliary buffer
        PFD_MAIN_PLANE,						// Main drawing layer
        0,									// Reserved
        0, 0, 0								// Layer masks (ignored)
    };

    // Choose the best match for drawing buffer's properties
    int pixel_format = ChoosePixelFormat( i_DeviceContext, &pfd );
    if( !pixel_format ) {
        ErrorMessage( "OpenGL Creation Error", "Could not find the requested pixel format." );
        return false;
    }
    // Set the selected pixel format
    if( !SetPixelFormat( i_DeviceContext, pixel_format, &pfd ) ) {
        ErrorMessage( "OpenGL Creation Error", "Could not set the requested pixel format." );
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateBaseContext                                                                                         //
//                                                                                                           //
// Creates base rendering context - object containing all OpenGL states needed for drawing - using old,      //
// Windows routines                                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool CreateBaseContext( const HDC i_DeviceContext,
                               HGLRC&    o_RenderingContext ) {
    // Create rendering context
    HGLRC rendering_context = wglCreateContext( i_DeviceContext );
    if( !rendering_context ) {
        ErrorMessage( "OpenGL Creation Error", "Could not create rendering context." );
        return false;
    }
    // Set the created context as the current context used for OpenGL's drawing commands
    if( !wglMakeCurrent( i_DeviceContext, rendering_context ) ) {
        wglDeleteContext( rendering_context );
        ErrorMessage( "OpenGL Creation Error", "Could not activate rendering context." );
        return false;
    }

    o_RenderingContext = rendering_context;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateExtendedOpenGL                                                                                      //
//                                                                                                           //
// Creates OpenGL context with extended (better) functions acquired during first creation                    //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool CreateExtendedOpenGL( const HDC i_DeviceContext,
                                  HGLRC&    o_RenderingContext ) {
    HGLRC rendering_context;

    // Check if new methods for setting pixel format are available
    if( ARB_pixel_format_present ) {
        // Set pixel format with new methods
        if( !SetUpExtendedPixelFormat( i_DeviceContext ) ) {
            return false;
        }
    } else {
        // Set pixel format with old methods
        if( !SetUpBasePixelFormat( i_DeviceContext ) ) {
            return false;
        }
    }
    // Check if new methods for creating rendering context are available
    if( ARB_create_context_present ) {
        // Create context with new methods
        if( !CreateExtendedContext( i_DeviceContext, rendering_context ) ) {
            return false;
        }
    } else {
        // Create context the old way
        if( !CreateBaseContext( i_DeviceContext, rendering_context ) ) {
            return false;
        }
    }
    // Check OpenGL's extensions and get addresses of OpenGL's functions beyond version 1.1
    // Newly created rendering context may have different addresses of the same functions!
    if( !LoadOpenGLFunctions( i_DeviceContext ) ) {
        return false;
    }

    o_RenderingContext = rendering_context;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SetUpExtendedPixelFormat                                                                                  //
//                                                                                                           //
// Sets up a pixel format - a description of a drawing buffers - using better, more flexible routines        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool SetUpExtendedPixelFormat( const HDC i_DeviceContext ) {
    // Fill the array describing our desired format of drawing buffers
    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,	GL_TRUE,                        // Support for rendering in a window
        WGL_SUPPORT_OPENGL_ARB,	GL_TRUE,                        // Support for OpenGL
        WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,                        // Support for double buffering
        WGL_PIXEL_TYPE_ARB,		WGL_TYPE_RGBA_ARB,              // RGBA color type (no palette)
        WGL_COLOR_BITS_ARB,		32,                             // 32 color bits (8 for each RGBA channel)
        WGL_DEPTH_BITS_ARB,		24,                             // 24 bits for depth buffer
        WGL_STENCIL_BITS_ARB,	8,                              // 8 bits for stencil buffer
        WGL_ACCELERATION_ARB,	WGL_FULL_ACCELERATION_ARB,      // Hardware acceleration
        0                                                       // End of list
    };

    int pixel_format = 0;
    unsigned int num_formats_found = 0;
    PIXELFORMATDESCRIPTOR pfd;

    // Choose the best format of a drawing buffers
    if( !(wglChoosePixelFormatARB( i_DeviceContext, pixel_format_attribs, nullptr, 1, &pixel_format, &num_formats_found )) || (pixel_format == 0) ) {
        ErrorMessage( "OpenGL Creation Error", "Could not find requested pixel format." );
        return false;
    }
    // Set the selected pixel format
    if( !SetPixelFormat( i_DeviceContext, pixel_format, &pfd ) ) {
        ErrorMessage( "OpenGL Creation Error", "Could not set the requested pixel format." );
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateExtendedContext                                                                                     //
//                                                                                                           //
// Creates extended rendering context - object containing all OpenGL states needed for drawing - using new   //
// methods acquired during the first creation of rendering context                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool CreateExtendedContext( const HDC i_DeviceContext,
                                   HGLRC&    o_RenderingContext ) {
    const GLint versions[][2] = { {4, 3}, {4, 2}, {4, 1}, {4, 0}, {3, 3}, {3, 2}, {3, 1}, {3, 0} };
    const int   major = 0;
    const int   minor = 1;
    const int   count = sizeof(versions)/sizeof(versions[0]);

    // Fill the array describing desired attributes of rendering context
    GLint context_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB,	versions[0][major],                         // Major version of context
        WGL_CONTEXT_MINOR_VERSION_ARB,	versions[0][minor],                         // Minor version of context
        WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,     // Forward compatible context
        WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_CORE_PROFILE_BIT_ARB,           // Only core functionality
        0                                                                           // End of list
    };

    // If "WGL_ARB_create_context_profile" extension is not set, "WGL_CONTEXT_PROFILE_MASK_ARB" flag cannot be used
    if( !ARB_create_context_profile_present ) {
        context_attribs[6] = context_attribs[7] = 0;
    }

    HGLRC rendering_context;

    // Find and create context with the highest supported on this computer version
    for( int i=0; i<count; ++i ) {
        context_attribs[1] = versions[i][major];
        context_attribs[3] = versions[i][minor];
        rendering_context = wglCreateContextAttribsARB( i_DeviceContext, 0, context_attribs );
        if( rendering_context ) {
            break;
        }
    }
    if( !rendering_context ) {
        ErrorMessage( "OpenGL Creation Error", "Could not create OpenGL 3.0+ rendering context." );
        return false;
    }
    // Set the created context as the current context used for OpenGL's drawing commands
    if( !wglMakeCurrent( i_DeviceContext, rendering_context ) ) {
        wglDeleteContext( rendering_context );
        ErrorMessage( "OpenGL Creation Error", "Could not activate rendering context." );
        return false;
    }

    o_RenderingContext = rendering_context;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DestroyOpenGL                                                                                             //
//                                                                                                           //
// Destroys OpenGL's context and releases device context                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DestroyOpenGL( const HWND i_WindowHandle,
                    HDC&       io_DeviceContext,
                    HGLRC&     io_RenderingContext ) {
    //void DestroyGeometry();

    // Destroy rendering context
    if( io_RenderingContext ) {
        // Stop using rendering context
        if( !wglMakeCurrent( nullptr, nullptr ) ) {
            ErrorMessage( "OpenGL Shutdown Error", "Could not deactivate rendering context." );
        }
        // Delete context
        if( !wglDeleteContext( io_RenderingContext ) ) {
            ErrorMessage( "OpenGL Shutdown Error", "Could not release rendering context." );
        }
        io_RenderingContext = 0;
    }

    if( io_DeviceContext ) {
        // Release device context
        if( !ReleaseDC( i_WindowHandle, io_DeviceContext ) ) {
            ErrorMessage( "OpenGL Shutdown Error", "Could not release device context." );
        }
        io_DeviceContext = 0;
    }
}
