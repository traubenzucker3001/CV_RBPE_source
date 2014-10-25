
#ifndef UNIFORMGRID2_H_
#define UNIFORMGRID2_H_

// <<<<<<<<<< includes >>>>>>>>>> //


//nicht sicher ob überhaupt singleton sein muss
class UniformGrid : public Singleton<UniformGrid> {
	friend class Singleton<UniformGrid>;

private:
	int* indexGrid;
	int* countGrid; 		 //?
	float voxelSize;
	int partPerVoxel = 4;	//4 nach gpu gems
	float gridMinPos;		//?
	int gridLength;

	int gridSize;
	int xSteps;
	int ySteps;

public:

	UniformGrid();
	~UniformGrid();

	void createGrid();
	void updateGrid();

	//
	bool isValidIndex(int* gridIndex);
	int* getNeighborParticleIndices(int* gridIndex);

	// <<<<<<<<<< getter + setter >>>>>>>>>> //

	int getPartPerVoxel() const {
		return partPerVoxel;
	}

	void setPartPerVoxel(int partPerVoxel = 4) {
		this->partPerVoxel = partPerVoxel;
	}
};

#endif /* UNIFORMGRID2_H_ */
