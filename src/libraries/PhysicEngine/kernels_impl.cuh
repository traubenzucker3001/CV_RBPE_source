//neuer cuda file zum lösen der include/compiler fehler
/*
#define GLM_FORCE_CUDA
#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
*/

#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define GLM_FORCE_CUDA
#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

__device__ __constant__ float d_voxelS;
//__device__ __constant__ int d_gridS;
__device__ __constant__ int d_gridSL;
__device__ __constant__ float d_worldS;
__device__ __constant__ float d_springC;
__device__ __constant__ float d_dampC;
__device__ __constant__ float d_pRadius;
__device__ __constant__ float d_duration;
__device__ __constant__ float d_termVeloc;

//__device__ __constant__ glm::vec3 d_gridMinPosVector;
__device__ __constant__ float d_gridMinPosVecX;
__device__ __constant__ float d_gridMinPosVecY;
__device__ __constant__ float d_gridMinPosVecZ;

//<<<<<<<<<< uniformgrid kernels >>>>>>>>>>
__global__ void resetGridC(int* gridCounters, glm::ivec4* gridCells, int gs){

	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i >= gs){
		return;
	}
	if (i < gs){
		gridCounters[i] = 0;
		gridCells[i].x = -1;
		gridCells[i].y = -1;
		gridCells[i].z = -1;
		gridCells[i].w = -1;
	}
}

__global__ void updateGridC(int* gridCounters, glm::ivec4* gridCells, glm::vec3* pPos, glm::ivec3* pGridIndex, int nop){	//, float voxelSL, int gridSL , glm::vec3 gridMinPosVec

	//unsigned int particleIndex = get_global_id(0);
	int pi = blockDim.x * blockIdx.x + threadIdx.x;

	if (pi >= nop){
		return;
	}
	if (pi < nop){
		pGridIndex[pi].x = (pPos[pi].x - d_gridMinPosVecX) / d_voxelS;
		pGridIndex[pi].y = (pPos[pi].y - d_gridMinPosVecY) / d_voxelS;
		pGridIndex[pi].z = (pPos[pi].z - d_gridMinPosVecZ) / d_voxelS;

		bool validIndex = (pGridIndex[pi].x > 0) &&
			(pGridIndex[pi].x < d_gridSL - 1) &&
			(pGridIndex[pi].y > 0) &&
			(pGridIndex[pi].y < d_gridSL - 1) &&
			(pGridIndex[pi].z > 0) &&
			(pGridIndex[pi].z < d_gridSL - 1);

		if (validIndex) {
			int xStride = d_gridSL * d_gridSL;
			int yStride = d_gridSL;
			int flatGridIndex = (int)pGridIndex[pi].x*xStride +
				(int)pGridIndex[pi].y * yStride +
				(int)pGridIndex[pi].z;

			//todo: zu cuda func
			//int particlesInCell = atomic_inc(&countGrid[flatGridIndex]);	//?
			//
			int n = 4;
			unsigned int* atom = (unsigned int*)(&gridCounters[flatGridIndex]);		//potenzielle fehlerquelle
			int particlesInCell = atomicInc(atom, n);
			//

			if (particlesInCell == 3) {
				gridCells[flatGridIndex].w = (float)pi;
			}
			else if (particlesInCell == 2) {
				gridCells[flatGridIndex].z = (float)pi;
			}
			else if (particlesInCell == 1) {
				gridCells[flatGridIndex].y = (float)pi;
			}
			else if (particlesInCell == 0) {
				gridCells[flatGridIndex].x = (float)pi;
			}
		}
	}
}


