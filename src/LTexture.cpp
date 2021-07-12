#include "LTexture.h"
#include <IL/il.h>

LTexture::LTexture(){
    mTextureID = 0;
    mTextureWidth = 0;
    mTextureHeight = 0;
}

LTexture::~LTexture(){
    freeTexture();
}

bool LTexture::loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height ){
    freeTexture();
    mTextureWidth = width;
    mTextureHeight = height;
    glGenTextures( 1, &mTextureID );//Generate texture ID

    glBindTexture( GL_TEXTURE_2D, mTextureID ); //binding a texture sets the CURRENT texture to this one
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glBindTexture( GL_TEXTURE_2D, NULL );//Unbind texture

    GLenum error = glGetError();
    if( error != GL_NO_ERROR ){
        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }
    return true;
}

bool LTexture::loadTextureFromFile( std::string path ){
    //Texture loading success
    bool textureLoaded = false;
    //Generate and set current image ID
    ILuint imgID = 0;
    ilGenImages( 1, &imgID );
    ilBindImage( imgID );
        //Load image
    ILboolean success = ilLoadImage(path.c_str()); //
    //Image loaded successfully
    if( success == IL_TRUE )
    {
        //Convert image to RGBA
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
                if( success == IL_TRUE )
        {
            //Create texture from file pixels
            textureLoaded = loadTextureFromPixels32( (GLuint*)ilGetData(), (GLuint)ilGetInteger( IL_IMAGE_WIDTH ), (GLuint)ilGetInteger( IL_IMAGE_HEIGHT ) );
        }

        //Delete file from memory
        ilDeleteImages( 1, &imgID );
    }

    //Report error
    if( !textureLoaded )
    {
        ILenum err = ilGetError();
        printf( "Unable to load %s, error %d\n", path.c_str(), err );

    }

    return textureLoaded;
}

void LTexture::freeTexture(){
    if( mTextureID != 0 ){
        glDeleteTextures( 1, &mTextureID );
        mTextureID = 0;
    }
    mTextureWidth = 0;
    mTextureHeight = 0;
}

void LTexture::render( GLfloat x, GLfloat y ){
    if( mTextureID != 0 ){
        glLoadIdentity();//Remove any previous transformations
        glTranslatef( x, y, 0.f );//Move to rendering point

        glBindTexture( GL_TEXTURE_2D, mTextureID );
        glBegin( GL_QUADS );
            glTexCoord2f( 0.f, 0.f ); glVertex2f(           0.f,            0.f );
            glTexCoord2f( 1.f, 0.f ); glVertex2f( mTextureWidth,            0.f );
            glTexCoord2f( 1.f, 1.f ); glVertex2f( mTextureWidth, mTextureHeight );
            glTexCoord2f( 0.f, 1.f ); glVertex2f(           0.f, mTextureHeight );
        glEnd();
    }
}

GLuint LTexture::getTextureID() {return mTextureID;}
GLuint LTexture::textureWidth() {return mTextureWidth;}
GLuint LTexture::textureHeight(){return mTextureHeight;}
