
#ifndef CUDA_H_
#define CUDA_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "DesignPatterns/Singleton.h"

//cuda data structure

class Cuda : public Singleton<Cuda> {
	friend class Singleton<Cuda>;

private:


public:

	Cuda();
	~Cuda();

	void initCUDA();
	void initCUDAGrid();
	void initHostArrays();
	void initDeviceArrays();
	void updateHostArrays();
	void hostToDevice();
	void stepCUDA();

};
#endif /* CUDA_H_ */
