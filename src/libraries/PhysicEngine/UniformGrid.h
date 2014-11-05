
#ifndef UNIFORMGRID_H_
#define UNIFORMGRID_H_

// <<<<<<<<<< includes >>>>>>>>>> //
#include <glm/glm.hpp>
#include "DesignPatterns\Singleton.h"

//nicht sicher ob überhaupt singleton sein muss
/** \brief Uniform Grid
 *
 * ...
 */
class UniformGrid : public Singleton<UniformGrid> {
	friend class Singleton<UniformGrid>;

private:
	int* indexGrid;			/**< ... */
	//?
	int* countGrid; 		/**< ... */
	float voxelSize;		/**< voxel size */
	//4 nach gpu gems
	int partPerVoxel = 4;	/**< particles per voxel */
	//?
	float gridMinPos;		/**< grid minimum position */
	int gridLength;			/**< grid lenght */

	int gridSize;			/**< grid size */
	int xSteps;				/**< ... */
	int ySteps;				/**< ... */

public:

	/** \brief constructor
	 *
	 * ...
	 */
	UniformGrid();

	/** \brief destructor
	 *
	 * ...
	 */
	~UniformGrid();

	/** \brief create uniform grid
	 *
	 * ...
	 */
	void createGrid();

	/** \brief update uniform grid
	 *
	 * ...
	 */
	void updateGrid();

	//
	/** \brief is valid grid index
	 *
	 * ...
	 */
	bool isValidIndex(glm::vec3 gridIndex);

	/** \brief get neighbor particle indices
	 *
	 * ...
	 */
	int* getNeighborPartIndices(glm::vec3 gridIndex);

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
