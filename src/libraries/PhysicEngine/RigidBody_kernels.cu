
// <<<<<<<<<< includes >>>>>>>>>> //
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <glm\glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "PhysicEngine\World.h"

//update momenta
void updateMom(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce, float duration, float termVeloc){

	//todo: blocks und threads berechn.
	int b = World::getInstance()->getAllBodyNum();
	int blockSize = 64;
	int numThreads = ;
	int numBlocks = ;

	updateMomC <<< numBlocks, numThreads >>>(rbMass,rbForce,rbPos,rbLinMom,rbAngMom,pPos,pForce,duration,termVeloc);
}

__global__ void updateMomC(float* rbMass, glm::vec3* rbForce, glm::vec3* rbPos, glm::vec3* rbLinMom, glm::vec3* rbAngMom, glm::vec3* pPos, glm::vec3* pForce, float duration, float termVeloc){

	//TODO
	//unsigned int bodyIndex = get_global_id(0);
	int bi = blockDim.x * blockIdx.x + threadIdx.x;

	//unsigned int totalNumberOfParticles = get_global_size(0) * 27;
	int tnop = (blockDim.x * blockIdx.x + threadIdx.x) * 27;

	int particleIndex = bi * 27;

	rbForce[bi].x = 0.0f;
	rbForce[bi].y = rbMass[bi] * -9.81f; //force of gravity
	rbForce[bi].z = 0.0f;

	glm::vec3 torque = glm::vec3( 0.0f, 0.0f, 0.0f );

	//Calculate body force and torque
	for (int i = 0; i<27; i++) {
		rbForce[bi] += pForce[particleIndex + i];
		glm::vec3 relativePos = pPos[particleIndex + i] - rbPos[bi];
		//torque += cross(relativePos, pForce[particleIndex + i]);
		torque += glm::cross(relativePos, pForce[particleIndex + i]);
	}

	float termMom = termVeloc * rbMass[bi];

	rbLinMom[bi].x += rbForce[bi].x * duration;
	rbLinMom[bi].y += rbForce[bi].y * duration;
	rbLinMom[bi].z += rbForce[bi].z * duration;

	//Limit momentum by terminal momentum
	//todo: zu cuda func
	//rbLinMom[bi] = clamp(rbLinMom[bi], -termMom, termMom);
	rbLinMom[bi] = glm::clamp(rbLinMom[bi], -termMom, termMom);

	rbAngMom[bi].x += torque.x * duration;
	rbAngMom[bi].y += torque.y * duration;
	rbAngMom[bi].z += torque.z * duration;
}


//perform step
void iterate(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens, float duration, float pRadius){

	//todo: blocks und threads berechn.
	int b = World::getInstance()->getAllBodyNum();
	int blockSize = 64;
	int numThreads = ;
	int numBlocks = ;

	iterateC <<< numBlocks, numThreads >>>(rbMass,rbPos,rbVeloc,rbLinMom,rbRotQuat,rbRotMat,rbAngVeloc,rbAngMom,initIITDiago,inverInertTens,duration,pRadius);
}

