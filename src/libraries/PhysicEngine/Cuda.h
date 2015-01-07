
#ifndef CUDA_H_
#define CUDA_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
//#define GLM_FORCE_CUDA
#include <glm/glm.hpp>

#include "DesignPatterns\Singleton.h"

//link fix try 4


/** \brief cuda data structure
*
* class with all necessary host and device arrays
*/
class Cuda{
private:

	int bodyNum;	/**< total number of rigid bodies */
	int partNum;	/**< total number of particles */
	int gridSize;	/**< total number of grid cells */

public:
	//benötigten arrays cpu (host)
	//rigidbody parameter
	float *h_rbMass;							/**< host array for rigid body masses */
	glm::vec3 *h_rbForce;						/**< host array for rigid body forces */
	glm::vec3 *h_rbPos;							/**< host array for rigid body positions */
	glm::vec3 *h_rbVeloc;						/**< host array for rigid body velocities */
	glm::vec3 *h_rbLinMom;						/**< host array for rigid body linear momentums */
	glm::quat *h_rbRotQuat;						/**< host array for rigid body rotation quaternions */
	glm::mat3 *h_rbRotMat;						/**< host array for rigid body rotation matrixes */
	glm::vec3 *h_rbAngVeloc;					/**< host array for rigid body angular velocities */
	glm::vec3 *h_rbAngMom;						/**< host array for rigid body angular momentums */
	glm::vec3 *h_rbInitInversInertTensDiago;	/**< host array for rigid body initial inverse inertia tensor diagonals */	//Initial Inverse Inertia Tensor Diagonal
	glm::mat3 *h_rbInverseInertTens;			/**< host array for rigid body inverse inertia tensors */

	//particle parameter
	float *h_pMass;				/**< host array for particle masses */
	glm::vec3 *h_pPos;			/**< host array for particle positions */
	glm::vec3 *h_pVeloc;		/**< host array for particle velocities */
	glm::vec3 *h_pForce;		/**< host array for particle forces */

	//update VOs arrays
	glm::vec3* h_uVOpos;		/**< host array for virtual objekt positions */
	glm::quat* h_uVOrot;		/**< host array for virtual objekt rotations */
	//glm::vec3* h_uVOpPos;		/**< host array for virtual objekt particle positions */

private:
	glm::ivec3 *h_pGridIndex;	/**< host array for particle grin indices */

	int* h_gridCounters;		/**< host array for grid counters */
	glm::ivec4* h_gridCells;	/**< host array for grid cells */


	//benötigte arrays gpu (device)
	//rigidbody parameter
	float *d_rbMass;							/**< device array for rigid body masses */
	glm::vec3 *d_rbForce;						/**< device array for rigid body forces */
	glm::vec3 *d_rbPos;							/**< device array for rigid body positions */
	glm::vec3 *d_rbVeloc;						/**< device array for rigid body velocities */
	glm::vec3 *d_rbLinMom;						/**< device array for rigid body linear momentums */
	glm::quat *d_rbRotQuat;						/**< device array for rigid body rotation quaternions */
	glm::mat3 *d_rbRotMat;						/**< device array for rigid body rotation matrixes */
	glm::vec3 *d_rbAngVeloc;					/**< device array for rigid body angular velocities */
	glm::vec3 *d_rbAngMom;						/**< device array for rigid body angular momentums */
	glm::vec3 *d_rbInitInversInertTensDiago;	/**< device array for rigid body initial inverse inertia tensor diagonals */
	glm::mat3 *d_rbInverseInertTens;			/**< device array for rigid body inverse inertia tensors */

	//particle parameter
	float *d_pMass;				/**< device array for particle masses */
	glm::vec3 *d_pPos;			/**< device array for particle positions */
	glm::vec3 *d_pVeloc;		/**< device array for particle velocities */
	glm::vec3 *d_pForce;		/**< device array for particle forces */

	glm::ivec3 *d_pGridIndex;	/**< device array for particle grid indices */

	int* d_gridCounters;		/**< device array for grid counters */
	glm::ivec4 *d_gridCells;	/**< device array for grid cells */		//int4?!

	float h_voxelS;						/**< host variable for voxel size */
	int h_gridSL;						/**< host variable for grid side lenght */
	float h_worldS;						/**< host variable for world size */
	float h_springC;					/**< host variable for spring coefficient */
	float h_dampC;						/**< host variable for damping coefficient */
	float h_pRadius;					/**< host variable for particle radius */
	float h_duration;					/**< host variable for simulation step duration */
	float h_termVeloc;					/**< host variable for body terminal velocity */

	glm::vec3 h_gridMinPosVector;		/**< host vector for minimum grid position*/

public:
	/** \brief constructor
	*
	* creates the cuda structure instance
	* @param bnIN total number of rigid bodies
	* @param pnIN total number of particles
	*/
	Cuda(int bnIN, int pnIN);

	/** \brief default constructor
	*
	* creates the cuda structure instance
	* -not implemented-
	*/
	Cuda();

	/** \brief constructor
	*
	* destroys the cuda structure instance
	*/
	~Cuda();

	/** \brief initial cuda
	*
	* initials the host arrays, the uniform grid and allocates memory for device arrays
	* @return void
	*/
	void initCUDA();
	
	/** \brief init uniform grid
	*
	* initials uniform grid host and device arrays
	* @return void
	*/
	void initCUDAGrid();
	
	/** \brief update host arrays
	*
	* update/fill the host arrays
	* @return void
	*/
	void updateHostArrays();
	
	/** \brief copy from host to device
	*
	* copy host array data to device arrays
	* @return void
	*/
	void hostToDevice();
	
	/** \brief step simulation
	*
	* step the physics simulation with cuda
	* @return void
	*/
	void stepCUDA();

	/** \brief update VO arrays
	*
	* copy current rigid body position and rotation from gpu to cpu arrays
	* @return void
	*/
	void updateVOarrays();
};
#endif /* CUDA_H_ */
