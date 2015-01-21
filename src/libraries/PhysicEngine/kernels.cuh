
//#define GLM_FORCE_CUDA
#include <glm\glm.hpp>


/** \brief fill device constant variables
*
* copy constant values from cpu to gpu memory
* @param voxelS voxel size
* @param gridSL grid size
* @param worldS world size
* @param springC spring coefficient
* @param dampC damping coefficient
* @param pRadius particle radius
* @param duration duration of a simulation step
* @param termVeloc terminal velocity
* @param gridMinPosVector uniform grid minimum position vector 
*/
void fillDeviceSymbols(float voxelS, int gridSL, float worldS, float springC, float dampC, float pRadius, float duration, float termVeloc, glm::vec3 gridMinPosVector);

/** \brief prepare reset grid kernel
*
* calculate threads and blocks and launch reset grid kernel
* @param gridCounters grid counters array
* @param gridCells grid cells array
* @param g amount of grid cells
*/
void resetGrid(int* gridCounters, glm::ivec4* gridCells, int g);

/** \brief prepare update grid kernel
*
* calculate threads and blocks and launch update grid kernel
* @param gridCounters grid counters array
* @param gridCells grid cells array
* @param pPos particle positions array
* @param pGridIndex particle grid indices array
*/
void updateGrid(int* gridCounters, glm::ivec4* gridCells, glm::vec3* pPos, glm::ivec3* pGridIndex);	//, float voxelSL, int gridSL  glm::vec3 gridMinPosVec,

/** \brief prepare update momentum kernel
*
* calculate threads and blocks and launch update momentum kernel
* @param rbMass rigid body masses array
* @param rbForce rigid body forces array
* @param rbPos rigid body positions array
* @param rbLinMom rigid body linear momentum array
* @param rbAngMom rigid body angular momentum array
* @param pPos particle positions array
* @param pForce particle forces array
*/
void updateMom(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce);	//, float duration, float termVeloc

/** \brief prepare iterate kernel
*
* calculate threads and blocks and launch iterate kernel
* @param rbMass rigid body masses array
* @param rbPos rigid body positions array
* @param rbVeloc rigid body velocity array
* @param rbLinMom rigid body linear momentum array
* @param rbRotQuat rigid body rotation quaternions
* @param rbRotMat rigid body rotation matrices
* @param rbAngVeloc rigid body angular velocity array
* @param rbAngMom rigid body angular momentum array
* @param initIITDiago initial inverse inertia tensor diagonal
* @param inverInertTens inverse inertia tensor
*/
void iterate(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens);	//float duration, float pRadius

/** \brief prepare calculate collision forces kernel
*
* calculate threads and blocks and launch calculate collision forces kernel
* @param pMass particle masses array
* @param pPos particle positions array
* @param pVeloc particle velocity array
* @param pForce particle forces array
* @param pGridIndex particle grid indices array
* @param gridCounters grid counters array
* @param gridCells grid cells array
*/
void calcCollForces(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, glm::ivec3* pGridIndex, int* gridCounters, glm::ivec4* gridCells);	//, int gridSL , float pRadius, float worldS, float springC, float dampC

/** \brief prepare update particle kernel
*
* calculate threads and blocks and launch update particle kernel
* @param rbPos rigid body positions array
* @param rbVeloc rigid body velocity array
* @param rbRotMat rigid body rotation matrices
* @param rbAngVeloc rigid body angular velocity array
* @param pPos particle positions array
* @param pVeloc particle velocity array
*/
void updatePart(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc);	//, float pRadius
