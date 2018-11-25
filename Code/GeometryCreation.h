#ifndef _GEOMETRY_CREATION_HEADER_
#define _GEOMETRY_CREATION_HEADER_

void CreateGeometry();
void CreateFullscreenQuad( const float i_Width, const float i_Height );
void DestroyGeometry();

static void GenerateMesh( const size_t, const int*, const float*, const float*, const float*, unsigned int&, unsigned int& );

#endif // _GEOMETRY_CREATION_HEADER_