#include "cPlayer.h"

cPlayer::cPlayer() : cModel()
{

}

void cPlayer::update(float elapsedTime)
{
	// Find out what direction we should be thrusting, using rotation.
	glm::vec3 mdlVelocityAdd;
	/*mdlVelocityAdd.x = -(float)glm::sin(cModel::m_mdlRotation);
	mdlVelocityAdd.y = 0.0f;
	mdlVelocityAdd.z = -(float)glm::cos(cModel::m_mdlRotation);*/

	//check which way the player is facing and translate the player accordingly
	switch (turns){
	case 0:
		mdlVelocityAdd.z += 1;
		break;
	case 1:
		mdlVelocityAdd.x += 1;
		break;
	case 2:
		mdlVelocityAdd.z -= 1;
		break;
	case 3:
		mdlVelocityAdd.x -= 1;
		break;
	}
	cModel::m_mdlRotation -= rotationAngle;

	mdlVelocityAdd *= translationZ;
	cModel::m_mdlDirection += mdlVelocityAdd;

	cModel::m_mdlPosition += cModel::m_mdlDirection * cModel::m_mdlSpeed *elapsedTime;
	cModel::m_mdlDirection *= 0.95f;

	rotationAngle = 0;
	translationZ = 0;
}

cPlayer::~cPlayer()
{

}