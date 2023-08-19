#pragma once

#include <ctime>

#include "../native/types.h"

using namespace std;

const Hash BLIP_STYLE_ENEMY = 0x318C617C;
const Hash BLIP_TYPE_BOUNTY_TARGET = 0x38CDE89D;
const Hash BLIP_SPRITE_BOUNTY_TARGET = 0x5846C31D;

// const char* SKINNER_BROTHER_MODEL = "g_m_m_unimountainmen_01";
// const char* F_LOWER_TOWN_FOLK = "A_F_M_LowerSDTownfolk_01";
// const char* M_LOWER_TOWN_FOLK = "A_M_M_LowerSDTownfolk_01";
// const char* M_BOUNTY_TARGET = "G_M_M_UniCriminals_02";
// const char* M_BOUNTY_MEXICAN = "G_M_M_UniBanditos_01";
// const char* M_BOUNTY_FANCY = "A_M_M_SDFANCYTRAVELLERS_01";


enum RaycastIntersectionOptions
{
	Everything = -1,
	Map = 1,
	MissionEntities,
	Peds1 = 12,
	Objects = 16,
	Unk1 = 32,
	Unk2 = 64,
	Unk3 = 128,
	Vegetation = 256,
	Unk4 = 512
};

Vector3 getRandomPositionInRange(Vector3 source, int radius);
Vector3 getRandomPedPositionInRange(Vector3 source, int radius);
void getGroundPos(Vector3* position);
void getGroundPos(Vector3 originalPos, Vector3* outPos);
float getGroundPos(Vector3 originalPos);
float distanceBetweenEntities(Entity entity1, Entity entity2);
float distanceBetweenEntitiesHor(Entity entity1, Entity entity2);
Object createProp(char* model, Vector3 position, float heading = 0, bool isStatic = false, bool isVisible = true);
// RaycastResult raycast(Vector3 source, Vector3 direction, float maxDist, RaycastIntersectionOptions intersectionOptions = Everything);
//void releaseEntitySafe(Entity* entity);
tm getGameTime();
float random_angle();
Vector3 random_position_around(const Vector3& center, float distance,bool safe_coords = false);