#include "LTexture.h"
#include <IL/il.h>
#include <IL/ilu.h>

LTexture::LTexture(){
    //Initialize texture ID
    mTextureID = 0;

    //Initialize image dimensions
    mImageWidth = 0;
    mImageHeight = 0;

    //Initialize texture dimensions
    mTextureWidth = 0;
    mTextureHeight = 0;
}

LTexture::~LTexture(){
    freeTexture();
}

bool LTexture::loadTextureFromPixels32( GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight )
{
    //Free texture if it exists
    freeTexture();

    //Get image dimensions
    mImageWidth = imgWidth;
    mImageHeight = imgHeight;
    mTextureWidth = texWidth;
    mTextureHeight = texHeight;

    //Generate texture ID
    glGenTextures( 1, &mTextureID );

    //Bind texture ID
    glBindTexture( GL_TEXTURE_2D, mTextureID );

    //Generate texture
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    //Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    //Unbind texture
    glBindTexture( GL_TEXTURE_2D, NULL );

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
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
    ILboolean success = ilLoadImage( path.c_str() );

    //Image loaded successfully
    if( success == IL_TRUE )
    {
        //Convert image to RGBA
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
        if( success == IL_TRUE )
        {
            //Initialize dimensions
            GLuint imgWidth = (GLuint)ilGetInteger( IL_IMAGE_WIDTH );
            GLuint imgHeight = (GLuint)ilGetInteger( IL_IMAGE_HEIGHT );
            //Calculate required texture dimensions
            GLuint texWidth = powerOfTwo( imgWidth );
            GLuint texHeight = powerOfTwo( imgHeight );

            //Texture is the wrong size
            if( imgWidth != texWidth || imgHeight != texHeight )
            {
                //Place image at upper left
                iluImageParameter( ILU_PLACEMENT, ILU_UPPER_LEFT );

                //Resize image
                iluEnlargeCanvas( (int)texWidth, (int)texHeight, 1 );
            }
                        //Create texture from file pixels
            textureLoaded = loadTextureFromPixels32( (GLuint*)ilGetData(), imgWidth, imgHeight, texWidth, texHeight );
        }

        //Delete file from memory
        ilDeleteImages( 1, &imgID );
    }

    //Report error
    if( !textureLoaded )
    {
        printf( "Unable to load %s\n", path.c_str() );
    }

    return textureLoaded;
}

bool LTexture::loadPixelsFromFile( std::string path )
{
    //Deallocate texture data
    freeTexture();

    //Texture loading success
    bool pixelsLoaded = false;

    //Generate and set current image ID
    ILuint imgID = 0;
    ilGenImages( 1, &imgID );
    ilBindImage( imgID );

    //Load image
    ILboolean success = ilLoadImage( path.c_str() );

    //Image loaded successfully
    if( success == IL_TRUE )
    {
        //Convert image to RGBA
        success = ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );
        if( success == IL_TRUE )
        {
            //Initialize dimensions
            GLuint imgWidth = (GLuint)ilGetInteger( IL_IMAGE_WIDTH );
            GLuint imgHeight = (GLuint)ilGetInteger( IL_IMAGE_HEIGHT );

            //Calculate required texture dimensions
            GLuint texWidth = powerOfTwo( imgWidth );
            GLuint texHeight = powerOfTwo( imgHeight );

            //Texture is the wrong size
            if( imgWidth != texWidth || imgHeight != texHeight )
            {
                //Place image at upper left
                iluImageParameter( ILU_PLACEMENT, ILU_UPPER_LEFT );

                //Resize image
                iluEnlargeCanvas( (int)texWidth, (int)texHeight, 1 );
            }
                        //Allocate memory for texture data
            GLuint size = texWidth * texHeight;
            mPixels = new GLuint[ size ];

            //Get image dimensions
            mImageWidth = imgWidth;
            mImageHeight = imgHeight;
            mTextureWidth = texWidth;
            mTextureHeight = texHeight;

            //Copy pixels
            memcpy( mPixels, ilGetData(), size * 4 );
            pixelsLoaded = true;
        }

        //Delete file from memory
        ilDeleteImages( 1, &imgID );
    }

    //Report error
    if( !pixelsLoaded )
    {
        printf( "Unable to load %s\n", path.c_str() );
    }

    return pixelsLoaded;
}
bool LTexture::loadTextureFromPixels32()
{
    //Loading flag
    bool success = true;

    //There is loaded pixels
    if( mTextureID == 0 && mPixels != NULL )
    {
        //Generate texture ID
        glGenTextures( 1, &mTextureID );

        //Bind texture ID
        glBindTexture( GL_TEXTURE_2D, mTextureID );

        //Generate texture
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels );

        //Set texture parameters
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

        //Unbind texture
        glBindTexture( GL_TEXTURE_2D, NULL );

        //Check for error
        GLenum error = glGetError();
        if( error != GL_NO_ERROR )
        {
            printf( "Error loading texture from %p pixels! %s\n", mPixels, gluErrorString( error ) );
            success = false;
        }
        else
        {
            //Release pixels
            delete[] mPixels;
            mPixels = NULL;
        }
    }
        //Error
    else
    {
        printf( "Cannot load texture from current pixels! " );

        //Texture already exists
        if( mTextureID != 0 )
        {
            printf( "A texture is already loaded!\n" );
        }
        //No pixel loaded
        else if( mPixels == NULL )
        {
            printf( "No pixels to create texture from!\n" );
        }

        success = false;
    }

    return success;
}

