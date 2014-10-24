/*
 * Timing2.cpp
 *
 *  Created on: 24.10.2014
 *      Author: Daniel
 */

#ifndef TIMING2_CPP_
#define TIMING2_CPP_

class Timing : public Singleton<Timing> {
	friend class Singleton<Timing>;

private:

	int frameCount;//
	float fps;

	//TODO vars

public:

	Timing();
	~Timing();

	//TODO func
};

#endif /* TIMING2_CPP_ */
