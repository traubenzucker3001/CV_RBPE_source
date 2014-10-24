

#ifndef UNIFORMGRID_H_
#define UNIFORMGRID_H_

class UniformGrid : public Singleton<UniformGrid> {
	friend class Singleton<UniformGrid>;

private:
	int indexGrid;
	int countGrid;
	float voxelSize;
	float gridMinimumPosition;
	int gridLength;
	int particlesPerVoxel;

public:
	void createGrid();
	void updateGrid();

};

//könnte man eig in klasse packen
extern int* indexGrid;
extern int* countGrid;
extern float voxelSize;
extern float gridMinimumPosition;
extern int gridLength;
extern int particlesPerVoxel;

void createGrid();
void updateGrid();
bool isValidIndex(int* gridIndex);
int* getNeighborParticleIndices(int* gridIndex);


#endif /* UNIFORMGRID_H_ */