//<<<<<<<<<< rigidbody kernels >>>>>>>>>>
__global__ void updateMomC(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce, int nob){	// float duration, float termVeloc,

	//TODO
	//unsigned int bodyIndex = get_global_id(0);
	int bi = blockDim.x * blockIdx.x + threadIdx.x;

	//unsigned int totalNumberOfParticles = get_global_size(0) * 27;
	//int tnop = (blockDim.x * blockIdx.x + threadIdx.x) * 27;

	int particleIndex = bi * 27;
	if (bi >= nob){
		return;
	}
	if (bi < nob){

		rbForce[bi].x = 0.0f;
		rbForce[bi].y = rbMass[bi] * -9.81f; //force of gravity
		rbForce[bi].z = 0.0f;

		glm::vec3 torque = glm::vec3(0.0f, 0.0f, 0.0f);

		//Calculate body force and torque
		for (int i = 0; i < 27; i++) {
			rbForce[bi] = rbForce[bi] + pForce[particleIndex + i];
			glm::vec3 relativePos = pPos[particleIndex + i] - rbPos[bi];
			//torque += cross(relativePos, pForce[particleIndex + i]);
			torque = torque + glm::cross(relativePos, pForce[particleIndex + i]);
		}

		float termMom = d_termVeloc * rbMass[bi];

		rbLinMom[bi].x = rbLinMom[bi].x + rbForce[bi].x * d_duration;
		rbLinMom[bi].y = rbLinMom[bi].y + rbForce[bi].y * d_duration;
		rbLinMom[bi].z = rbLinMom[bi].z + rbForce[bi].z * d_duration;

		//Limit momentum by terminal momentum
		//todo: zu cuda func
		//rbLinMom[bi] = clamp(rbLinMom[bi], -termMom, termMom);
		rbLinMom[bi] = glm::clamp(rbLinMom[bi], -termMom, termMom);

		rbAngMom[bi].x = rbAngMom[bi].x + torque.x * d_duration;
		rbAngMom[bi].y = rbAngMom[bi].y + torque.y * d_duration;
		rbAngMom[bi].z = rbAngMom[bi].z + torque.z * d_duration;
	}
}

