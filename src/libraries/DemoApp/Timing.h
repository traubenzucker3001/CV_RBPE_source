#ifndef TIMING_H
#define TIMING_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

/** \brief timing
 *
 * Timing is Singleton to ensure that there is only one instance at a time
 */
class Timing : public Singleton<Timing> {
	friend class Singleton<Timing>;

private:
	unsigned int frameNum;				/**< counts frame numbers */

	unsigned int lastFrameTimeStamp;	/**< time stamp of last frame */

	unsigned int lastFrameDuration;		/**< duration of last frame */

	unsigned long lastFrameClockStamp;	/**< clock stamp of last frame */

	unsigned long lastFrameClockTicks;	/**< clock ticks of last frame */

	double averageFrameDuration;		/**< average frame duration */

	float framesPerSec;					/**< frames per second */


public:
	/** \brief constructor
	 *
	 * ...
	 */
	Timing();

	/** \brief destructor
	 *
	 * ...
	 */
	~Timing();

	/** \brief initial time
	 *
	 * ...
	 */
	void initTime();

	/** \brief update time
	 *
	 * ...
	 */
	void updateTime();

	/** \brief get time
	 *
	 * ...
	 */
	unsigned int getTime();

	/** \brief get clock
	 *
	 * ...
	 */
	unsigned long getClock();

};
#endif
