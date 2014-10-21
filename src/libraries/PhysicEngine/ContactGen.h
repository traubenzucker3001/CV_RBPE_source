#ifndef CONTACT_GEN_H
#define CONTACT_GEN_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

/** \brief
 *
 * ...
 */
class ContactGen {
private:
	bool isGPU;		/**< ... */
	int deviceID;	/**< ... */

public:
	/** \brief
	 *
	 * ...
	 */
	void addContact();

	/** \brief start detection on CPU
	*
	* ...
	*/
	void startCPU();

	/** \brief start detection on GPU
	*
	* ...
	*/
	void startGPU();
};
#endif
