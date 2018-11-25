#include "ShaderCreation.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateShaders                                                                                             //
//                                                                                                           //
// Creates shaders and program objects used for drawing                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CreateShaders() {
    GLuint vshader, fshader;

    // Base pass shaders

    // Create vertex and fragment shaders for base pass
    vshader = CreateShader( "Shaders/BasePass.vp", GL_VERTEX_SHADER );
    fshader = CreateShader( "Shaders/BasePass.fp", GL_FRAGMENT_SHADER );
    // Create program object for base pass
    BasePassProgram = glCreateProgram();

    // Attach shaders to base pass program
    glAttachShader( BasePassProgram, vshader );
    glAttachShader( BasePassProgram, fshader );
    // Link base pass program
    glLinkProgram( BasePassProgram );

    GLint status;

    // Check linking status
    glGetProgramiv( BasePassProgram, GL_LINK_STATUS, &status );
    if( status == GL_FALSE ) {
        GLchar linking_info[1000];
        // Check what went wrong and display message box with information
        glGetProgramInfoLog( BasePassProgram, 1000, nullptr, linking_info );
        ErrorMessage( "Effect Initialization Error", linking_info );
        return false;
    }

    // Bind streams of vertex data with proper shader attributes
    // 12b. Bind named input vertex attributes (from BasePass.vp file) with proper stream numbers (see glBindAttribLocation function documentation)
    // ...
	glBindAttribLocation(BasePassProgram, 0, "inPosition");
	glBindAttribLocation(BasePassProgram, 1, "inTexCoord");
	glBindAttribLocation(BasePassProgram, 2, "inNormal");
    // 19a. Bind color output from fragment shader with 0. channel (see glBindFragDataLocation function documentation)
    // ...
	glBindFragDataLocation(BasePassProgram, 0, "oColor");
    // 19b. Bind normal output from fragment shader with 1. channel
    // ...
	glBindFragDataLocation(BasePassProgram, 1, "oNormal");
    // 29. Bind position output from fragment shader (from BasePass.fp file) with 2. channel
    // ...
	glBindFragDataLocation(BasePassProgram, 2, "oPosition");
    // 12c. Perform once again BasePassProgram linking to confirm introduced changes
    // ...
	glLinkProgram(BasePassProgram);
    // Lighting pass shaders

    // Create vertex and fragment shaders for lighting pass
    vshader = CreateShader( "Shaders/LightingPass.vp", GL_VERTEX_SHADER );
    fshader = CreateShader( "Shaders/LightingPass.fp", GL_FRAGMENT_SHADER );
    // Create program object for lighting pass
    LightingPassProgram = glCreateProgram();

    // Attach shaders to lighting pass program
    glAttachShader( LightingPassProgram, vshader );
    glAttachShader( LightingPassProgram, fshader );
    // Link lighting pass program
    glLinkProgram( LightingPassProgram );

    // Check linking status
    glGetProgramiv( LightingPassProgram, GL_LINK_STATUS, &status );
    if( status == GL_FALSE ) {
        GLchar linking_info[1000];
        // Check what went wrong and display message box with information
        glGetProgramInfoLog( LightingPassProgram, 1000, nullptr, linking_info );
        ErrorMessage( "Effect Initialization Error", linking_info );
        return false;
    }

    // Bind streams of vertex data with proper shader attributes
    glBindAttribLocation( LightingPassProgram, 0, "inPosition" );
    glBindAttribLocation( LightingPassProgram, 1, "inTexcoord" );
    glBindAttribLocation( LightingPassProgram, 2, "inTexcoord2" );

    // Link lighting pass program again so the new configuration is confirmed
    glLinkProgram( LightingPassProgram );
    return true;
}

void DestroyShaders() {
    glDeleteProgram( BasePassProgram );
    glDeleteProgram( LightingPassProgram );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateShader                                                                                              //
//                                                                                                           //
// Creates shader object of a given type from given file                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static GLuint CreateShader( const std::string i_Filename,
                            const GLenum      i_Type ) {
    std::string source_code;

    // Read source code from selected file
    GetTextfileContents( i_Filename, source_code );

    const char* code = source_code.c_str();
    int length = source_code.length();

    // Create shader object
    GLuint shader = glCreateShader( i_Type );
    // Store source code in shader object
    glShaderSource( shader, 1, &code, &length );
    // Compile shader
    glCompileShader( shader );

    GLint status;

    // Check compilation status of a shader
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status);
    if( status == GL_FALSE ) {
        GLchar compilation_info[1000];
        // Check what went wrong and display message box with information
        glGetShaderInfoLog( shader, 1000, nullptr, compilation_info );
        ErrorMessage( "Shader Initialization Error", compilation_info );
        // Delete created shader object
        glDeleteShader( shader );
    }
    return shader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetTextfileContents                                                                                       //
//                                                                                                           //
// Stores contents of a text file in a given variable                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void GetTextfileContents( const std::string i_Filename,
                                 std::string&      o_FileContents ) {
    std::stringstream contents_stream;

    // Open selected file
    std::ifstream file( i_Filename );
    if( file.fail() ) {
        return;
    }

    // Read data from file
    contents_stream << file.rdbuf();
    file.close();

    // Store file contents
    o_FileContents = contents_stream.str();
}