// (c) 2010 Geom e.U. Bernhard Kornberger, Graz/Austria. All rights reserved.
//
// This file is part of the Fade2D library. You can use it for your personal
// non-commercial research. Licensees holding a commercial license may use this 
// file in accordance with the Commercial License Agreement provided 
// with the Software.
//
// This software is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Please contact the author if any conditions of this licensing are not clear 
// to you.
// 
// Author: Bernhard Kornberger, bkorn (at) geom.at 
// http://www.geom.at


#pragma once
#include "Segment2.h"
#include "ConstraintSegment2.h"
#include <map>


#include "common.h"
#if GEOM_PSEUDO3D==GEOM_TRUE 
	namespace GEOM_FADE25D {
#elif GEOM_PSEUDO3D==GEOM_FALSE
	namespace GEOM_FADE2D {
#else
	#error GEOM_PSEUDO3D is not defined 
#endif 
	
class Dt2; // Forward
class ConstraintSegment2; // Forward
class GeomTest; // Forward


/** \brief A set of enforced edges
*
* \see \ref createConstraint in the Fade2D class
* 
* \image html crop_ignoreBike.jpg "Constraints in a triangulation"
* \image latex crop_ignoreBike.eps "Constraints in a triangulation" width=12cm
*/

class ConstraintGraph2
{
public:
/** \brief Constructor for a one or more segments
*/
	CLASS_DECLSPEC 
	ConstraintGraph2(Dt2* pDt2_,std::vector<ConstraintSegment2*>& vCSegments,ConstraintInsertionStrategy cis_);

/** \brief Does the constraint graph form a closed polygon?
* 
* @warning This method does not check if it is a simple polygon (without self-intersections)!
*/
	CLASS_DECLSPEC 
	bool isPolygon() const;

/** \brief Get the vertices of the constraint segments
* 
* Use this method to retrieve the segments of *this in form of a vector
* of vertices. If *this is a closed polygon, then the points are ordered
* and oriented in counterclockwise direction, e.g. (a,b,b,c,c,d,d,a). If
* the *this is not a polygon, then the segments are returned in the 
* original direction.
* 
* @note If it was necessary to split the constraint segments, then the
* splitted segments are returned. For example, if the constraint
* segment (a,b) crosses a previously inserted point x, then the
* result is (a,x,x,b,b,c,c,d,d,a).
* 
* @note It is not sufficient to have a ConstraintGraph object with
* certain constraint segments. The ConstraintGrap must also be applied.   
*/
	CLASS_DECLSPEC 
	void getPolygonVertices(std::vector<Point2*>& vTriangulationPoints_) const; 

/** \brief Get the insertion strategy
* 
* @return CIS_KEEP_DELAUNAY or @n
* CIS_IGNORE_DELAUNAY
*/
	CLASS_DECLSPEC 
	ConstraintInsertionStrategy getInsertionStrategy() const;
	

/** \brief Check if an edge is a constraint
* 
* Checks if the edge (p0,p1) is a constraint of *this
*/
	CLASS_DECLSPEC
	bool isConstraint(Point2* p0,Point2* p1) const;


/** \brief Get the original ConstraintSegment2 objects
* 
* Get the original, not subdivided ConstraintSegment2 objects. The
* ones which have been splitted are not alive anymore. But they 
* have children (for which the same may hold).
* 
*/
	CLASS_DECLSPEC
	void getOriginalConstraintSegments(std::vector<ConstraintSegment2*>& vConstraintSegments_) const;


/** \brief Update a constraint segment of *this
* Internal method
*/	
	void updateSplittedConstraintSegment(ConstraintSegment2* pCSeg,bool bDirChange0,bool bDirChange1,ConstraintSegment2* pChild0,ConstraintSegment2* pChild1);

protected:
	bool isReverse(ConstraintSegment2* pCSeg) const;
	bool checkAndSortPolygon(std::vector<ConstraintSegment2*>& vCSegments_);	
	void getAliveConstraintChain(std::vector<ConstraintSegment2*>& vAliveCSeg) const;

	// Data
	Dt2* pDt2;
	GeomTest* pGeomPredicates;
	ConstraintInsertionStrategy cis;
	std::vector<ConstraintSegment2*> vCSegParents; 
	bool bIsPolygon;
	std::map<ConstraintSegment2*,bool,func_ltDerefPtr<ConstraintSegment2*> > mCSegReverse;
};

} // (namespace)
