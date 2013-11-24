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

#include "Point2.h"
#include "Triangle2.h"
#include "TriangleAroundVertexIterator.h"
#include "Visualizer2.h"
#include "Zone2.h"
#include "ConstraintGraph2.h"
#include "Performance.h"
#include "RefineCandidate.h"


#if GEOM_PSEUDO3D==GEOM_TRUE 
	#include "IsoContours.h"
#endif



#if GEOM_PSEUDO3D==GEOM_TRUE 
	namespace GEOM_FADE25D {
#elif GEOM_PSEUDO3D==GEOM_FALSE
	namespace GEOM_FADE2D {
#else
	#error GEOM_PSEUDO3D is not defined 
#endif 



class Dt2; // Forward

/** \brief Delaunay triangulation - the main class 
*
* Fade_2D represents a Delaunay triangulation in 2D or 2.5D (depends on the used namespace)
*/
class Fade_2D
{
public:
/** \brief Constructor of the main triangulation class
*
* @param numExpectedVertices specifies the number of points that will probably be inserted.
*  
* By default, the internal data structures are initialized for 1000 points and
* they are automatically reconfigured later if necessary. Specifying \e numExpectedVertices
* saves memory but is not required. 
*/
	CLASS_DECLSPEC
	Fade_2D(unsigned numExpectedVertices=1000)
	{
		initFade(numExpectedVertices);	
	}
	
	CLASS_DECLSPEC
	~Fade_2D();


/** \brief Checks if a triangulation is valid
*
* This debug method checks the validity of the data structure. If \e bCheckEmptyCircleProperty 
* is true, then (slow!) multiprecision arithmetic is used to re-check the empty circle property. 
* A debug string can be added as optional parameter msg. Don't call this method unless you 
* assume that something is wrong with the code.
*/ 
	CLASS_DECLSPEC
	bool checkValidity(bool bCheckEmptyCircleProperty,const std::string& msg="");


/** \brief Draws the triangulation as postscript file.
*
* @note This is a convenience function for quick outputs with a default 
* look. It is also possible to use the Visualizer2 class directly to draw 
* arbitrary circles, line segments, vertices and labels with custom colors.  
* 
*/
	CLASS_DECLSPEC
 	void show(const std::string& postscriptFilename);

#if GEOM_PSEUDO3D==GEOM_TRUE
	/** \brief Draws the triangulation in 3D. 
	*
	* @note The free viewer Geomview can be used to view such files   
	*/
	CLASS_DECLSPEC
	void showGeomview(const std::string& filename);
#endif


/** \brief Insert a single 2D point
*
* @param p is the point to be inserted. 
* @return a pointer to the point in the triangulation
* 
* The triangulation keeps a copy of \e p. The return value is a pointer to this copy. 
* If duplicate points are inserted, the triangulation does not create new copies but
* returns a pointer to the copy of the very first insertion. @n
* @n 
* @note This method offers a very good performance but it is still faster if all 
* points are passed at once, if possible.
*/ 
	CLASS_DECLSPEC
	Point2* insert(const Point2& p);
	
/** \brief Insert a vector of 2D points.
*
* @param vInputPoints contains the points to be inserted.
* 
*/ 
	CLASS_DECLSPEC
	void insert(const std::vector<Point2>& vInputPoints);

/** \brief Insert 2D points from a std::vector and store pointers in \e vHandles
*
* @param vInputPoints contains the points to be inserted.
* @param vHandles (empty) is used to return Point2 pointers
* 
* Internally, the triangulation keeps copies of the inserted points which 
* are returned in \e vHandles (in the same order). If duplicate points are
* contained in vInputPoints then only one copy will be made and a pointer
* to this unique copy will be stored in vHandles for every occurance. 
*/ 

	CLASS_DECLSPEC
	void insert(const std::vector<Point2>& vInputPoints,std::vector<Point2*>& vHandles);

#if GEOM_PSEUDO3D==GEOM_FALSE
/** \brief Insert 2D points from an array
*
* @param numPoints is the number of points to be inserted
* @param aCoordinates is an array of \e 2n double values, e.g. {x0,y0,x1,y1,...,xn,yn}
* @param aHandles is an array with size \e n where pointers to the inserted points will be stored
*/ 
	CLASS_DECLSPEC
	void insert(int numPoints,double * aCoordinates,Point2 ** aHandles);
#endif

/** \brief Enable multithreading
*
* This is an experimental feature for Linux systems: Fade2D can be executed on 
* multiple CPU cores. The possible speed up is currently limited to 20% as not
* the whole algorithm is multithreaded yet. 
*/
	CLASS_DECLSPEC
	void enableMultithreading();

/** \brief Locate a triangle which contains \e p
*
* \image html locate.jpg "Figure 1: Point location" 
* \image latex locate.eps "Point location" width=7cm 
*
* The Fade_2D class can be used as a data structure for point location. 
* This method returns a pointer to a triangle which contains \e p. 
* 
* @param p is the query point
* @return a pointer to a Triangle2 object (or NULL if is2D()==false or if \e p is outside the triangulation)
* 
*/ 

	CLASS_DECLSPEC
	Triangle2* locate(const Point2& p);


#if GEOM_PSEUDO3D==GEOM_TRUE
/** \brief Compute the height (z-coordinate) of a certain point
*
* If the coordinates (x,y) are inside the triangulation, this function computes 
* the height and returns true. If the (x,y) is outside, the function returns false.
* 
*/
	CLASS_DECLSPEC
	bool getHeight(double x,double y,double& height) const;
#endif
	
/** \brief Delaunay refinement
 * 
 * Creates a mesh inside the area given by a Zone2 object. 
 * 
 * @param pZone is the zone whose triangles are refined
 * @param minAngleDegree (up to 30) is the desired minimum angle in all triangles
 * @param minEdgeLength is a lower threshold on the edge length. Triangles with smaller edges are not refined.
 * @param maxEdgeLength is an upper threshold on the edge length. Triangles with larger edges are always refined.
 * @param bAllowConstraintSplitting specifies if also constraint edges may be splitted     
 * 
 * @note Constraint edges may enforce small angles nevertheless 
 */
	CLASS_DECLSPEC
	void refine(Zone2* pZone,double minAngleDegree,double minEdgeLength,double maxEdgeLength,bool bAllowConstraintSplitting);
	
/** \brief Number of points
 * 
 * @return the number of points in the triangulation 
 * 
 * @note if the returned number of points is smaller than the number of inserted points
 * then duplicate points have been inserted.
 */
	CLASS_DECLSPEC
	unsigned numberOfPoints() const;

/** \brief Number of triangles
 * 
 * @return the number of triangles in the triangulation (or 0 as long as is2D() is false).   
 * 
 */
	CLASS_DECLSPEC
	unsigned numberOfTriangles() const;


/** \brief Get pointers to all triangles
*
* @param vAllTriangles is an empty vector of Triangle2 pointers. 
* 
* If is2D()==true then pointers to all existing triangles are stored in vAllTriangles.
* If is2D()==false then no triangles exist at all. 
*/ 
	CLASS_DECLSPEC
	void getTrianglePointers(std::vector<Triangle2*>& vAllTriangles) const;

/** \brief Get pointers to all vertices
*
* @param vAllPoints is an empty vector of Point2 pointers. 
* 
* Stores pointers to all vertices of the triangulation in vAllPoints. The order in which
* the points are stored is \e not necessarily the insertion order. For geometrically identical 
* points which have been inserted multiple times, only one pointer exists. Thus vAllPoints.size()
* can be smaller than the number of inserted points.  
* 
*/ 
	CLASS_DECLSPEC
	void getVertexPointers(std::vector<Point2*>& vAllPoints) const;

/** \brief Check if the triangulation contains triangles (which is the case if
* at least 3 non-collinear points exist in the triangulation
*
* As long as all inserted points are collinear the triangulation does not contain
* triangles. This is clearly the case as long as less than three input points are
* present but it may also be the case when 3 or more points have been inserted when
* all these points are collinear. These points are then in a pending state, i.e. 
* they will be triangulated as soon as the first non-collinear point is inserted. 
* 
* \image html coll.jpg "Figure 2: Triangles are generated as soon as the first non-collinear point is inserted."
* \image latex coll.eps "Triangles are generated as soon as the first non-collinear point is inserted." width=7cm
* 
* @return true if at least one triangle exists@n
* false otherwise
*
*/ 
	CLASS_DECLSPEC
	bool is2D() const;

/** \brief Add constraint edges (edges, a polyline or a polygon)
* \anchor createConstraint
* @param vSegments ...contains line segments which shall appear as edges of the 
* triangulation. The order of the line segments is \e not important. If they
* form a closed \e simple polygon then the resulting ConstraintGraph2 object can 
* later be used to define a Zone2. 
* @param cis
* CIS_KEEP_DELAUNAY: Automatically subdivide the constraint edges in order to 
* keep the empty circle property (Conforming Delaunay triangulation)@n
* CIS_IGNORE_DELAUNAY: Do not subdivide (Constrained Delaunay triangulation)@n
*
* @return a pointer to the new ConstraintGraph2 object@n
* @warning The constraint edges must \e NOT intersect each other and they must \e NOT
* intersect the edges of other ConstraintGraph2 objects. If they do, the insertion process 
* is likely to fail or run forever!
*
* \htmlonly <div class="center"> \endhtmlonly
* 
* \image html crop_bareBike.jpg "Figure 3: Delaunay triangulation without constraints"
* \image latex crop_bareBike.eps "Delaunay triangulation without constraints" width=12cm
*
* \image html crop_ignoreBike.jpg "Figure 4: Constraint insertion with CIS_IGNORE_DELAUNAY."
* \image latex crop_ignoreBike.eps "Constraint insertion with CIS_IGNORE_DELAUNAY" width=12cm
* 
* \image html crop_keepBike.jpg "Figure 5: Constraint insertion with CIS_KEEP_DELAUNAY. The inserted segments are automatically subdivided."
* \image latex crop_keepBike.eps "Constraint insertion with CIS_IGNORE_DELAUNAY" width=12cm
*
* \htmlonly </div> \endhtmlonly
* 
*/ 
	CLASS_DECLSPEC
	ConstraintGraph2* createConstraint(std::vector<Segment2>& vSegments,ConstraintInsertionStrategy cis);
	
	
	
	
/** \brief Create a zone bounded by a constraint graph
* 
* \anchor createZone
* A Zone2 object is an area of a triangulation. This method takes a
* ConstraintGraph2 object as boundary of the area. The \e ZoneLocation
* value specifies if the zone is inside or outside of the boundary or
* if it is simply the whole triangulation (ZL_GLOBAL).   
* 
* @param zoneLoc is either ZL_INSIDE, ZL_OUTSIDE or ZL_GLOBAL
* @param pConstraintGraph is a pointer to a formerly created ConstraintGraph2 
* object which must contain a \e simple polygon. In case of zoneLoc==ZL_GLOBAL,
* this pointer is NULL. 
*
* @return a pointer to the new Zone2 object or NULL if pConstraintGraph->isPolygon() 
* returns false.@n
*
* @warning As constraints must \e NOT intersect, the same restriction applies to 
* the boundaries of Zone2 objects. However, a Zone2 can be completely inside
* another Zone2. 
* 
* \see deleteZone(Zone2* pZone).
* 
* \htmlonly <div class="center"> \endhtmlonly
* \image html bikeZones.jpg "Figure 6: Zones in a triangulation"
* \htmlonly </div> \endhtmlonly
*  
* \image latex bikeZones.eps "Zones in a triangulation" width=12cm
*/ 
	CLASS_DECLSPEC
	Zone2* createZone(ConstraintGraph2* pConstraintGraph,ZoneLocation zoneLoc);
	
	
	
/** \brief Create a zone limited by multiple ConstraintGraph2 objects by growing from a start point
* 
* A Zone2 object is an area of the traingulation, see \ref createZone
* 
* @param vConstraintGraphs is a vector of ConstraintGraph objects
* @param zoneLoc must be ZL_GROW
* @param startPoint is the point from which the area is grown until the borders specified in vConstraintGraphs are reached  
* @return a pointer to the new Zone2 object (or NULL if zoneLoc!=ZL_GROW)
*
*/ 
	CLASS_DECLSPEC
	Zone2* createZone(const std::vector<ConstraintGraph2*> vConstraintGraphs,ZoneLocation zoneLoc,const Point2& startPoint);
	
/** \brief Create a zone limited by a ConstraintGraph by growing from a start point
* 
* A Zone2 object is an area of the traingulation, see \ref createZone
* 
* @param pConstraintGraph is a constraint whose edges specify the area's border
* @param zoneLoc must be ZL_GROW
* @param startPoint is the point from which the area is grown until the borders specified in pConstraint are reached  
* @return a pointer to the new Zone2 object (or NULL if zoneLoc!=ZL_GROW)
*
*/ 
	CLASS_DECLSPEC
	Zone2* createZone(ConstraintGraph2* pConstraintGraph,ZoneLocation zoneLoc,const Point2& startPoint);
	
	
	
/** \brief Delete a Zone2 object
* 
* Zone2 objects are automatically destroyed with their Fade_2D objects. In
* addition this method provides the possibility to eliminate Zone2 objects earlier.
*/
	CLASS_DECLSPEC
	void deleteZone(Zone2* pZone);
		
	
	
	
/** \brief Apply all formerly defined constraints and zones. 
* 
* For technical reasons all defined constraints are inserted at the end of the 
* triangulation process. This step must be triggered manually, i.e., it is
* up to the user to call applyConstraintsAndZones() before the resulting triangulation
* is used. If afterwards more points are inserted, applyConstraintsAndZones() must 
* be called again.
* 
* @warning This command may fail or run forever when constraints are inserted which
* intersect each other. The user must provide clean input data.
* 
*/ 
	CLASS_DECLSPEC
	void applyConstraintsAndZones();



/** \brief Compute the axis-aligned bounding box of the points
*
* If no points have been inserted yet, then the returned Bbox2 object
* is invalid and its member function \e Bbox2::isValid() will return false.
* 
*/
	CLASS_DECLSPEC
	Bbox2 computeBoundingBox() const;

/** \brief Check if an edge is a constraint edge
*
* Returns whether the edge in triangle \e pT which is opposite to the
* \e ith vertex is a constraint edge.
* 
*/
	CLASS_DECLSPEC
	bool isConstraint(Triangle2* pT,int ith) const;

	CLASS_DECLSPEC
	void printLicense() const;
	
protected:
	void initFade(unsigned numExpectedVertices);
	Fade_2D(const Fade_2D&); // No copy constructor
	Fade_2D& operator=(const Fade_2D&); // No assignment allowed
	Dt2* pImpl;
};

CLASS_DECLSPEC
std::string getFade2DVersion();


} // (namespace)



namespace GEOM_LIC
{
	CLASS_DECLSPEC
	void setLic(
		const std::string& l1,
		const std::string& l2,
		const std::string& dt,
		const std::string& s1,
		const std::string& s2_
		);
	class Lic; // FWD
	Lic* getLic();
}

#ifndef GEOM_NOLIC
	#include "License.h"
#endif

