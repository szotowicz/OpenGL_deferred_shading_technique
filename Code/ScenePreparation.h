#ifndef _SCENE_PREPARATION_HEADER_
#define _SCENE_PREPARATION_HEADER_

#include <Windows.h>
#include <vector>
#include <GL\glcorearb.h>

bool PrepareScene();

void Resize( const int i_Width,
             const int i_Height );

void Update( WPARAM i_wParam,
             LPARAM i_lParam );

static GLuint CreateRectTexture( const size_t, const size_t, const GLenum, const GLenum, const GLenum );
static GLuint CreateRenderTarget( const std::vector<std::pair<GLenum, GLuint>> );

#endif // _SCENE_PREPARATION_HEADER_