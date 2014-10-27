
#ifndef TIMING2_CPP_
#define TIMING2_CPP_

// <<<<<<<<<< includes >>>>>>>>>> //
#include "DesignPatterns\Singleton.h"

/** \brief Timing
 *
 * class to control simulation time and frames per second
 */
class Timing : public Singleton<Timing> {
	friend class Singleton<Timing>;

private:

	//framecount
	int frameNum;					/**< frame number */
	float fps;						/**< frames per second */

	float afd;		/**< average frame duration */

	//gesamtzeit
	int totalTime;					/**< total running time */

	int startTime;					/**< frame start time */
	int endTime;					/**< frame end time */

	//TODO vars
	//eig erledigt
	//aber vllt noch weitere?!

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

	/** \brief start frame
	*
	* ...
	*/
	void startFrame();

	/** \brief end frame
	*
	* ...
	*/
	void endFrame();

	/** \brief get current time
	*
	* ...
	*/
	int getTime();

	/** \brief update average frame duration
	*
	* ...
	*/
	void updateAFD();

	/** \brief update frames per second
	*
	* ...
	*/
	int updateFPS();

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
};

#endif /* TIMING2_CPP_ */
