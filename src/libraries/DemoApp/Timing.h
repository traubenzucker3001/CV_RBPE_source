
#ifndef TIMING_H_
#define TIMING_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "DesignPatterns\Singleton.h"

/** \brief Timing
 *
 * class to control simulation time and frames per second
 */
class Timing : public Singleton<Timing> {
	friend class Singleton<Timing>;

private:

	int frameNum;		/**< frame number */
	float fps;			/**< frames per second */

	//float afd;			/**< average frame duration */

	int totalTime;		/**< total running time */

	int startTime;		/**< frame start time */
	int endTime;		/**< frame end time */

	int currentTime;	/**< current frame duration */
	int previousTime;	/**< frame duration of previous simulation step */

public:

	/** \brief constructor
	*
	* creates a timing instance
	*/
	Timing();

	/** \brief destructor
	*
	* deletes a timing instance
	*/
	~Timing();

	/** \brief start frame
	*
	* mark the begin of a frames
	* @return void
	*/
	void startFrame();

	/** \brief end frame
	*
	* mark the end of a frame
	* @return void
	*/
	void endFrame();

	/** \brief get current time
	*
	* get the current time from glfw
	* @return current time
	*/
	int getTime();

	//void updateAFD();

	/** \brief update frames per second
	*
	* recalculates the number of frames per second
	* @return current frames per second
	*/
	float updateFPS();

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
};

#endif /* TIMING_H_ */
