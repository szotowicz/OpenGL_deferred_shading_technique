#include <cstdio>
#include "TextureLoader.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// STGA                                                                                                      //
//                                                                                                           //
// Structure for handling TGA files                                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct STGA {
    STGA() {
        Data = nullptr;
        Width = 0;
        Height = 0;
        ByteCount = 0;
    }

    ~STGA() {
        delete[] Data;
        Data = nullptr;
    }

    void destroy() {
        delete[] Data;
        Data = nullptr;
    }

    size_t          Width;
    size_t          Height;
    unsigned char   ByteCount;
    unsigned char*  Data;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateTextureFromTGA                                                                                      //
//                                                                                                           //
// Loads image from TGA file and creates OpenGL texture from it                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CreateTextureFromTGA( const char* i_Filename,
                           GLuint&     o_TextureID ) {
    STGA tga_file;

    // Load contents of a TGA file
    if( !LoadTGA( i_Filename, tga_file ) ) {
        ErrorMessage("Error Creating Texture", "Could not load image from TGA file." );
        return false;
    }

    // Create and bind texture
    glGenTextures( 1, &o_TextureID);
    glBindTexture( GL_TEXTURE_2D, o_TextureID );

    // Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );     // Set linear filtering for magnification
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );     // Set linear filtering for minification
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );         // Repeat X texture coordinates around object
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );         // Repeat Y texture coordinates around object

    // Fill texture with data loaded from file
    glTexImage2D( GL_TEXTURE_2D,        // 2D texture
                  0,                    // 0 MIP level
                  GL_RGBA8,             // 8 bits per channel
                  tga_file.Width,       // Texture width
                  tga_file.Height,      // Texture height
                  0,                    // No border
                  GL_RGBA,              // 4-component texture
                  GL_UNSIGNED_BYTE,     // Data type for each component
                  tga_file.Data );      // Pointer to texture data that should be loaded onto graphics card memory
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LoadTGA                                                                                                   //
//                                                                                                           //
// Load image from given TGA file                                                                            //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool LoadTGA( const char* i_Filename,
                     STGA&       o_TGAFile ) {
    FILE *file;
    unsigned char type[4];
    unsigned char info[6];

    // Check if file exists
    file = fopen( i_Filename, "rb" );
    if( !file ) {
        return false;
    }

    // Load TGA file header
    fread (&type, sizeof (char), 3, file);
    fseek (file, 12, SEEK_SET);
    fread (&info, sizeof (char), 6, file);

    // Check image type
    if( (type[1] != 0) || ((type[2] != 2) && (type[2] != 3)) ) {
        fclose(file);
        return false;
    }

    // Get image dimensions
    o_TGAFile.Width     = info[0] + info[1] * 256;
    o_TGAFile.Height    = info[2] + info[3] * 256;
    o_TGAFile.ByteCount = info[4] / 8;
    if( (o_TGAFile.ByteCount != 3) && (o_TGAFile.ByteCount != 4) ) {
        fclose(file);
        return false;
    }

    // Calculate image size and read date from file
    size_t image_size = o_TGAFile.Width * o_TGAFile.Height * o_TGAFile.ByteCount;
    // Allocate memory for image data
    o_TGAFile.Data = new unsigned char[image_size];
    // Read image data
    fread( o_TGAFile.Data, sizeof(unsigned char), image_size, file );
    
    // Convert colors from BGRA format to RGBA
    for( size_t i = 0; i < image_size; i += 4 ) {
        unsigned char B = o_TGAFile.Data[i + 0];
        unsigned char G = o_TGAFile.Data[i + 1];
        unsigned char R = o_TGAFile.Data[i + 2];
        unsigned char A = o_TGAFile.Data[i + 3];
        o_TGAFile.Data[i + 0] = R;
        o_TGAFile.Data[i + 1] = G;
        o_TGAFile.Data[i + 2] = B;
        o_TGAFile.Data[i + 3] = A;
    }

    // Close file
    fclose( file );
    return true;
}
