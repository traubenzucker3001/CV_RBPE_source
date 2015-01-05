//neuer cuda file zum lösen der include/compiler fehler

//#define GLM_FORCE_CUDA
#include <glm\glm.hpp>


//extern "C"{
void fillDeviceSymbols(float voxelS, int gridSL, float worldS, float springC, float dampC, float pRadius, float duration, float termVeloc, glm::vec3 gridMinPosVector);

	void resetGrid(int* gridCounters, glm::ivec4* gridCells, int g);
	void updateGrid(int* gridCounters, glm::ivec4* gridCells, glm::vec3* pPos, glm::ivec3* pGridIndex);	//, float voxelSL, int gridSL  glm::vec3 gridMinPosVec,
	void updateMom(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce);	//, float duration, float termVeloc
	void iterate(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens);	//float duration, float pRadius
	void calcCollForces(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, glm::ivec3* pGridIndex, int* gridCounters, glm::ivec4* gridCells);	//, int gridSL , float pRadius, float worldS, float springC, float dampC
	void updatePart(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc);	//, float pRadius
//}