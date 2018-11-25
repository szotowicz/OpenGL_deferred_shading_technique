#include "ApplicationCreation.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateApplication                                                                                         //
//                                                                                                           //
// Creates application window and OpenGL context                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CreateApplication( const HINSTANCE i_ApplicationInstance,
                        const char*     i_ApplicationClassName,
                        const WNDPROC   i_WindowProcedure,
                        HWND&           o_WindowHandle,
                        HDC&            o_DeviceContext,
                        HGLRC&          o_RenderingContext ) {
    HWND  window_handle;
    HDC   device_context;
    HGLRC rendering_context;

    // Register class of the application
    if( !RegisterApplicationClass( i_ApplicationInstance, i_ApplicationClassName, i_WindowProcedure ) ) {
        return false;
    }
    // Create window
    if( !CreateApplicationWindow( i_ApplicationInstance, i_ApplicationClassName, window_handle ) ) {
        return false;
    }
    // Create OpenGL context using old, Windows functions
    if( !CreateOpenGL( true, window_handle, device_context, rendering_context ) ) {
        return false;
    }

    // Destroy OpenGL context created with old functions
    DestroyOpenGL( window_handle, device_context, rendering_context );
    // Destroy window - pixel format for window can be set only once so new window need to be created
    DestroyApplicationWindow( window_handle );

    // Create window again
    if( !CreateApplicationWindow( i_ApplicationInstance, i_ApplicationClassName, window_handle ) ) {
        return false;
    }
    // Create OpenGL context this time using more flexible functions (if available)
    if( !CreateOpenGL( false, window_handle, device_context, rendering_context ) ) {
        return false;
    }

    ShowWindow( window_handle, SW_SHOW );
    UpdateWindow( window_handle );

    o_WindowHandle = window_handle;
    o_DeviceContext = device_context;
    o_RenderingContext = rendering_context;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RegisterApplicationClass                                                                                  //
//                                                                                                           //
// Register class of the application in operating system                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool RegisterApplicationClass( const HINSTANCE i_ApplicationInstance,
                                      const char*     i_ApplicationClassName,
                                      const WNDPROC   i_WindowProcedure ) {
    // Initialize structure describing application class
    WNDCLASSEX WindowClassEX;
    WindowClassEX.cbSize		= sizeof( WNDCLASSEX );
    WindowClassEX.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WindowClassEX.hInstance		= i_ApplicationInstance;
    WindowClassEX.lpfnWndProc	= i_WindowProcedure;
    WindowClassEX.cbClsExtra	= 0;
    WindowClassEX.cbWndExtra	= 0;
    WindowClassEX.hIcon			= nullptr;
    WindowClassEX.hIconSm		= nullptr;
    WindowClassEX.hCursor		= LoadCursor( nullptr, IDC_ARROW );
    WindowClassEX.hbrBackground	= nullptr;
    WindowClassEX.lpszMenuName	= nullptr;
    WindowClassEX.lpszClassName	= i_ApplicationClassName;

    // Register application's class in operating system
    if( !RegisterClassEx( &WindowClassEX ) ) {
        ErrorMessage( "Application Creation Error", "Failed to register application class." );
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateApplicationWindow                                                                                   //
//                                                                                                           //
// Creates window for the application                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool CreateApplicationWindow( const HINSTANCE i_ApplicationInstance,
                                     const char*     i_ApplicationClassName,
                                     HWND&           o_WindowHandle ) {
    DEVMODE displayProperties;

    // Check the resolution of desktop
    EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &displayProperties );

    // Set the window size to be a little smaller than desktop
    int Top		= 5;
    int Left	= 5;
    int Width	= displayProperties.dmPelsWidth-10;
    int Height	= displayProperties.dmPelsHeight-50;

    // Default style for windows that are not fullscreen
    DWORD WinStyle		= WS_OVERLAPPEDWINDOW;
    DWORD ExtendedStyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    // Create window
    HWND window_handle = CreateWindowEx( ExtendedStyle, i_ApplicationClassName, i_ApplicationClassName, WinStyle,
                                         Left, Top, Width, Height, nullptr, nullptr, i_ApplicationInstance, nullptr );
    if( !window_handle ) {
        ErrorMessage( "Application Creation Error", "Could not create application window." );
        return 0;
    }

    o_WindowHandle = window_handle;
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DestroyApplication                                                                                        //
//                                                                                                           //
// Destroys OpenGL's rendering context and application window                                                //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DestroyApplication( HWND&  io_WindowHandle,
                         HDC&   io_DeviceContext,
                         HGLRC& io_RenderingContext ) {
    // Destroy OpenGL rendering context
    DestroyOpenGL( io_WindowHandle, io_DeviceContext, io_RenderingContext );
    // Destroy window
    DestroyApplicationWindow( io_WindowHandle );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DestroyApplicationWindow                                                                                  //
//                                                                                                           //
// Destroys application window                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void DestroyApplicationWindow( HWND& io_WindowHandle ) {
    if( io_WindowHandle ) {
        // Destroy handle associated with window
        if( !DestroyWindow( io_WindowHandle ) ) {
            ErrorMessage( "Window Shutdown Error", "Could not release hWnd." );
        }
        io_WindowHandle = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UnregisterApplicationClass                                                                                //
//                                                                                                           //
// Unregisters class of the application                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void UnregisterApplicationClass( const char* i_ApplicationClassName,
                                        HINSTANCE&  io_ApplicationInstance ) {
    if( io_ApplicationInstance != nullptr ) {
        // Stop using class registered for the application
        if( !UnregisterClass( i_ApplicationClassName, io_ApplicationInstance ) ) {
            ErrorMessage( "Window Shutdown Error", "Could not unregister window class." );
        }
        io_ApplicationInstance = nullptr;
    }
}
