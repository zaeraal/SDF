#pragma once


#include "Point2.h"

#include "common.h"
#if GEOM_PSEUDO3D==GEOM_TRUE 
	namespace GEOM_FADE25D {
#elif GEOM_PSEUDO3D==GEOM_FALSE
	namespace GEOM_FADE2D {
#else
	#error GEOM_PSEUDO3D is not defined 
#endif 

class Triangle2; // Fwd
class Dt2; // Fwd
class ConstraintSegment2; // Fwd
	
class RefineCandidate
{
public:
	RefineCandidate(Dt2* pDt_,Triangle2* pTriangle_,double minAngleGrad_);

	CLASS_DECLSPEC
	Triangle2* getTriangle() const;

	CLASS_DECLSPEC
	Point2 getCircumcenter() const;

	CLASS_DECLSPEC
	double getRadius() const;

	CLASS_DECLSPEC
	std::pair<Triangle2*,int> getContainingTriangleAndIndex() const;

	CLASS_DECLSPEC
	bool dualCloseToInfinity() const;

	CLASS_DECLSPEC
	bool isConstraint(int ith) const;

	CLASS_DECLSPEC
	ConstraintSegment2* getConstraintSegment(int ith) const;

	CLASS_DECLSPEC
	bool operator>(const RefineCandidate& pOther) const;

	CLASS_DECLSPEC
	bool isAlive() const;

	CLASS_DECLSPEC
	void checkEnc();

	int debug_label;	
	static int debug_runningLabel;
	void debug();

protected:
	
	int walkUntilConstraint(Point2* pVtx,Triangle2*& pStartTriangle);
	Dt2* pDt;
	Triangle2* pTriangle;
	Triangle2* pContainingT;
	Point2 circumcenter;
	double radius;
	int splitIndex;
	bool bAccurateCC;
	Point2* vOriginalPoints[6];
	double minAngleRad;
	
	
};
} // Namespace

