#ifndef VIRTUAL_OBJECT_H
#define VIRTUAL_OBJECT_H

// <<<<<<<<<< includes >>>>>>>>>> //
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

	//CVK::Node* graphicBody;		/**< CVK Geometry (Sphere or Cube) as graphics component of this object */

	glm::vec3 position;			/**< position */	//eig  überflüssig wenn model matrix habe

	glm::mat4 modelMatrix;		/**< model matrix */

	int id;						/**< identification number */


public:
	/** \brief default constructor
	 *
	 * creates a virtual object instance
	 * @param posIN initial position
	 * @param bodyCount counts number of initialized bodies
	 * @param massIN body mass
	 * @param staticIN static object (true) or not (false) (not observed at the moment)
	 * @param sizeIN box half size or sphere radius
	 * @param idIN ...
	 */
	VirtualObject(glm::vec3 posIN, int bodyCount, float massIN, bool staticIN, float sizeIN, int idIN);

	/** \brief destructor
	 *
	 * destroys a virtual object instance
	 */
	~VirtualObject();

	/** \brief update modelmatrix
	 *
	 * update modelmatrix when simulation is running on cpu
	 * @return void
	 */
	void updateCPU();

	/** \brief update modelmatrix
	*
	* update modelmatrix when simulation is running on gpu
	* @return void
	*/
	void updateGPU();

	/** \brief update particle values
	*
	* ...
	* @return void
	*/
	void updatePartValuesVO();	//bodies[i]->updateParticleValues();
	
	/** \brief update momenta
	*
	* ...
	* @param duration time length of a simulation step
	* @return void
	*/
	void updateMomentaVO(float duration);	//bodies[i]->updateMomenta(delta);
	
	/** \brief iterate
	*
	* ...
	* @param duration time length of a simulation step
	* @return void
	*/
	void iterateVO(float duration);	//bodies[i]->iterateVO(duration);

};
#endif
