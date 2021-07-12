#include "LUtil.h"
#include "LTexture.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include <string>

LTexture gLoadedTexture;
std::string path_exe;
int poopsies = 0;

void getPath() {
    char path_exe_temp[1024];
    GetModuleFileName(NULL, path_exe_temp, 1024);
    int pos=std::string(path_exe_temp).find_last_of("\\/");
    path_exe=std::string(path_exe_temp).substr( 0, pos+1);
}

bool initGL(){
    getPath();
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,SCREEN_WIDTH,SCREEN_HEIGHT,0.0,1.0,-1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable( GL_TEXTURE_2D );
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        printf("Error initializing OpenGL! %s\n",gluErrorString(error));
        return false;
    }
    ilInit();
    ilClearColor(255,255,255,000);
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! %s\n", iluErrorString( ilError ) );
        return false;
    }

    return true;
}

bool loadMedia()
{
    //Load texture
    if(!gLoadedTexture.loadTextureFromFile(path_exe+"texture.png"))
    {
        printf( "Unable to load file texture!\n" );
        return false;
    }

    return true;
}






void update(){

}

void render(){
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    //Calculate centered offsets
    GLfloat x = ( SCREEN_WIDTH - gLoadedTexture.textureWidth() ) / 4.f;
    GLfloat y = ( SCREEN_HEIGHT - gLoadedTexture.textureHeight() ) / 4.f;

    //Render texture
    gLoadedTexture.render( x, y );

    //Update screen
    glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y ){
           if( key == 'q' ){
               poopsies=poopsies+1;
    } else if( key == 'e' ){
    }
}
