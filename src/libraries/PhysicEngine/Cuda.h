
#ifndef CUDA_H_
#define CUDA_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
//#define GLM_FORCE_CUDA
#include <glm/glm.hpp>

#include "DesignPatterns\Singleton.h"

/** \brief cuda data structure
*
* class with all necessary host and device arrays
* singleton because it must be sure that every array should only exist once
*/
class Cuda : public Singleton<Cuda> {
	friend class Singleton<Cuda>;

private:

	int bodyNum;	/**< total number of rigid bodies */
	int partNum;	/**< total number of particles */

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

private:
	glm::vec3 *h_pGridIndex;	/**< host array for particle grin indices */

	int* h_gCountGrid;			/**< host array for grid ... */
	glm::vec4 *h_gIndexGrid;	/**< host array for grid ... */		//int4?!


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

	glm::vec3 *d_pGridIndex;	/**< device array for particle grin indices */

	int* d_gCountGrid;			/**< device array for grid ... */
	glm::vec4 *d_gIndexGrid;	/**< device array for grid ... */		//int4?!

	glm::vec3 d_gridMinPosVector;		//?!	//todo: auch auf gpu packen!!
	glm::vec3 h_gridMinPosVector;

	//weitere auf gpu benötigte werte //todo: auf gpu allokieren und von cpu auf gpu übertagen
	/*__constant__ float d_voxelS;
	__device__ int d_gridS;
	__device__ float d_worldS;
	__device__ float d_springC;
	__device__ float d_dampC;
	__device__ float d_pRadius;
	__device__ float d_duration;
	__device__ float d_termVeloc;*/

	float h_voxelS;
	int h_gridS;
	float h_worldS;
	float h_springC;
	float h_dampC;
	float h_pRadius;
	float h_duration;
	float h_termVeloc;


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

	//void initHostArrays();		//zu initcuda gepackt
	//void initDeviceArrays();		//zu initcuda gepackt
	
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

};
#endif /* CUDA_H_ */