__global__ void iterateC(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens, int nob){	//float duration, float pRadius

	//weitere input param
	/*__global float* bodyVBO,
	int bodyVBOStride
	*/

	//unsigned int bodyIndex = get_global_id(0);
	int bi = blockDim.x * blockIdx.x + threadIdx.x;

	//unsigned int bodyVBOIndex = bodyIndex * 24 * 3;

	//int mi = bi * 9;	//*9 nicht nötig wenn glm::mat3!? also mi eig nit nötig
	
	if (bi >= nob){
		return;
	}
	if (bi < nob){

		//Update inverse inertia tensor
			{
				glm::mat3 tempRotMat1 = rbRotMat[bi];
				//glm::mat3 tempIIT1 = inverInertTens[bi];

				float a = tempRotMat1[0].x;
				float b = tempRotMat1[0].y;
				float c = tempRotMat1[0].z;
				float d = tempRotMat1[1].x;
				float e = tempRotMat1[1].y;
				float f = tempRotMat1[1].z;
				float g = tempRotMat1[2].x;
				float h = tempRotMat1[2].y;
				float i = tempRotMat1[2].z;

				float u = initIITDiago[bi].x;
				float v = initIITDiago[bi].y;
				float w = initIITDiago[bi].z;

				//WICHTIG!! --> wird auch richtiger wert beschrieben?!?! wert wird genommen und beschrieben aber eig wert in array wird ja nicht verändert
				/*
				tempIIT1[0].x = u*a*a + b*b*v + c*c*w;
				tempIIT1[0].y = a*d*u + b*e*v + c*f*w;
				tempIIT1[0].z = a*g*u + b*h*v + c*i*w;
				tempIIT1[1].x = a*d*u + b*e*v + c*f*w;
				tempIIT1[1].y = u*d*d + e*e*v + f*f*w;
				tempIIT1[1].z = d*g*u + e*h*v + f*i*w;
				tempIIT1[2].x = a*g*u + b*h*v + c*i*w;
				tempIIT1[2].y = d*g*u + e*h*v + f*i*w;
				tempIIT1[2].z = u*g*g + h*h*v + i*i*w;
				*/
				inverInertTens[bi][0].x = u*a*a + b*b*v + c*c*w;
				inverInertTens[bi][0].y = a*d*u + b*e*v + c*f*w;
				inverInertTens[bi][0].z = a*g*u + b*h*v + c*i*w;
				inverInertTens[bi][1].x = a*d*u + b*e*v + c*f*w;
				inverInertTens[bi][1].y = u*d*d + e*e*v + f*f*w;
				inverInertTens[bi][1].z = d*g*u + e*h*v + f*i*w;
				inverInertTens[bi][2].x = a*g*u + b*h*v + c*i*w;
				inverInertTens[bi][2].y = d*g*u + e*h*v + f*i*w;
				inverInertTens[bi][2].z = u*g*g + h*h*v + i*i*w;
			}

		//Perform linear step
		{
		rbVeloc[bi].x = rbLinMom[bi].x / rbMass[bi];
		rbVeloc[bi].y = rbLinMom[bi].y / rbMass[bi];
		rbVeloc[bi].z = rbLinMom[bi].z / rbMass[bi];

		rbPos[bi].x = rbPos[bi].x + rbVeloc[bi].x * d_duration;
		rbPos[bi].y = rbPos[bi].y + rbVeloc[bi].y * d_duration;
		rbPos[bi].z = rbPos[bi].z + rbVeloc[bi].z * d_duration;
		}

		//Perform angular step
		{
		//Update angular velocity
			{
			glm::mat3 tempIIT2 = inverInertTens[bi];

			float a = tempIIT2[0].x;
			float b = tempIIT2[0].y;
			float c = tempIIT2[0].z;
			float d = tempIIT2[1].x;
			float e = tempIIT2[1].y;
			float f = tempIIT2[1].z;
			float g = tempIIT2[2].x;
			float h = tempIIT2[2].y;
			float i = tempIIT2[2].z;

			float u = rbAngMom[bi].x;
			float v = rbAngMom[bi].y;
			float w = rbAngMom[bi].z;

			rbAngVeloc[bi].x = a*u + b*v + c*w;
			rbAngVeloc[bi].y = d*u + e*v + f*w;
			rbAngVeloc[bi].z = g*u + h*v + i*w;
			}
		float angularVelocitySize = sqrt(
			rbAngVeloc[bi].x*rbAngVeloc[bi].x +
			rbAngVeloc[bi].y*rbAngVeloc[bi].y +
			rbAngVeloc[bi].z*rbAngVeloc[bi].z);

		if (angularVelocitySize > 0.0f) {
			float3 rotationAxis = {
				rbAngVeloc[bi].x / angularVelocitySize,
				rbAngVeloc[bi].y / angularVelocitySize,
				rbAngVeloc[bi].z / angularVelocitySize };

			float rotationAngle = angularVelocitySize*d_duration;

			float ds = cos(rotationAngle / 2.0f);
			float dvx = rotationAxis.x*sin(rotationAngle / 2.0f);
			float dvy = rotationAxis.y*sin(rotationAngle / 2.0f);
			float dvz = rotationAxis.z*sin(rotationAngle / 2.0f);

			float s = rbRotQuat[bi].x;
			float vx = rbRotQuat[bi].y;
			float vy = rbRotQuat[bi].z;
			float vz = rbRotQuat[bi].w;

			rbRotQuat[bi].x = s*ds - vx*dvx - vy*dvy - vz*dvz;
			rbRotQuat[bi].y = ds*vx + s*dvx + dvy*vz - dvz*vy;
			rbRotQuat[bi].z = ds*vy + s*dvy + dvz*vx - dvx*vz;
			rbRotQuat[bi].w = ds*vz + s*dvz + dvx*vy - dvy*vx;
			}
		}

		//Update rotation matrix
		{
		//Normalize quaternion
			{
			float mag2 = rbRotQuat[bi].x*rbRotQuat[bi].x +
						 rbRotQuat[bi].y*rbRotQuat[bi].y +
						 rbRotQuat[bi].z*rbRotQuat[bi].z +
						 rbRotQuat[bi].w*rbRotQuat[bi].w;

			if (mag2 != 0.0f && (fabs(mag2 - 1.0f) > 0.00001f)) {
				float mag = sqrt(mag2);
				rbRotQuat[bi].x /= mag;
				rbRotQuat[bi].y /= mag;
				rbRotQuat[bi].z /= mag;
				rbRotQuat[bi].w /= mag;
			}
		}
		float w = rbRotQuat[bi].x;
		float x = rbRotQuat[bi].y;
		float y = rbRotQuat[bi].z;
		float z = rbRotQuat[bi].w;

		float xx = x * x;
		float yy = y * y;
		float zz = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		//glm::mat3 tempRotMat2 = rbRotMat[bi];
		//WICHTIG!! --> wird auch richtiger wert beschrieben?!?! wert wird genommen und beschrieben aber eig wert in array wird ja nicht verändert
		/*
		tempRotMat2[0].x = 1.0f - 2.0f*(yy + zz);
		tempRotMat2[0].y = 2.0f*(xy - wz);
		tempRotMat2[0].z = 2.0f*(xz + wy);
		tempRotMat2[1].x = 2.0f*(xy + wz);
		tempRotMat2[1].y = 1.0f - 2.0f*(xx + zz);
		tempRotMat2[1].z = 2.0f*(yz - wx);
		tempRotMat2[2].x = 2.0f*(xz - wy);
		tempRotMat2[2].y = 2.0f*(yz + wx);
		tempRotMat2[2].z = 1.0f - 2.0f*(xx + yy);
		*/

		rbRotMat[bi][0].x = 1.0f - 2.0f*(yy + zz);
		rbRotMat[bi][0].y = 2.0f*(xy - wz);
		rbRotMat[bi][0].z = 2.0f*(xz + wy);
		rbRotMat[bi][1].x = 2.0f*(xy + wz);
		rbRotMat[bi][1].y = 1.0f - 2.0f*(xx + zz);
		rbRotMat[bi][1].z = 2.0f*(yz - wx);
		rbRotMat[bi][2].x = 2.0f*(xz - wy);
		rbRotMat[bi][2].y = 2.0f*(yz + wx);
		rbRotMat[bi][2].z = 1.0f - 2.0f*(xx + yy);
		}
	}
	//Update body VBO
	//siehe anhang
}


