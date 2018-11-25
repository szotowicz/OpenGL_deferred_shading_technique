#ifndef _TEXTURE_LOADER_HEADER_
#define _TEXTURE_LOADER_HEADER_

#include "OpenGLFunctions.h"
#include "ErrorMessage.h"

struct STGA;

bool CreateTextureFromTGA( const char* i_Filename,
                           GLuint&     o_TextureID );

static bool LoadTGA( const char*, STGA& );

#endif // _TEXTURE_LOADER_HEADER_