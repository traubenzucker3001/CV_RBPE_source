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
	double *velocityIterations;		/**< defines number of velocity iteration steps, defined in Core */

	double *velocityEpsilon;		/**< ..., defined in Core */

	double *positionIterations;		/**< defines number of position iteration steps, defined in Core */

	double *positionEpsilon;		/**< ..., defined in Core */

	double calcVelocIter;			/**< counts number of used velocity iterations */

	double calcPosIter;				/**< counts number of used position iterations */

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
	void prepareContacts(Contact *contArray, unsigned int numContacts, double duration);

	/** \brief resolver
	 *
	 * resolves the contacts
	 */
	void resolveContacts(Contact *contArray, unsigned int numContacts, double duration);

	/** \brief
	 *
	 * ...
	 */
	void adjustVeloc(Contact *contArray, unsigned int numContacts, double duration);

	/** \brief
	 *
	 * ...
	 */
	void adjustPos(Contact *contArray, unsigned int numContacts, double duration);

};
#endif
