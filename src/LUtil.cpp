#include "LUtil.h"
#include "LTexture.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include <fstream>


LTexture gNon2NTexture;
LFRect logo;

int poopsies = 0;
std::string EXEpath;



bool initGL(){
    printf("Initializing OpenGL...");
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,SCREEN_WIDTH,SCREEN_HEIGHT,0.0,1.0,-1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        printf("Error initializing OpenGL! %s\n",gluErrorString(error));
        return false;
    }
    printf("Done\nInitializing DevIL... ");
    ilInit();
    iluInit();
    ilClearColor(255,255,255,000);
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }

    return true;
}
void setLUtilPath(std::string data){
    EXEpath=data;
    return;
}

bool loadMedia()
{
    logo.x=0.f;
    logo.y=0.f;
    logo.w=978.f;
    logo.h=512.f;
    //Load texture
    if(!gNon2NTexture.loadTextureFromFileWithColorKey(EXEpath+"assets/mainLogo.png",000,000,000))
    {
        printf( "Unable to load file texture!\n" );
        return false;
    }
    logo.w=(float)gNon2NTexture.imageWidth();
    logo.h=(float)gNon2NTexture.imageHeight();
    return true;
}


void update(){

}

void render(){
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Calculate centered offsets
    //GLfloat x = ( SCREEN_WIDTH - gNon2NTexture.textureWidth() ) / 4.f;
    //GLfloat y = ( SCREEN_HEIGHT - gNon2NTexture.textureHeight() ) / 4.f;

    //Render texture
    gNon2NTexture.render( ( SCREEN_WIDTH - gNon2NTexture.imageWidth() ) / 2.f, ( SCREEN_HEIGHT - gNon2NTexture.imageHeight() ) / 2.f ,&logo);

    //Update screen
    glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y ){
           if( key == 'q' ){
               poopsies=poopsies+1;
    } else if( key == 'e' ){
    }
}
