#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

#define GLM_FORCE_RADIANS

#include <stdlib.h>
#include <Windows.h>
#include <GL\glut.h>
//#include <assimp\Importer.hpp>    // C++ importer interface
//#include <assimp\scene.h>           // Output data structure
//#include <assimp\postprocess.h>     // Post processing flags
#include <al.h>
#include <alc.h>
#include <alut.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <geometric.hpp>
#include <gtc\quaternion.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\quaternion.hpp>
#include "glm-0.3.2\glm\glm.h"
#include <vector>


//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "OpenGl Models, Lighting and Text"

struct mdlDimensions{
	float s_mdlWidth, s_mdlheight, s_mdldepth;
};

const float PLAYFIELDX  = 100.0f;
const float PLAYFIELDZ = 300.0f;

extern int drawMode;
extern float rotationAngle;
extern float translationZ;
extern bool fire;


#endif