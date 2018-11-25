#include "GlobalVariables.h"
#include "ApplicationCreation.h"
#include "ScenePreparation.h"
#include "Drawing.h"

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
extern void Update( WPARAM _wParam, LPARAM _lParam );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WinMain                                                                                                   //
//                                                                                                           //
// Application entry point                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE   i_Instance,
                    HINSTANCE   i_PrevInstance,
                    LPSTR       i_CmdLine,
                    int         i_CmdShow ) {
    // 3. Add name and surname to application title
    // ...
    // Create window and OpenGL rendering context
    if( !CreateApplication( i_Instance, "Mikolaj Szotowicz", WndProc, GWindowHandle, GDeviceContext, GRenderingContext ) ) {
        return 1;
    }
    // Setup OpenGL state and prepare objects for drawing
    if( !PrepareScene() ) {
        return 1;
    }

    // Main program loop - processes messages and draws scene
    MSG		message;
    bool	quit = false;
    while( !quit ) {
        // Check queue for message and, if there is any, process it
        if( PeekMessage( &message, nullptr, 0, 0, PM_REMOVE ) ) { 
            if( message.message == WM_QUIT ) {
                quit = true;
            } else {
                // Process message
                TranslateMessage( &message );
                DispatchMessage( &message );
            }
        } else {
            // Draw scene rendered with OpenGL
            Draw();
        }
    }

    // Release all objects, destroy window
    DestroyApplication( GWindowHandle, GDeviceContext, GRenderingContext );
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WndProc                                                                                                   //
//                                                                                                           //
// Window procedure handling all messages                                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc( HWND      i_hWnd,
                          UINT      i_Message,
                          WPARAM    i_wParam,
                          LPARAM    i_lParam ) {
    switch( i_Message ) {
    case WM_PAINT: {                                                // Window needs repainting
            Draw();
            ValidateRect( i_hWnd, nullptr );                        // Tell operating system that drawing is finished
            return 0;
        }
    case WM_SIZE: {                                                 // Size of the window has changed
            Resize( LOWORD(i_lParam), HIWORD(i_lParam) );           // Change OpenGL screen size (viewport)
            return 0;
        }
    case WM_CLOSE: {                                                // Application is closing
            PostQuitMessage( 0 );                                   // Send message about application exit
            return 0;
        }
    case WM_KEYDOWN: {                                              // User pressed any key
            Update( i_wParam, i_lParam );
            return 0;
        }
    case WM_KEYUP: {                                                // User released any key
            if( i_wParam == VK_ESCAPE ) {                           // If key released by user was escape, tell application to quit
                PostQuitMessage(0);
            }
            break;
        }
    case WM_SYSCOMMAND: {                                           // Operating system messages
            switch( i_wParam ) {
            case SC_SCREENSAVE:                                     // Turn off screensaver or monitor standby mode by capturing proper messages
            case SC_MONITORPOWER:
                return 0;
            }
        }
    default: {                                                      // Leave processing of any other messages to operating system
            return DefWindowProc( i_hWnd, i_Message, i_wParam, i_lParam );
        }
    }
    return 0;
}
