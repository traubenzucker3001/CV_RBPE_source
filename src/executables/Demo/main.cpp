#include <iostream>
//#include <CVK.../CVKFramework.h>

using namespace std;

int main() {

	//cout << "Hallo Test!" << endl;

	glfwInit();
	CVK::useOpenGL33CoreProfile();
	int w,h;
	w = ;
	h = ;
	GLFWwindow* window = glfwCreateWindow(w, h, "RBPE Demo", 0, 0);
	glfwSetWindowPos( window, 100, 50);
	glfwMakeContextCurrent(window);

	//...

	glewInit();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

//oder demo in einzelnes neues projekt??!
