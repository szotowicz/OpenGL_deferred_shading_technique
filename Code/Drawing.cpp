#include "Drawing.h"
#include "MatrixAlgebra.h"
#include "OpenGLFunctions.h"
#include "GlobalVariables.h"
#include "ShaderCreation.h"
#include "GeometryCreation.h"
#include "TextureLoader.h"
#include "ErrorMessage.h"
#include <vector>

// 20a. Add another attachment with number 1 to target buffers used for drawing scene geometry
// ...

// 30a. Add another attachment with number 2 to target buffers
// ...

GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw                                                                                                      //
//                                                                                                           //
// Draws scene                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Draw() {
    // Base pass

    // 16a. Activate render target (framebuffer object) for base pass (see glBindFramebuffer function)
    // ...
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, BasePassRT);
    // 16b. Set target buffers into which drawing should be performed (see buffer variable and glDrawBuffers function)
    // ...
	glDrawBuffers(3, buffers);
    // 20b. Modify the number of target buffers used for drawing (see step 16b)
    // ...

    // 30b. Once again modify the number of target buffers used for drawing (see step 16b)
    // ...

    // Clear all render targets
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // Enable depth testing for base pass
    glEnable( GL_DEPTH_TEST );

    // Active shader program for base pass
    glUseProgram( BasePassProgram );
    // Activate VAO for drawing spheres
    glBindVertexArray( GSphereVAO );

    // Sphere 1
    // Rotate sphere than translate it
    GetYRotationMatrix( Angle, ModelViewMatrix );
    Translate( 2.5f, 0.0f, -9.0f, ModelViewMatrix );
    // Calculate MVP matrix
    Multiply( ProjectionMatrix, ModelViewMatrix, ModelViewProjectionMatrix );
    // Set values of shader uniform variables
    glUniformMatrix4fv( MVPMatrixHandle, 1, false, ModelViewProjectionMatrix );
    glUniformMatrix4fv( MVMatrixHandle, 1, false, ModelViewMatrix );
    // Draw sphere using VAO
    glDrawArrays( GL_TRIANGLES, 0, 672 );

    // Sphere 2
    // Rotate sphere than translate it
    GetYRotationMatrix( -Angle * 0.3f, ModelViewMatrix );
    Translate( -1.0f, 0.0f, -6.0f, ModelViewMatrix );
    // Calculate MVP matrix
    Multiply( ProjectionMatrix, ModelViewMatrix, ModelViewProjectionMatrix );
    // Set values of shader uniform variables
    glUniformMatrix4fv( MVPMatrixHandle, 1, false, ModelViewProjectionMatrix );
    glUniformMatrix4fv( MVMatrixHandle, 1, false, ModelViewMatrix );
    // Draw sphere using VAO
    glDrawArrays( GL_TRIANGLES, 0, 672 );

    // Activate VAO for drawing plane
    glBindVertexArray( GPlaneVAO );
    // Plane
    // Place plane at proper position
    GetTranslationMatrix( 0.0f, -1.0f, -5.0f, ModelViewMatrix );
    // Calculate MVP matrix
    Multiply( ProjectionMatrix, ModelViewMatrix, ModelViewProjectionMatrix );
    // Set values of shader uniform variables
    glUniformMatrix4fv( MVPMatrixHandle, 1, false, ModelViewProjectionMatrix );
    glUniformMatrix4fv( MVMatrixHandle, 1, false, ModelViewMatrix );
    // Draw plane using VAO
    glDrawArrays( GL_TRIANGLES, 0, 6 );

    // Lighting pass

    // 16c. Deactivate render target (disable FBO) before postprocess lighting phase
    // From now rendering will be performed normally, to window
    // ...
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // 16d. Set the back buffer as a target for drawing commands
    // ...
	glDrawBuffer(GL_BACK);
    // 16e. Clear color and depth of a back buffer (see glClear function)
    // ...
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 16f. Disable depth testing - it is not necessary in postprocess phase
    // ...
	glDisable(GL_DEPTH_TEST);
    // 14b. Activate shader program for lighting pass (see glUseProgram function)
    // ... 
	glUseProgram(LightingPassProgram);
    // 14c. Activate VAO for drawing fullscreen quad (see glBindVertexArray function)
    // ...
	glBindVertexArray(GQuadVAO);
    // 14d. Draw quad using VAO and TRIANGLES primitive (see glDrawArrays function)
    // ...
	glDrawArrays(GL_TRIANGLES, 0, 6);
    // Disable VAO - it is always good to disable all OpenGL objects when they are not required
    glBindVertexArray( 0 );

    // Swap back and front buffers
    SwapBuffers( GDeviceContext );
}
