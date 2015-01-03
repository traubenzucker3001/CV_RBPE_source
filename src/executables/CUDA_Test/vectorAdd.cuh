#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//void vectorAdd(glm::quat* A, glm::quat* B, glm::quat* C, int numEl);
void vectorAdd(int* A, glm::ivec4* B, int numEl);


//pMass, pPos, pVeloc, pForce, pRadius, worldS, springC, dampC, pGridIndex, gridCounters, gridCells, gridSL, p
void vectorAdd2(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::ivec3* pGridIndex, int* gridCounters, glm::ivec4* gridCells, int gridSL, int numEl);