__global__ void iterateC(float* rbMass, glm::vec3* rbPos, glm::vec3* rbVeloc, glm::vec3* rbLinMom, glm::quat* rbRotQuat, glm::mat3* rbRotMat, glm::vec3* rbAngVeloc, glm::vec3* rbAngMom, glm::vec3* initIITDiago, glm::mat3* inverInertTens, float duration, float pRadius){

	//weitere input param
	/*__global float* bodyVBO,
	  int bodyVBOStride
	*/

	//TODO

	//unsigned int bodyIndex = get_global_id(0);
	int bi = blockDim.x * blockIdx.x + threadIdx.x;
	
	//unsigned int bodyVBOIndex = bodyIndex * 24 * 3;

	int mi = bi * 9;	//*9 nicht nötig wenn glm::mat3!? also mi eig nit nötig

	//Update inverse inertia tensor
	{
		glm::mat3 tempRotMat1 = rbRotMat[bi];
		glm::mat3 tempIIT1 = inverInertTens[bi];

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

		tempIIT1[0].x = u*a*a + b*b*v + c*c*w;
		tempIIT1[0].y = a*d*u + b*e*v + c*f*w;
		tempIIT1[0].z = a*g*u + b*h*v + c*i*w;
		tempIIT1[1].x = a*d*u + b*e*v + c*f*w;
		tempIIT1[1].y = u*d*d + e*e*v + f*f*w;
		tempIIT1[1].z = d*g*u + e*h*v + f*i*w;
		tempIIT1[2].x = a*g*u + b*h*v + c*i*w;
		tempIIT1[2].y = d*g*u + e*h*v + f*i*w;
		tempIIT1[2].z = u*g*g + h*h*v + i*i*w;
	}

	//Perform linear step
	{
		rbVeloc[bi].x = rbLinMom[bi].x / rbMass[bi];
		rbVeloc[bi].y = rbLinMom[bi].y / rbMass[bi];
		rbVeloc[bi].z = rbLinMom[bi].z / rbMass[bi];

		rbPos[bi].x += rbVeloc[bi].x * duration;
		rbPos[bi].y += rbVeloc[bi].y * duration;
		rbPos[bi].z += rbVeloc[bi].z * duration;
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

			float rotationAngle = angularVelocitySize*duration;

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

		glm::mat3 tempRotMat2 = rbRotMat[bi];
		//WICHTIG!! --> wird auch richtiger wert beschrieben?!?! wert wird genommen und beschrieben aber eig wert in array wird ja nicht verändert
		tempRotMat2[0].x = 1.0f - 2.0f*(yy + zz);
		tempRotMat2[0].y = 2.0f*(xy - wz);
		tempRotMat2[0].z = 2.0f*(xz + wy);
		tempRotMat2[1].x = 2.0f*(xy + wz);
		tempRotMat2[1].y = 1.0f - 2.0f*(xx + zz);
		tempRotMat2[1].z = 2.0f*(yz - wx);
		tempRotMat2[2].x = 2.0f*(xz - wy);
		tempRotMat2[2].y = 2.0f*(yz + wx);
		tempRotMat2[2].z = 1.0f - 2.0f*(xx + yy);

	}

	//Update body VBO
	//siehe anhang
}


//----- anhang -----
//opencl
	//Update body VBO
/*
	{

		float side = particleRadius * 3.0f;

		float normals[6][3] = { // Cube face normals 
				{ -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
				{ 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } };
		int faceIndices[6][4] = { // Cube faces' vertex indices 
				{ 0, 1, 2, 3 }, { 3, 2, 6, 7 }, { 7, 6, 5, 4 },
				{ 4, 5, 1, 0 }, { 5, 6, 2, 1 }, { 7, 4, 0, 3 } };
		float vertices[8][3] = { // Cube vertex positions 
				{ -side, -side, side }, { -side, -side, -side }, { -side, side, -side },
				{ -side, side, side }, { side, -side, side }, { side, -side, -side },
				{ side, side, -side }, { side, side, side } };

		for (int i = 0; i<6; i++) { //for every face
			for (int j = 0; j<4; j++) { //for every vertex in the face

				float r0 = rotationMatrix[matrixIndex];
				float r1 = rotationMatrix[matrixIndex + 1];
				float r2 = rotationMatrix[matrixIndex + 2];
				float r3 = rotationMatrix[matrixIndex + 3];
				float r4 = rotationMatrix[matrixIndex + 4];
				float r5 = rotationMatrix[matrixIndex + 5];
				float r6 = rotationMatrix[matrixIndex + 6];
				float r7 = rotationMatrix[matrixIndex + 7];
				float r8 = rotationMatrix[matrixIndex + 8];

				float v0 = vertices[faceIndices[i][j]][0];
				float v1 = vertices[faceIndices[i][j]][1];
				float v2 = vertices[faceIndices[i][j]][2];

				bodyVBO[bodyVBOIndex] = r0*v0 + r1*v1 + r2*v2 + position[bodyIndex].x;
				bodyVBO[bodyVBOIndex + 1] = r3*v0 + r4*v1 + r5*v2 + position[bodyIndex].y;
				bodyVBO[bodyVBOIndex + 2] = r6*v0 + r7*v1 + r8*v2 + position[bodyIndex].z;

				float n0 = normals[i][0];
				float n1 = normals[i][1];
				float n2 = normals[i][2];

				bodyVBO[bodyVBOIndex + bodyVBOStride] = r0*n0 + r1*n1 + r2*n2;
				bodyVBO[bodyVBOIndex + bodyVBOStride + 1] = r3*n0 + r4*n1 + r5*n2;
				bodyVBO[bodyVBOIndex + bodyVBOStride + 2] = r6*n0 + r7*n1 + r8*n2;

				bodyVBOIndex += 3;
			}
		}
	}
}	
*/
//