
#ifndef TIMING2_CPP_
#define TIMING2_CPP_

// <<<<<<<<<< includes >>>>>>>>>> //


class Timing : public Singleton<Timing> {
	friend class Singleton<Timing>;

private:

	int frameNum;//framecount
	float fps;

	float averageFrameDuration;		/**< average frame duration */

	int totalTime;	//gesamtzeit

	int startTime;
	int endTime;

	//TODO vars

public:

	Timing();
	~Timing();

	void startFrame();
	void endFrame();

	int getTime();

	void updateAFD();
	int updateFPS();

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
};

#endif /* TIMING2_CPP_ */
