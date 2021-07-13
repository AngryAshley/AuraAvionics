#include "LUtil.h"
#include <string>

using namespace std;

bool debug = true;
string version="0.1";
std::string path_exe;

void getPath() {
    char path_exe_temp[1024];
    GetModuleFileName(NULL, path_exe_temp, 1024);
    int pos=std::string(path_exe_temp).find_last_of("\\/");
    path_exe=std::string(path_exe_temp).substr( 0, pos+1);
}

void runMainLoop(int val);

int main(int argc, char* args[]){
    if(argc>1){ //handle arguments before running.
        bool enableConsole=false;
        if(!strcmp("-d",args[1])){enableConsole=true;};

        if(enableConsole){AllocConsole();freopen("CONOUT$","a",stdout);SetConsoleTitle("Aura Virtual Cockpit Debug Console");}
    }

    printf("Initializing Program\n");
    getPath();setLUtilPath(path_exe); //Handle path variable
    printf("Initializing Graphics... \n");
	glutInit(&argc, args);
	glutInitContextVersion(2,1);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Aura Virtual Cockpit");
	if(!initGL()){
		printf("ERROR: Unable to initialize OpenGL!\n");return 1;
	}
	printf("Done\nInitializing media... ");
	if(!loadMedia()){
        printf( "Unable to load media!\n" );return 2;
    }
    printf("Done\n");
    glutKeyboardFunc(handleKeys);
	glutDisplayFunc(render);
	glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);


	printf("Initialization finished\n\n *****************************************************\n *** Welcome to AuraAvionics Virtual Cockpit V %s ***\n *****************************************************\n",version.c_str());
	glutMainLoop();

	return 0;
}

void runMainLoop( int val ){
    update();
    render();

    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}
