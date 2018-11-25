#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ErrorMessage                                                                                              //
//                                                                                                           //
// Displays message box with a given title and information                                                   //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ErrorMessage( const char* i_Title,
                   const char* i_Message ) {
    // Display message box
    MessageBox( nullptr, i_Message, i_Title, MB_OK | MB_ICONINFORMATION );
}
