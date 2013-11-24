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

#include "common.h"
#if GEOM_PSEUDO3D==GEOM_TRUE 
	namespace GEOM_FADE25D {
#elif GEOM_PSEUDO3D==GEOM_FALSE
	namespace GEOM_FADE2D {
#else
	#error GEOM_PSEUDO3D is not defined 
#endif 



class Dt2; // Fwd
class ConstraintGraph2; // Fwd
class Triangle2; // Fwd
class Point2; // Fwd



/** \brief Area in a triangulation. 
*
*  \see \ref createZone in the Fade2D class
* 
*/

class Zone2
{
public:
	CLASS_DECLSPEC
	Zone2(Dt2* pDt_,ZoneLocation zoneLoc_);
	CLASS_DECLSPEC
	Zone2(Dt2* pDt_,ZoneLocation zoneLoc_,ConstraintGraph2* pConstraintGraph_);
	CLASS_DECLSPEC
	Zone2(Dt2* pDt_,const std::vector<ConstraintGraph2*>& vConstraintGraphs_,ZoneLocation zoneLoc_,double startPointX_,double startPointY_);
/** \brief Returns the ZoneLocation value 
*
* \returns ZL_INSIDE if the zone applies to the triangles inside pConstraintGraph@n
* ZL_OUTSIDE if the zone applies to the outside triangles@n
* ZL_GLOBAL if the zone holds all triangles@n
* ZL_RESULT if the zone is the result of a set operation@n
* ZL_GROW if the zone is specified by a set of constraint graphs and an inner point@n
* \image html in_and_outside_zone.jpg "An ouside zone and in inside zone"
* \image latex in_and_outside_zone.eps "An ouside zone and in inside zone" width=12cm
*/
	CLASS_DECLSPEC
	ZoneLocation getZoneLocation() const;
	CLASS_DECLSPEC
	bool assignDt2(Dt2* pDt_);
/** \brief Returns the triangles of the zone.  
*
* @warning Fade_2D::void applyConstraintsAndZones() must be called after the last 
* insertion of points and constraints. Otherwise the result won't be valid.
*/
	CLASS_DECLSPEC
	void getTriangles(std::vector<Triangle2*>& vTriangles_) const;
/** \brief Get the associated constraint
* @return a pointer to the ConstraintGraph2 object which defines the zone.@n
* or NULL for ZL_RESULT-, ZL_GROW and ZL_GLOBAL-zones.  
*/
	CLASS_DECLSPEC
	ConstraintGraph2* getConstraintGraph() const;
	void getConstraintGraphs(std::vector<ConstraintGraph2*>& vConstraintGraphs_) const;
/** \brief Get a pointer to the associated Delaunay triangulation   
*/
	CLASS_DECLSPEC
	Dt2* getDelaunayTriangulation() const;

/** \brief Get a the number of ConstraintGraph2 objects
* 
* A Zone2 object might be defined by zero, one or more ConstraintGraph2 objects.    
*/
	CLASS_DECLSPEC
	size_t numberOfConstraintGraphs() const;

protected:
	Zone2(const Zone2&);


	// Data
	Dt2* pDt;
	ZoneLocation zoneLoc;
	//ConstraintGraph2* pConstraintGraph;
//	std::vector<Triangle2*> vResultTriangles;
	Zone2* pInputZone0;
	Zone2* pInputZone1;
	std::vector<ConstraintGraph2*> vConstraintGraphs;
	double startPointX,startPointY;
	
	CLASS_DECLSPEC
	friend Zone2* zoneUnion(Zone2* pZone0,Zone2* pZone1);
	CLASS_DECLSPEC
	friend Zone2* zoneIntersection(Zone2* pZone0,Zone2* pZone1);
	CLASS_DECLSPEC
	friend Zone2* zoneDifference(Zone2* pZone0,Zone2* pZone1);
	CLASS_DECLSPEC
	friend Zone2* zoneSymmetricDifference(Zone2* pZone0,Zone2* pZone1);

};

// Free functions

/** \brief Compute the union of two zones
* @return a new zone containing the union of the triangles of *pZone0 and *pZone1
*/
CLASS_DECLSPEC
Zone2* zoneUnion(Zone2* pZone0,Zone2* pZone1);
/** \brief Compute the intersection of two zones
* @return a new zone containing the intersection of *pZone0 and *pZone1
*/
CLASS_DECLSPEC
Zone2* zoneIntersection(Zone2* pZone0,Zone2* pZone1);
/** \brief Compute the difference of two zones
* @return a new zone containing the triangles of *pZone0 minus the ones of *pZone1
*/
CLASS_DECLSPEC
Zone2* zoneDifference(Zone2* pZone0,Zone2* pZone1);
/** \brief Compute the symmetric difference of two zones
* @return a new zone containing the triangles that are present in one of 
* the zones but not in the other one. 
*/
CLASS_DECLSPEC
Zone2* zoneSymmetricDifference(Zone2* pZone0,Zone2* pZone1);


} // (namespace)
