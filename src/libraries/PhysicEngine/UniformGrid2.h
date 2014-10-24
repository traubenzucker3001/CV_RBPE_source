
#ifndef UNIFORMGRID2_H_
#define UNIFORMGRID2_H_

//nicht sicher ob überhaupt singleton sein muss
class UniformGrid : public Singleton<UniformGrid> {
	friend class Singleton<UniformGrid>;

private:
	int* indexGrid;
	int* countGrid; 		 //?
	float voxelSize;
	int partPerVoxel = 4;	//4 nach gpu gems
	float gridMinPosition;	//?
	int gridLength;

public:

	void createGrid();
	void updateGrid();

	//
	bool isValidIndex(int* gridIndex);
	int* getNeighborParticleIndices(int* gridIndex);
};

#endif /* UNIFORMGRID2_H_ */
