
#ifndef CUDA_H_
#define CUDA_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
//#define GLM_FORCE_CUDA
#include <glm/glm.hpp>

#include "DesignPatterns/Singleton.h"

//cuda data structure

class Cuda : public Singleton<Cuda> {
	friend class Singleton<Cuda>;

private:

	int bodyNum;
	int partNum;

	//benötigten arrays cpu (host)
	//rigidbody parameter
	float *h_rbMass;
	glm::vec3 *h_rbForce;
	glm::vec3 *h_rbPos;
	glm::vec3 *h_rbVeloc;
	glm::vec3 *h_rbLinMom;
	glm::quat *h_rgRotQuat;
	glm::mat3 *h_rbRotMat;
	glm::vec3 *h_rbAngVeloc;
	glm::vec3 *h_rbAngMom;
	glm::vec3 *h_rbInitInversInertTensDiago;	//Initial Inverse Inertia Tensor Diagonal
	glm::mat3 *h_rbInverseInertTens;

	//particle parameter
	float *h_pMass;
	glm::vec3 *h_pPos;
	glm::vec3 *h_pVeloc;
	glm::vec3 *h_pForce;

	glm::vec3 *h_pGridIndex;

	int* h_gCountGrid;
	int4* h_gIndexGrid; //int4?!


	//benötigte arrays gpu (device)
	//rigidbody parameter
	float *d_rbMass;
	glm::vec3 *d_rbForce;
	glm::vec3 *d_rbPos;
	glm::vec3 *d_rbVeloc;
	glm::vec3 *d_rbLinMom;
	glm::quat *d_rgRotQuat;
	glm::mat3 *d_rbRotMat;
	glm::vec3 *d_rbAngVeloc;
	glm::vec3 *d_rbAngMom;
	glm::vec3 *d_rbInitInversInertTensDiago;
	glm::mat3 *d_rbInverseInertTens;

	//particle parameter
	float *d_pMass;
	glm::vec3 *d_pPos;
	glm::vec3 *d_pVeloc;
	glm::vec3 *d_pForce;

	glm::vec3 *d_pGridIndex;

	int* d_gCountGrid;
	int4* d_gIndexGrid;	//int4?!

	glm::vec3 gridMinPosVector;		//?!

public:

	Cuda(int bnIN, int pnIN);
	~Cuda();

	void initCUDA();
	void initCUDAGrid();
	//void initHostArrays();		//zu initcuda gepackt
	//void initDeviceArrays();		//zu initcuda gepackt
	void updateHostArrays();
	void hostToDevice();
	void stepCUDA();

};
#endif /* CUDA_H_ */
