#ifndef _SHADER_CREATION_HEADER_
#define _SHADER_CREATION_HEADER_

#include <string>
#include <string>
#include <sstream>
#include <fstream>
#include <GL\glcorearb.h>
#include "ErrorMessage.h"
#include "OpenGLFunctions.h"
#include "GlobalVariables.h"

bool CreateShaders();
void DestroyShaders();

static GLuint CreateShader( const std::string, GLenum );
static void GetTextfileContents( const std::string, std::string& );

#endif // _SHADER_CREATION_HEADER_