#include "ScenePreparation.h"
#include "GeometryCreation.h"
#include "ShaderCreation.h"
#include "MatrixAlgebra.h"
#include "TextureLoader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrepareScene                                                                                              //
//                                                                                                           //
// Creates geometry, shaders, textures, gets handles to shader parameters and sets their values              //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool PrepareScene() {
    // Set up default OpenGL state
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );                 // Set background clearing color to black
    glClearDepth( 1.0f );                                   // Set highest value for depth buffer cleaning
    glEnable( GL_DEPTH_TEST );                              // Enable depth test
    glDepthFunc( GL_LEQUAL );                               // Set less-equal function for depth testing
    glEnable( GL_CULL_FACE );                               // Enable face culling
    glCullFace( GL_BACK );                                  // Cull back faces

    // Create meshes used for drawing
    CreateGeometry();
    // Create shaders and program objects
    if( !CreateShaders() ) {
        return false;
    }

    // Get handles (addresses/locations) of shader uniform parameters
    MVPMatrixHandle      = glGetUniformLocation( BasePassProgram, "uMVPMatrix" );
    MVMatrixHandle       = glGetUniformLocation( BasePassProgram, "uModelViewMatrix" );
    DiffuseTextureHandle = glGetUniformLocation( BasePassProgram, "uTexture" );

    ColorTextureHandle    = glGetUniformLocation( LightingPassProgram, "uColor" );

    // 21. Store location of a normal texture from LightingPass.fp shader in a NormalTextureHandle variable (see glGetUniformLocation function documentation)
    // ...
	NormalTextureHandle = glGetUniformLocation(LightingPassProgram, "uNormal");
    // 31. Store location of a position texture from LightingPass.fp shader in a PositionTextureHandle variable (see glGetUniformLocation function documentation)
    // ...
	PositionTextureHandle = glGetUniformLocation(LightingPassProgram, "uPosition");
    // Get perspective projection matrix
    GetPerspectiveProjectionMatrix( 45.0f, 1.0f, 20.0f, ProjectionMatrix );

    // Create G-Buffer - textures for all data stored in base pass
    ColorTexture    = CreateRectTexture( (size_t)GWidth, (size_t)GHeight, GL_RGBA, GL_RGBA16F, GL_HALF_FLOAT );
    NormalTexture   = CreateRectTexture( (size_t)GWidth, (size_t)GHeight, GL_RGB, GL_RGB16F, GL_HALF_FLOAT );
    PositionTexture = CreateRectTexture( (size_t)GWidth, (size_t)GHeight, GL_RGB, GL_RGB16F, GL_HALF_FLOAT );
    DepthTexture    = CreateRectTexture( (size_t)GWidth, (size_t)GHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_UNSIGNED_INT );
    // Load diffuse texture for geometry drawn in base pass
    if( !CreateTextureFromTGA( "01.tga", DiffuseTexture ) ) {
        return false;
    }

    // Set textures to proper "indices"
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, DiffuseTexture );
    glActiveTexture( GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_RECTANGLE, ColorTexture );

    // 22. Activate texture channel 2 and bind NormalTexture texture with it (see glActiveTexture and glBindTexture functions documentation)
    // Check the type of NormalTexture
    // ...
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, NormalTexture);
    // 32. Activate texture channel 3 and bind PositionTexture texture with it (see glActiveTexture and glBindTexture functions documentation)
    // ...
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_RECTANGLE, PositionTexture);
    // Set values for shader uniform parameters
    glUseProgram( BasePassProgram );
    glUniform1i( DiffuseTextureHandle, 0 );

    glUseProgram( LightingPassProgram );
    glUniform1i( ColorTextureHandle, 1 );

    // 23. Store in a uniform variable represented by NormalTextureHandle activated texture channel number (see glUniform1i function documentation)
    // ...
	glUniform1i(NormalTextureHandle, 2);
    // 33. Store in a uniform variable represented by PositionTextureHandle activated texture channel number (see glUniform1i function documentation)
    // ...
	glUniform1i(PositionTextureHandle, 3);
    // Deactivate any texture units
    glActiveTexture( GL_TEXTURE8 );
    glBindTexture( GL_TEXTURE_2D, 0 );
    glBindTexture( GL_TEXTURE_RECTANGLE, 0 );

    // 15b. Analize CreateRenderTarget function call
    // ...
    std::pair<GLenum, GLuint> pairs[] = {
        std::make_pair( GL_COLOR_ATTACHMENT0, ColorTexture ),
        std::make_pair( GL_COLOR_ATTACHMENT1, NormalTexture ),
        std::make_pair( GL_COLOR_ATTACHMENT2, PositionTexture ),
        std::make_pair( GL_DEPTH_ATTACHMENT, DepthTexture ) };
    BasePassRT = CreateRenderTarget( std::vector<std::pair<GLenum, GLuint>>( pairs, pairs + 4 ) );

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateRectTexture                                                                                         //
//                                                                                                           //
// Creates empty rectangle texture with provided properties                                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static GLuint CreateRectTexture( const size_t i_Width,
                                 const size_t i_Height,
                                 const GLenum i_Channels,
                                 const GLenum i_Format,
                                 const GLenum i_ChannelDataType ) {
    GLuint texture;

    // Generate texture ID and bind it
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_RECTANGLE, texture );

    // Set texture parameters
    glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR );      // Set linear filtering for magnification
    glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR );      // Set linear filtering for minification
    glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );   // Clamp texture coordinates in texture's X direction
    glTexParameteri( GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );   // Clamp texture coordinates in texture's Y direction
    // Allocate memory for texture but no data is loaded
    glTexImage2D( GL_TEXTURE_RECTANGLE, 0, i_Format, i_Width, i_Height, 0, i_Channels, i_ChannelDataType, nullptr );

    // Deactivate texture
    glBindTexture( GL_TEXTURE_RECTANGLE, 0 );
    return texture;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateRenderTarget                                                                                        //