bool LTexture::loadTextureFromFileWithColorKey( std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
    //Load pixels
    if( !loadPixelsFromFile( path ) )
    {
        return false;
    }

    //Go through pixels
    GLuint size = mTextureWidth * mTextureHeight;
    for( int i = 0; i < size; ++i )
    {
        //Get pixel colors
        GLubyte* colors = (GLubyte*)&mPixels[ i ];

        //Color matches
        if( colors[ 0 ] == r && colors[ 1 ] == g && colors[ 2 ] == b && ( 0 == a || colors[ 3 ] == a ) )
        {
            //Make transparent
            colors[ 0 ] = 255;
            colors[ 1 ] = 255;
            colors[ 2 ] = 255;
            colors[ 3 ] = 000;
        }
    }

    //Create texture
    return loadTextureFromPixels32();
}


void LTexture::freeTexture(){
    //Delete texture
    if( mTextureID != 0 )
    {
        glDeleteTextures( 1, &mTextureID );
        mTextureID = 0;
    }

    mImageWidth = 0;
    mImageHeight = 0;
    mTextureWidth = 0;
    mTextureHeight = 0;
}

void LTexture::render( GLfloat x, GLfloat y, LFRect* clip ){
    //If the texture exists
    if( mTextureID != 0 )
    {
        //Remove any previous transformations
        glLoadIdentity();

        //Texture coordinates
        GLfloat texTop = 0.f;
        GLfloat texBottom = (GLfloat)mImageHeight / (GLfloat)mTextureHeight;
        GLfloat texLeft = 0.f;
        GLfloat texRight = (GLfloat)mImageWidth / (GLfloat)mTextureWidth;

        //Vertex coordinates
        GLfloat quadWidth = mImageWidth;
        GLfloat quadHeight = mImageHeight;
        //Handle clipping
        if( clip != NULL )
        {
            //Texture coordinates
            texLeft = clip->x / mTextureWidth;
            texRight = ( clip->x + clip->w ) / mTextureWidth;
            texTop = clip->y / mTextureHeight;
            texBottom = ( clip->y + clip->h ) / mTextureHeight;

            //Vertex coordinates
            quadWidth = clip->w;
            quadHeight = clip->h;
        }

        //Move to rendering point
        glTranslatef( x, y, 0.f );

        //Set texture ID
        glBindTexture( GL_TEXTURE_2D, mTextureID );

        //Render textured quad
        glBegin( GL_QUADS );
            glTexCoord2f(  texLeft,    texTop ); glVertex2f(       0.f,        0.f );
            glTexCoord2f( texRight,    texTop ); glVertex2f( quadWidth,        0.f );
            glTexCoord2f( texRight, texBottom ); glVertex2f( quadWidth, quadHeight );
            glTexCoord2f(  texLeft, texBottom ); glVertex2f(       0.f, quadHeight );
        glEnd();
    }
}


GLuint LTexture::getTextureID() {return mTextureID;}
GLuint LTexture::textureWidth() {return mTextureWidth;}
GLuint LTexture::textureHeight(){return mTextureHeight;}
GLuint LTexture::imageWidth()   {return mImageWidth;}
GLuint LTexture::imageHeight()  {return mImageHeight;}

GLuint LTexture::powerOfTwo( GLuint num )
{
    if( num != 0 )
    {
        num--;
        num |= (num >> 1); //Or first 2 bits
        num |= (num >> 2); //Or next 2 bits
        num |= (num >> 4); //Or next 4 bits
        num |= (num >> 8); //Or next 8 bits
        num |= (num >> 16); //Or next 16 bits
        num++;
    }

    return num;
}