//<<<<<<<<<< particle kernels >>>>>>>>>>
__global__ void calcCollForcesC(float* pMass, glm::vec3* pPos, glm::vec3* pVeloc, glm::vec3* pForce, glm::ivec3* pGridIndex, int* gridCounters, glm::ivec4* gridCells, int nop){	//, int gridSL , float pRadius, float worldS, float springC, float dampC

	//TODO
	//unsigned int particleIndex = get_global_id(0);
	int pi = blockDim.x * blockIdx.x + threadIdx.x;

	if (pi >= nop){
		return;
	}
	if (pi < nop){
		pForce[pi].x = 0.0f;
		pForce[pi].y = 0.0f;
		pForce[pi].z = 0.0f;

		glm::ivec3 gridIndex = pGridIndex[pi];

		//Pretend border cell is 1 position inwards to avoid checking outside bounds for neighbors
		//todo: zu cuda func
		//gridIndex = clamp(gridIndex, 1, gridSL - 2);
		gridIndex = glm::clamp(gridIndex, 1, d_gridSL - 2);	//WICHTIG!! --> wird auch richtiger wert beschrieben?!?! wert wird genommen und beschrieben aber eig wert in array wird ja nicht verändert
		int xSteps = d_gridSL*d_gridSL;
		int ySteps = d_gridSL;

		//int flatGridIndex = (int)gridIndex.x * xSteps + (int)gridIndex.y * ySteps + (int)gridIndex.z;
		int flatGridIndex = gridIndex.x * xSteps + gridIndex.y * ySteps + gridIndex.z;

		//oder glm::vec4 besser??, dann auf umstellung bei zählweise achten!!
		glm::vec4 neighborCells[27];	//int4??
		int cellIndexJ = 0;

		flatGridIndex = flatGridIndex - xSteps;
		flatGridIndex = flatGridIndex + 2 * ySteps;
		flatGridIndex = flatGridIndex + 2; //zStride

		for (int x = 0; x < 3; x++) {	//braucht man den *3 schritt überhaupt, da ja vec3 benutzt?!
			flatGridIndex = flatGridIndex - 3 * ySteps;

			for (int y = 0; y < 3; y++) {
				flatGridIndex = flatGridIndex - 3;

				for (int z = 0; z < 3; z++) {

					neighborCells[cellIndexJ] = gridCells[flatGridIndex];
					cellIndexJ++;
					flatGridIndex++;
				}
				flatGridIndex = flatGridIndex + ySteps;
			}
			flatGridIndex = flatGridIndex + xSteps;
		}

		for (int j = 0; j < 27; j++) {

			glm::vec4 neighborParticles = glm::vec4(neighborCells[j].x,
													neighborCells[j].y,
													neighborCells[j].z,
													neighborCells[j].w);

			for (int k = 0; k < 4; k++) {
				int otherParticle = (int)neighborParticles[k];
				if ((otherParticle != pi) && (otherParticle != (-1))) {
					glm::vec3 distance = pPos[otherParticle] - pPos[pi];

					float absDistance = sqrt(distance.x*distance.x + distance.y*distance.y + distance.z*distance.z);

					if ((absDistance + 0.000001f) < (2.0f * d_pRadius)) {
						pForce[pi].x = pForce[pi].x - d_springC*(d_pRadius + d_pRadius - absDistance)*(distance.x / absDistance);
						pForce[pi].y = pForce[pi].y - d_springC*(d_pRadius + d_pRadius - absDistance)*(distance.y / absDistance);
						pForce[pi].z = pForce[pi].z - d_springC*(d_pRadius + d_pRadius - absDistance)*(distance.z / absDistance);

						//glm::vec3
						float3 relativeVelocity = { pVeloc[otherParticle].x - pVeloc[pi].x,
													pVeloc[otherParticle].y - pVeloc[pi].y,
													pVeloc[otherParticle].z - pVeloc[pi].z };

						pForce[pi].x = pForce[pi].x + d_dampC*relativeVelocity.x;
						pForce[pi].y = pForce[pi].y + d_dampC*relativeVelocity.y;
						pForce[pi].z = pForce[pi].z + d_dampC*relativeVelocity.z;

					}
				}
			}
		}

		//Boundary forces
		{
		bool collisionOccured = false;
		// Ground collision
		if (pPos[pi].y - d_pRadius < 0.0f) {
			collisionOccured = true;
			pForce[pi].y = pForce[pi].y + d_springC*(d_pRadius - pPos[pi].y);
		}

		// X-axis Wall Collision
		if (pPos[pi].x - d_pRadius < -d_worldS) {
			collisionOccured = true;
			pForce[pi].x = pForce[pi].x + d_springC*(-d_worldS - pPos[pi].x + d_pRadius);

		}
		else if (pPos[pi].x + d_pRadius > d_worldS) {
			collisionOccured = true;
			pForce[pi].x = pForce[pi].x + d_springC*(d_worldS - pPos[pi].x - d_pRadius);
		}

		// Z-axis Wall Collision
		if (pPos[pi].z - d_pRadius < -d_worldS) {
			collisionOccured = true;
			pForce[pi].z = pForce[pi].z + d_springC*(-d_worldS - pPos[pi].z + d_pRadius);

		}
		else if (pPos[pi].z + d_pRadius > d_worldS) {
			collisionOccured = true;
			pForce[pi].z = pForce[pi].z + d_springC*(d_worldS - pPos[pi].z - d_pRadius);
		}

		// Damping
		if (collisionOccured) {
			pForce[pi].x = pForce[pi].x - d_dampC*pVeloc[pi].x;
			pForce[pi].y = pForce[pi].y - d_dampC*pVeloc[pi].y;
			pForce[pi].z = pForce[pi].z - d_dampC*pVeloc[pi].z;
		}
		}
	}
}

