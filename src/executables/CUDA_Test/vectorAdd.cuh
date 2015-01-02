#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//void vectorAdd(glm::quat* A, glm::quat* B, glm::quat* C, int numEl);
void vectorAdd(int* A, glm::ivec4* B, int numEl);


pMass, pPos, pVeloc, pForce, pRadius, worldS, springC, dampC, pGridIndex, gridCounters, gridCells, gridSL, p