#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include "GameConstants.h"
#include <windows.h>
#include "windowOGL.h"
#include "cWNDManager.h"
#include "libdrawtext-0.2.1\drawtext.h"
#include "cModelLoader.h"
#include "cModel.h"
#include "cEnemy.h"
#include "cPlayer.h"
#include "cLaser.h"
#include "cSound.h"

//#include "cCamera.h"
#include <vector>
#include <time.h>

#define FONT_SZ	24
#define FONT_SZ2 50

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;
	
	

    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

    //The example OpenGL code
    windowOGL theOGLWnd;

    //Attach our example to our window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);


    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL()) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	srand(time(NULL));

	//Load models
	cModelLoader enemyMdl;
	enemyMdl.loadModel("Models/Platformer.obj");
	

	cModelLoader playerMdl;
	playerMdl.loadModel("Models/Jack_Skellington.obj"); // Player

	cModelLoader bullitMdl;
	bullitMdl.loadModel("Models/missile.obj");

	cModelLoader terrainMdl;
	terrainMdl.loadModel("Models/terrain.obj");

	cModelLoader castleMdl;
	castleMdl.loadModel("Models/castle.obj");

	std::vector<cEnemy*> enemyList;
	std::vector<cEnemy*>::iterator enemyIndex;
	//create 5 enemies
	for (int loop = 0; loop < 5; loop++)
	{
		cEnemy* enemy = new cEnemy();
		enemy->randomise();
		enemy->setMdlDimensions(enemyMdl.getModelDimensions());
		enemy->setScale(glm::vec3(5, 5, 5));
		enemyList.push_back(enemy);
	}

	//initialise the player
	cPlayer thePlayer;
	thePlayer.initialise(glm::vec3(0, 10, 0), 180.0f, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 5.0f, true);
	thePlayer.setMdlDimensions(playerMdl.getModelDimensions());
	thePlayer.setScale(glm::vec3(0.05f, 0.05f, 0.05f));
	
	cPlayer terrain;

	//set the position and size of the terrain
	terrain.setPosition(glm::vec3(0, thePlayer.getPosition().y-10, 0));
	terrain.setScale(glm::vec3(100, 1, 100));
	std::vector<cLaser*> laserList;
	std::vector<cLaser*>::iterator index;

	cPlayer theCastle;

	//set the position and size of the castle
	theCastle.setPosition(glm::vec3(0, 0, -200));
	theCastle.setScale(glm::vec3(7,7,7));
	//cLaser laserList[30];
	//int numShots = 0;

	// Load font
	struct dtx_font *fntmain;

	fntmain = dtx_open_font("Fonts/SHOWG.ttf", 0);
	dtx_prepare_range(fntmain, FONT_SZ, 0, 256);             /* ASCII */

	dtx_use_font(fntmain, FONT_SZ);

	// Load Sound
	cSound themeMusic;
	themeMusic.createContext();
	themeMusic.loadWAVFile("Audio/themeMusic.wav");
	themeMusic.playAudio(AL_LOOPING);
	
	// explosion
	cSound explosionFX;
	explosionFX.createContext();
	explosionFX.loadWAVFile("Audio/explosion2.wav");

	//firing sound
	cSound firingFX;
	firingFX.createContext();
	firingFX.loadWAVFile("Audio/fireBall.wav");

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		//initialise the window and set background colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		theOGLWnd.initOGL();
		glClearColor(1,0, 0, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		//if the game hasn't been started yet don't do anything 
		if (start == true)
		{

		}
		//if game has already begun
		else if (gameEnd == false)
		{
			
			
			if (bullits == 0)
			{
				gameEnd = true;
			}
			//check which camera perspective has been selected and set view accordingly
			if (ortho == false)
			{
				gluLookAt(0, 10, 150, 0, 10, 0, 0, 1, 0);
			}
			else if (ortho == true)
			{
				switch (turns){
				case 0:
					gluLookAt(thePlayer.getPosition().x, 12, thePlayer.getPosition().z* -1, thePlayer.getPosition().x, 10, (thePlayer.getPosition().z)*-1 - 10, 0, 1, 0);
					break;
				case 1:
					gluLookAt(thePlayer.getPosition().x, 12, thePlayer.getPosition().z*-1, thePlayer.getPosition().x + 10, 10, (thePlayer.getPosition().z)*-1, 0, 1, 0);
					break;
				case 2:
					gluLookAt(thePlayer.getPosition().x, 12, thePlayer.getPosition().z*-1, thePlayer.getPosition().x, 10, (thePlayer.getPosition().z*-1) + 10, 0, 1, 0);
					break;
				case 3:
					gluLookAt(thePlayer.getPosition().x, 12, thePlayer.getPosition().z*-1, thePlayer.getPosition().x - 10, 10, (thePlayer.getPosition().z)*-1, 0, 1, 0);

					break;
				}

			}


			//Do any pre-rendering logic
			//Render the scene
			//Drawing the model
			//glTranslatef(0, -10,0);
			//for each of the enemies set the radius and render the model 
			for (enemyIndex = enemyList.begin(); enemyIndex != enemyList.end(); enemyIndex++)
			{
				(*enemyIndex)->setMdlRadius(10);
				//tardisMdl.renderMdl(enemyList[loop].getPosition(), enemyList[loop].getRotation(), enemyList[loop].getScale());
				//enemyList[loop].update(elapsedTime);
				enemyMdl.renderMdl((*enemyIndex)->getPosition(), (*enemyIndex)->getRotation(), (*enemyIndex)->getScale());
				(*enemyIndex)->update(elapsedTime);
			}

			//render the player model
			playerMdl.renderMdl(thePlayer.getPosition(), thePlayer.getRotation(), thePlayer.getScale());
			thePlayer.update(elapsedTime);

			//position and render the castle
			glTranslatef(0, 40, 0);
			castleMdl.renderMdl(glm::vec3(0, 0, 800), 0, theCastle.getScale());

			//position and render the terrain
			glTranslatef(0, -50, 0);
			terrainMdl.renderMdl(glm::vec3(0, -10, 0), 0, terrain.getScale());
			glTranslatef(0, 15, 0);
			//terrain->update(elapsedTime);

			////are we shooting?
			if (fire)
			{
				cLaser* laser = new cLaser();
				glm::vec3 mdlLaserDirection;
				/*mdlLaserDirection.x = -(float)glm::sin(thePlayer.getRotation());
				mdlLaserDirection.y = 0.0f;
				mdlLaserDirection.z = (float)glm::cos(thePlayer.getRotation());
				mdlLaserDirection *= -1;*/

				//check which way the player is facing and set the weapon direction accordingly
				switch (turns){
				case 0:
					mdlLaserDirection.x = 0.0f;
					mdlLaserDirection.y = 0.0f;
					mdlLaserDirection.z = 1.0f;
					laser->setRotation(-90.0f);
					break;
				case 1:
					mdlLaserDirection.x = 1.0f;
					mdlLaserDirection.y = 0.0f;
					mdlLaserDirection.z = 0.0f;
					laser->setRotation(180.0f);
					break;
				case 2:
					mdlLaserDirection.x = 0.0f;
					mdlLaserDirection.y = 0.0f;
					mdlLaserDirection.z = -1.0f;
					laser->setRotation(90.0f);
					break;
				case 3:
					mdlLaserDirection.x = -1.0f;
					mdlLaserDirection.y = 0.0f;
					mdlLaserDirection.z = 0.0f;
					break;


				}
				laser->setDirection(mdlLaserDirection);   ///     glm::vec3(0, 0, 5)

				//set the values for the weapon
				laser->setScale(glm::vec3(1, 1, 1));
				laser->setSpeed(5.0f);
				laser->setPosition(thePlayer.getPosition() + mdlLaserDirection);    //   glm::vec3(0, 0, 0)
				laser->setIsActive(true);
				laser->setMdlDimensions(bullitMdl.getModelDimensions());
				laser->update(elapsedTime);
				laserList.push_back(laser);
				fire = false;
				firingFX.playAudio(AL_FALSE);
				//numShots++;
			}

			//for (int loop = 0; loop < numShots; loop++)
			//{
			//	theLaser.renderMdl(laserList[loop].getPosition(), laserList[loop].getRotation());
			//	laserList[loop].update(elapsedTime);
			//}

			//for each weapon fired render the model and check for collisions
			for (index = laserList.begin(); index != laserList.end(); ++index)
			{
				if ((*index)->isActive())
				{

					//glScalef(3, 3, 3);

					bullitMdl.renderMdl((*index)->getPosition(), (*index)->getRotation(), (*index)->getScale());

					(*index)->update(elapsedTime);

					// check for collisions
					for (enemyIndex = enemyList.begin(); enemyIndex != enemyList.end(); enemyIndex++)
					{
						//enemyList[loop].update(elapsedTime);
						if ((*index)->SphereSphereCollision((*enemyIndex)->getPosition(), (*enemyIndex)->getMdlRadius()))
						{
							//enemyList[loop].setIsActive(false);
							(*enemyIndex)->setIsActive(false);
							enemyList.erase(enemyIndex);
							num++;
							explosionFX.playAudio(AL_FALSE);
							break; // No need to check for other bullets.
						}
					}
				}
			}
			if (num == 4)
			{
				for (int loop = 0; loop < 5; loop++)
				{
					cEnemy* enemy = new cEnemy();
					enemy->randomise();
					enemy->setMdlDimensions(enemyMdl.getModelDimensions());
					enemy->setScale(glm::vec3(5, 5, 5));
					enemyList.push_back(enemy);
				}
			}
			
		}
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0, windowWidth, 0, windowHeight);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glPushAttrib(GL_DEPTH_TEST);
			glDisable(GL_DEPTH_TEST);

			glTranslatef(50, 50, 0);
			//when the game is first started set the font and display the instructions
			if (start == true)
			{
				dtx_prepare_range(fntmain, FONT_SZ2, 0, 256);             /* ASCII */

				dtx_use_font(fntmain, FONT_SZ2);
				glTranslatef(70, 550, 0);

				const char *startScreen = "Welcome to Cirno's Castle!!!";
				const char *cameraControls = "Use the 'C' and 'V' keys to toggle the cameras";
				const char *playerControls1 = "Up and down arrows move the player forward and backwards";
				const char *playerControls2 = "Left and right arrows rotate the player";
				const char *playerControls3 = "Space bar fires weapon";
				const char *startGame = "Press 'S' to begin";
				dtx_string(startScreen);
				dtx_prepare_range(fntmain, FONT_SZ, 0, 256);             /* ASCII */

				dtx_use_font(fntmain, FONT_SZ);
				glTranslatef(0, -50, 0);
				dtx_string(cameraControls);

				glTranslatef(0, -50, 0);
				dtx_string(playerControls1);

				glTranslatef(0, -50, 0);
				dtx_string(playerControls2);

				glTranslatef(0, -50, 0);
				dtx_string(playerControls3);

				glTranslatef(100, -100, 0);
				dtx_string(startGame);

			}
			else if (gameEnd == false)
			{
				//once the game has begun show the number of enemies killed
				std::string s = std::to_string(num);
				char const *pchar = s.c_str();
				char const *guff = "Enemies killed: ";
				std::string bull = std::to_string(bullits);
				char const *bullit = bull.c_str();
				char const *ammo = "Ammo: ";
				dtx_string(guff);
				glTranslatef(200, 0, 0);
				dtx_string(pchar);

				glTranslatef(500, 0, 0);
				dtx_string(ammo);
				glTranslatef(100, 0, 0);
				dtx_string(bullit);
			}
			else if (gameEnd == true)
			{
				glTranslatef(0,500,0);
				std::string t = std::to_string(num);
				char const *qchar = t.c_str();
				char const *finish = "You Scored: ";
				dtx_string(finish);
				glTranslatef(200, 0, 0);
				dtx_string(qchar);

				const char *retry = "To try again press 'R' or 'Esc' to quit";
				glTranslatef(100, -200, 0);
				dtx_string(retry);

			}
			
		
			

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		pgmWNDMgr->swapBuffers();
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
