#ifndef PCT_structuresH
#define PCT_structuresH
//---------------------------------------------------------------------------
#include <math.h>
#include <float.h>
#include <tnt/tnt.h>
#include <set>
#include <vector>

//---------------------------------------------------------------------------
#define PI 3.1415926535897932					// This is our famous PI
//---------------------------------------------------------------------------

using namespace TNT;

struct PCTCVector2
{
public:

	// A default constructor
	PCTCVector2() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	PCTCVector2(float X, float Y)
	{
		x = X; y = Y;
	}

	// Here we overload the * operator so we can multiply by scalars
	PCTCVector2 operator*(float num)
	{
		// Return the scaled vector
		return PCTCVector2(x * num, y * num);
	}
	PCTCVector2 operator/(float num)
	{
		// Return the scale vector
		return PCTCVector2(x / num, y / num);
	}
	// Here we overload the + operator so we can add vectors together
	PCTCVector2 operator+(PCTCVector2 vVector)
	{
		// Return the added vectors result.
		return PCTCVector2(vVector.x + x, vVector.y + y);
	}
	PCTCVector2 operator-(PCTCVector2 vVector)
	{
		// Return the added vectors result.
		return PCTCVector2(vVector.x - x, vVector.y - y);
	}

	float x, y;
};

// This is our basic 3D point/vector class
struct PCTCVector3
{
public:

	// A default constructor
	PCTCVector3() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	PCTCVector3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}

	// Here we overload the + operator so we can add vectors together
	PCTCVector3 operator+(PCTCVector3 vVector)
	{
		// Return the added vectors result.
		return PCTCVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// Here we overload the - operator so we can subtract vectors
	PCTCVector3 operator-(PCTCVector3 vVector)
	{
		// Return the subtracted vectors result
		return PCTCVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	// Here we overload the * operator so we can multiply by scalars
	PCTCVector3 operator*(float num)
	{
		// Return the scaled vector
		return PCTCVector3(x * num, y * num, z * num);
	}

	// Here we overload the / operator so we can divide by a scalar
	PCTCVector3 operator/(float num)
	{
		// Return the scale vector
		return PCTCVector3(x / num, y / num, z / num);
	}
	float x, y, z;
};

struct PCTNeighVisualization
{
	bool isE_local_visualize;
	PCTCVector3 * visNormals;
	std::set<int> localNeighs;
	std::vector<int> edges;
	//PCTCVector2 * pointsInTangentPlane;
	PCTNeighVisualization(){
		visNormals = new PCTCVector3[4];
	}
	~PCTNeighVisualization(){
		delete[] visNormals;
	}
};

struct PCTMeshGraph {
	int numOfVertices;
	PCTCVector3 * pVerts;			// The object's vertices
	Array2D<bool> E;

	PCTMeshGraph(){
		pVerts = new PCTCVector3[0];
	}
	~PCTMeshGraph(){
		delete[] pVerts;
		pVerts = NULL;
	}
};

//---------------------------------------------------------------------------


//	This returns a perpendicular vector from 2 given vectors by taking the cross product.
PCTCVector3 Cross(PCTCVector3 vVector1, PCTCVector3 vVector2);

//	This returns the magnitude of a normal (or any other vector)
float Magnitude(PCTCVector3 vNormal);

//	This returns a normalize vector (A vector exactly of length 1)
PCTCVector3 Normalize(PCTCVector3 vNormal);

//	This returns the normal of a polygon (The direction the polygon is facing)
PCTCVector3 Normal(PCTCVector3 vPolygon[]);

// This returns the dot product between 2 vectors
float Dot(PCTCVector3 vVector1, PCTCVector3 vVector2);

// This returns the distance between 2 3D points
float Distance(PCTCVector3 vPoint1, PCTCVector3 vPoint2);

// This returns the distance between 2 2D points
float Distance(PCTCVector2 vPoint1, PCTCVector2 vPoint2);

// This returns the angle between 2 vectors
double AngleBetweenVectors(PCTCVector3 vector1, PCTCVector3 vector2);

double OrientedAngleBetweenVectors(PCTCVector3 vector1, PCTCVector3 vector2);

float DeterminantInPlane(PCTCVector3 vVector1, PCTCVector3 vVector2, PCTCVector3 vVector3);

//---------------------------------------------------------------------------
#endif
