#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "LOpenGL.h"
#include <stdio.h>
#include <string>
#include "LFRect.h"

class LTexture
{
    public:
        LTexture();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Initializes member variables
        Side Effects:
         -None
        */

        ~LTexture();
        /*
        Pre Condition:
         -None
        Post Condition:
         -Frees texture
        Side Effects:
         -None
        */

        bool loadTextureFromPixels32( GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight );
        /*
        Pre Condition:
         -A valid OpenGL context
        Post Condition:
         -Creates a texture from the given pixels
         -Reports error to console if texture could not be created
        Side Effects:
         -Binds a NULL texture
        */
        bool loadTextureFromFile( std::string path );

        void freeTexture();
        /*
        Pre Condition:
         -A valid OpenGL context
        Post Condition:
         -Deletes texture if it exists
         -Sets texture ID to 0
        Side Effects:
         -None
        */

        void render( GLfloat x, GLfloat y, LFRect* clip );
        /*
        Pre Condition:
         -A valid OpenGL context
         -Active modelview matrix
        Post Condition:
         -Translates to given position and renders textured quad
        Side Effects:
         -Binds member texture ID
        */

        GLuint getTextureID();
        GLuint textureWidth();
        GLuint textureHeight();
        GLuint imageWidth();
        GLuint imageHeight();
    private:
        GLuint powerOfTwo( GLuint num );

        //Texture name
        GLuint mTextureID;

        //Texture dimensions
        GLuint mTextureWidth;
        GLuint mTextureHeight;

        //Unpadded image dimensions
        GLuint mImageWidth;
        GLuint mImageHeight;
};

#endif
