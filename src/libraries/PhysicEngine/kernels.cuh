//neuer cuda file zum lösen der include/compiler fehler

#include <glm\glm.hpp>


//extern "C"{
	void resetGrid(int* countGrid, glm::vec4* indexGrid);
	void updateGrid(int* countGrid, glm::vec4* indexGrid, glm::vec3* pPos, glm::vec3 gridMinPosVec, float voxelSL, int gridSL, glm::vec3* pGridIndex);
	void updateMom(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce, float duration, float termVeloc);
	void iterate(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens, float duration, float pRadius);
	void calcCollForces(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, float pRadius, float worldS, float springC, float dampC, glm::vec3* pGridIndex, int* countGrid, glm::vec4* indexGrid, int gridSL);
	void updatePart(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc, float pRadius);
//}