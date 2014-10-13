#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PhysicEngine/CollisionShape.h"
#include "PhysicEngine/Sphere.h"
#include "PhysicEngine/Box.h"

using namespace std;

int main() {

	cout << "Hallo Test!" << endl;

	//Core core = new Core();
	glm::vec3 vec = glm::vec3(1,2,3);
	CollisionShape *test = new Sphere(vec,2.0f);
	test->calcParticles();
	//Sphere *test2 = new Sphere(vec,2);
	//test->getOrigin();
	//test->
	float testArr[9] = {0,1,2,3,4,5,6,7,8};
	glm::mat3 testMat = glm::make_mat3(testArr);
	glm::mat3 testMat2 = glm::mat3(1,2,3,4,5,6,7,8,9);
	//cout << testMat[0] << endl;
	cout << testMat[1][0] << endl;
	cout << testMat2[0].y << endl;

	return 0;
}