//                                                                                                           //
// Creates framebuffer object with attachments provided through vector of pairs:                             //
// type and number of attachment - texture ID that should be attached to it                                  //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static GLuint CreateRenderTarget( const std::vector<std::pair<GLenum, GLuint>> i_Textures ) {
    // 15a. Analize function that creates render target (framebuffer object)
    // ...
    GLuint framebuffer;

    // Generate and framebuffer object (FBO)
    glGenFramebuffers( 1, &framebuffer );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, framebuffer );

    // Attach all provided textures to specified framebuffer attachments (places textures in specified slots)
    for( size_t i=0; i<i_Textures.size(); ++i ) {
        glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, i_Textures[i].first, GL_TEXTURE_RECTANGLE, i_Textures[i].second, 0 );
    }

    // Check status of created framebuffer
    GLenum error = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER );
    switch( error ) {
    case GL_FRAMEBUFFER_COMPLETE:
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        ErrorMessage( "RT Creation Error", "Incomplete attachement" );
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        ErrorMessage( "RT Creation Error", "Missing attachement" );
        break;
    default:
        ErrorMessage( "RT Creation Error", "Could not create render target" );
        break;
    }

    // Disable FBO for normal, on-screen rendering
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
    return framebuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Resize                                                                                                    //
//                                                                                                           //
// Calculates aspect ratio and updates viewport size and fullscreen quad mesh                                //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Resize( const int i_Width,
             const int i_Height ) {
    int w = i_Width;
    int h = i_Height;
    if( h <= 0 ) {
        h = 1;
    }

    // Store new dimensions
    GWidth	= (float)w;
    GHeight	= (float)h;
    GAspectRatio = (float)GWidth / (float)GHeight;

    // Update viewport dimensions
    glViewport( 0, 0, w, h );
    // Update fullscreen quad mesh
    CreateFullscreenQuad( (float)w, (float)h );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update                                                                                                    //
//                                                                                                           //
// Responds to user input                                                                                    //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Update( WPARAM i_wParam,
             LPARAM i_lParam ) {
    switch( i_wParam ) {
    case VK_LEFT: {                                     // User pressed left arrow key
            Angle -= 1.0f;
            break;
        }
    case VK_RIGHT: {                                    // User pressed right arrow key
            Angle += 1.0f;
            break;
        }
    case VK_UP: {                                       // User pressed up arrow key
            glUseProgram( LightingPassProgram );
            LightDistance -= 0.1f;
            glUniform1fv( LightDistanceHandle, 1, &LightDistance );
            break;
        }
    case VK_DOWN: {                                     // User pressed down arrow key
            glUseProgram( LightingPassProgram );
            LightDistance += 0.1f;
            glUniform1fv( LightDistanceHandle, 1, &LightDistance );
            break;
        }
    }
}
