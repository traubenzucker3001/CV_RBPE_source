
#ifndef UNIFORMGRID_H_
#define UNIFORMGRID_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <glm/glm.hpp>
#include "DesignPatterns\Singleton.h"

//nicht sicher ob überhaupt singleton sein muss
/** \brief Uniform Grid
 *
 * class for uniform grid representation
 * singleton because there can be only one grid over the one physics simulation world
 */
class UniformGrid : public Singleton<UniformGrid> {
	friend class Singleton<UniformGrid>;

private:
	int* gridCells;			/**< ... */	

	int* gridCounters; 		/**< ... */
	float voxelSize;		/**< voxel size */

	int partPerVoxel = 4;	/**< particles per voxel */		//4 nach gpu gems

	float gridMinPos;		/**< grid minimum position */	//?
	int gridLength;			/**< grid lenght */

	int gridSize;			/**< grid size */
	int xSteps;				/**< ... */
	int ySteps;				/**< ... */

public:

	/** \brief constructor
	 *
	 * creates the uniform grid instance
	 */
	UniformGrid();

	/** \brief destructor
	 *
	 * destroys the uniform grid instance
	 */
	~UniformGrid();

	/** \brief create uniform grid
	 *
	 * initial the uniform grid
	 * @return void
	 */
	void createGrid();

	/** \brief update uniform grid
	 *
	 * update the uniform grid
	 * @return void
	 */
	void updateGrid();

	//
	/** \brief is valid grid index
	 *
	 * proof of given index is a valid one
	 * @param gridIndex ...
	 * @return true if it is a valid index, false if not
	 */
	bool isValidIndex(glm::ivec3 gridIndex);

	/** \brief get neighbor particle indices
	 *
	 * get the neighbors of the particle with given grid index
	 * @param gridIndex ...
	 * @return array of neighbor particles
	 */
	int* getNeighborPartIndices(glm::ivec3 gridIndex);

	// <<<<<<<<<< getter + setter >>>>>>>>>> //
	int getPartPerVoxel() const {
		return partPerVoxel;
	}

	void setPartPerVoxel(int partPerVoxel = 4) {
		this->partPerVoxel = partPerVoxel;
	}

	float getGridMinPos() const {
		return gridMinPos;
	}

	void setGridMinPos(float gridMinPos) {
		this->gridMinPos = gridMinPos;
	}

	float getVoxelSize() const {
		return voxelSize;
	}

	void setVoxelSize(float voxelSize) {
		this->voxelSize = voxelSize;
	}

	int getGridSize() const {
		return gridSize;
	}

	void setGridSize(int gridSize) {
		this->gridSize = gridSize;
	}
};

#endif /* UNIFORMGRID_H_ */