__global__ void updatePartC(glm::vec3* rbPos, glm::vec3* rbVeloc, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* pPos, glm::vec3* pVeloc, int nop){	// float pRadius,

	//weitere input param
	/*__global float* particleVBO,*/

	//unsigned int particleIndex = get_global_id(0);
	int pi = blockDim.x * blockIdx.x + threadIdx.x;
	int bi = pi / 27;
	//int mi = bi * 9;	//*9 nicht nötig wenn glm::mat3!? also mi eig nit nötig

	if (pi >= nop){
		return;
	}
	if (pi < nop){
		glm::vec3 originalRelativePos;
		//Calculate original relative position
		{
		int relativeIndex = pi % 27;

		int xIndex = relativeIndex / 9;
		relativeIndex = relativeIndex - xIndex * 9;

		int yIndex = relativeIndex / 3;
		relativeIndex = relativeIndex - yIndex * 3;

		int zIndex = relativeIndex;

		float space = 2.0f*d_pRadius;

		xIndex--;
		yIndex--;
		zIndex--;

		originalRelativePos.x = (float)xIndex * space;
		originalRelativePos.y = yIndex*space;
		originalRelativePos.z = zIndex*space;
		}

		//Update particle position
		{
		glm::mat3 tempRotMat = rbRotMat[bi];
		pPos[pi].x = originalRelativePos.x * tempRotMat[0].x +				//oder doch mi+1 +2 +3 +4 +5 +... ?!	//.xyz wollt er nit
					 originalRelativePos.y * tempRotMat[0].y +
					 originalRelativePos.z * tempRotMat[0].z;

		pPos[pi].y = originalRelativePos.x * tempRotMat[1].x +
					 originalRelativePos.y * tempRotMat[1].y +
					 originalRelativePos.z * tempRotMat[1].z;

		pPos[pi].z = originalRelativePos.x * tempRotMat[2].x +
					 originalRelativePos.y * tempRotMat[2].y +
					 originalRelativePos.z * tempRotMat[2].z;

		pPos[pi].x = pPos[pi].x + rbPos[bi].x;
		pPos[pi].y = pPos[pi].y + rbPos[bi].y;
		pPos[pi].z = pPos[pi].z + rbPos[bi].z;
		}

		//Update particle velocity
		float scalar = sqrt(rbAngVeloc[bi].x*rbAngVeloc[bi].x +
							rbAngVeloc[bi].y*rbAngVeloc[bi].y +
							rbAngVeloc[bi].z*rbAngVeloc[bi].z);

		scalar *= scalar;

		pVeloc[pi].x = rbVeloc[bi].x;
		pVeloc[pi].y = rbVeloc[bi].y;
		pVeloc[pi].z = rbVeloc[bi].z;

		if (scalar > 0.0f) {
			glm::vec3 relativePosition = glm::vec3(	pPos[pi].x - rbPos[bi].x,
													pPos[pi].y - rbPos[bi].y,
													pPos[pi].z - rbPos[bi].z );

			float scalar2 = (rbAngVeloc[bi].x*relativePosition.x +
							 rbAngVeloc[bi].y*relativePosition.y +
							 rbAngVeloc[bi].z*relativePosition.z) / scalar;

			glm::vec3 term = glm::vec3(	relativePosition.x - rbAngVeloc[bi].x*scalar2,
										relativePosition.y - rbAngVeloc[bi].y*scalar2,
										relativePosition.z - rbAngVeloc[bi].z*scalar2 );
			
			pVeloc[pi].x += (rbAngVeloc[bi].y*term.z - rbAngVeloc[bi].z*term.y);
			pVeloc[pi].y += (rbAngVeloc[bi].z*term.x - rbAngVeloc[bi].x*term.z);
			pVeloc[pi].z += (rbAngVeloc[bi].x*term.y - rbAngVeloc[bi].y*term.x);
		}
	}
	//Update particle VBO
	//siehe anhang
}