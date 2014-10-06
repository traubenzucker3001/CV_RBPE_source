#ifndef CONTACT_RESOLVER_H
#define CONTACT_RESOLVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

/** \brief
 *
 * ...
 */
class ContactResolver
{
private:
	float *velocityIterations;		/**< defines number of velocity iteration steps, defined in Core */

	float *velocityEpsilon;		/**< ..., defined in Core */

	float *positionIterations;		/**< defines number of position iteration steps, defined in Core */

	float *positionEpsilon;		/**< ..., defined in Core */

	float countVelocIter;			/**< counts number of used velocity iterations */

	float countPosIter;			/**< counts number of used position iterations */

public:
	/** \brief constructor
	 *
	 * ...
	 */
	ContactResolver();

	/** \brief destructor
	 *
	 * ...
	 */
	~ContactResolver();

	/** \brief resolving preparation
	 *
	 * ...
	 */
	void prepareContacts(Contact *contArray[], unsigned int numContacts, float duration);

	/** \brief resolver
	 *
	 * resolves the contacts
	 */
	void resolveContacts(Contact *contArray[], unsigned int numContacts, float duration);

	/** \brief adjust velocity
	 *
	 * ...
	 */
	void adjustVeloc(Contact *contArray[], unsigned int numContacts, float duration);

	/** \brief adjust position
	 *
	 * ...
	 */
	void adjustPos(Contact *contArray[], unsigned int numContacts, float duration);

};
#endif
