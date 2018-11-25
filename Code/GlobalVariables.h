#ifndef _GLOBAL_VARIABLES_HEADER_
#define _GLOBAL_VARIABLES_HEADER_

#include <Windows.h>
#include <GL\glcorearb.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Window management variables                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern HINSTANCE        GApplicationInstance;                   // Operating system's handle to instance of the application
extern const char*      GApplicationClassName;                  // Name of the class associated with application
extern HWND             GWindowHandle;                          // Handle to window
extern HDC              GDeviceContext;                         // Operating system's device context used for drawing in a window
extern HGLRC            GRenderingContext;                      // Rendering context for OpenGL
extern float            GWidth;                                 // Window's width
extern float            GHeight;                                // Window's height
extern float            GAspectRatio;                           // Window's aspect ratio

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL management variables                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern bool             ARB_extensions_string_present;          // Extension indicating that all extensions can be check
extern bool             ARB_pixel_format_present;               // Extension that allows setting pixel format with more flexible functions
extern bool             ARB_create_context_present;             // Extension that allows creating rendering context with more flexible functions
extern bool             ARB_create_context_profile_present;     // Extension that allows creating core rendering context

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Geometry variables                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern unsigned int     GSphereVAO;                             // Sphere mesh
extern unsigned int     GPlaneVAO;                              // Plane mesh
extern unsigned int     GQuadVAO;                               // Fullscreen quad mesh

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader variables                                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern unsigned int     BasePassProgram;                        // Shader program used for drawing base pass
extern unsigned int     LightingPassProgram;                    // Shader program used for drawing lighting passGQuadVAO;

extern GLint            MVPMatrixHandle;                        // Base pass model view projection matrix parameter handle
extern GLint            MVMatrixHandle;                         // Base pass model view matrix parameter handle
extern GLint            DiffuseTextureHandle;                   // Base pass diffuse texture parameter handle

extern GLint            ColorTextureHandle;                     // Lighting pass color texture parameter handle
extern GLint            NormalTextureHandle;                    // Lighting pass normal texture parameter handle
extern GLint            PositionTextureHandle;                  // Lighting pass position texture parameter handle
extern GLint            LightDistanceHandle;                    // Lighting pass light distance parameter handle

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene variables                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern GLuint           DiffuseTexture;                         // Diffuse texture ID for geometry

extern GLuint           ColorTexture;                           // Texture containing colors of scene objects
extern GLuint           NormalTexture;                          // Texture containing normal vectors of scene objects
extern GLuint           PositionTexture;                        // Texture containing positions of scene objects
extern GLuint           DepthTexture;                           // Texture used as a depth buffer
extern GLuint           BasePassRT;                             // Render target texture for base pass

extern float            Angle;                                  // Rotation angle for scene objects
extern float            LightDistance;                          // Light position distance from camera
extern float            ProjectionMatrix[16];                   // Projection matrix
extern float            ModelViewMatrix[16];                    // Model view matrix
extern float            ModelViewProjectionMatrix[16];          // Model view projection matrix

#endif // _GLOBAL_VARIABLES_HEADER_