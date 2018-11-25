#include "GlobalVariables.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Window management variables                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

HINSTANCE       GApplicationInstance;                   // Operating system's handle to instance of the application
const char*     GApplicationClassName;                  // Name of the class associated with application
HWND            GWindowHandle;                          // Handle to window
HDC             GDeviceContext;                         // Operating system's device context used for drawing in a window
HGLRC           GRenderingContext;                      // Rendering context for OpenGL
float           GWidth;                                 // Window's width
float           GHeight;                                // Window's height
float           GAspectRatio;                           // Window's aspect ratio

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL management variables                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool            ARB_extensions_string_present;          // Extension indicating that all extensions can be check
bool            ARB_pixel_format_present;               // Extension that allows setting pixel format with more flexible functions
bool            ARB_create_context_present;             // Extension that allows creating rendering context with more flexible functions
bool            ARB_create_context_profile_present;     // Extension that allows creating core rendering context

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Geometry variables                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int    GSphereVAO;                             // Sphere mesh
unsigned int    GPlaneVAO;                              // Plane mesh
unsigned int    GQuadVAO;                               // Fullscreen quad mesh

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shader variables                                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int    BasePassProgram;                        // Shader program used for drawing base pass
unsigned int    LightingPassProgram;                    // Shader program used for drawing lighting pass

GLint           MVPMatrixHandle;                        // Base pass model view projection matrix parameter handle
GLint           MVMatrixHandle;                         // Base pass model view matrix parameter handle
GLint           DiffuseTextureHandle;                   // Base pass diffuse texture parameter handle

GLint           ColorTextureHandle;                     // Lighting pass color texture parameter handle
GLint           NormalTextureHandle;                    // Lighting pass normal texture parameter handle
GLint           PositionTextureHandle;                  // Lighting pass position texture parameter handle
GLint           LightDistanceHandle;                    // Lighting pass light distance parameter handle

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene variables                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLuint          DiffuseTexture;                         // Diffuse texture ID for geometry

GLuint          ColorTexture;                           // Texture containing colors of scene objects
GLuint          NormalTexture;                          // Texture containing normal vectors of scene objects
GLuint          PositionTexture;                        // Texture containing positions of scene objects
GLuint          DepthTexture;                           // Texture used as a depth buffer
GLuint          BasePassRT;                             // Render target texture for base pass

float           Angle;                                  // Rotation angle for scene objects
float           LightDistance;                          // Light position distance from camera
float           ProjectionMatrix[16];                   // Projection matrix
float           ModelViewMatrix[16];                    // Model view matrix
float           ModelViewProjectionMatrix[16];          // Model view projection matrix