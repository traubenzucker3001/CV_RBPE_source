#ifndef VIRTUAL_OBJECT_H
#define VIRTUAL_OBJECT_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <CVK_Framework/CVK_Framework.h>

#include "RigidBody.h"

/** \brief virtual object
 *
 * connection between physical and graphical representation
 */
class VirtualObject
{
private:
	RigidBody* physicBody;		/**< RigidBody as physics component of this object */

	CVK::Geometry graphicBody;	/**< CVK Geometry (Sphere or Cube) as graphics component of this object */

	//eig  überflüssig wenn model matrix habe
	glm::vec3 position;			/**< position */

	glm::mat4 modelMatrix;		/**< model matrix */


public:
	/** \brief default constructor
	 *
	 * ...
	 */
	VirtualObject(RigidBody* physBody);

	/** \brief constructor
	 *
	 * for sphere as graphics component
	 * ...
	 */
	//VirtualObject(bool shape, float shapeSize);

	/** \brief destructor
	 *
	 * ...
	 */
	~VirtualObject();

	/** \brief
	 *
	 * ...
	 */
	void update();

	/** \brief
	 *
	 * ...
	 */
	void translate();

	/** \brief
	 *
	 * ...
	 */
	void scale();

};
#endif
