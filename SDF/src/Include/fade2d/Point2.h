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
#include "Vector2.h"

#if GEOM_PSEUDO3D==GEOM_TRUE 
	namespace GEOM_FADE25D {
#elif GEOM_PSEUDO3D==GEOM_FALSE
	namespace GEOM_FADE2D {
#else
	#error GEOM_PSEUDO3D is not defined 
#endif 

const double INV=-999;

class Triangle2;
extern bool benchCounter(const std::string& str);
/** \brief Point
* 
* This class represents a point in 2D with x- and y-coordinates and an additional pointer
* to an associated triangle. 
*/
class Point2
{
public:
	
#if GEOM_PSEUDO3D==GEOM_TRUE
/** \brief Constructor
*
* @param x_ x-coordinate
* @param y_ y-coordinate
* @param z_ z-coordinate
*/
	CLASS_DECLSPEC 
	Point2(const double x_,const double y_,const double z_):
		coordX(x_),
		coordY(y_),
		coordZ(z_),
		pAssociatedTriangle(NULL),
		customIndex(-1)
	{
		BC("Point2 Constructor default");
		customIndex=-1;
	}
/** \brief Default constructor
*
* The coordinates are initialized to -999.
*/
	CLASS_DECLSPEC 
	Point2():coordX(INV),coordY(INV),coordZ(INV),pAssociatedTriangle(NULL)
	{
	
	}
/** \brief Copy constructor
*
* Create a point as a copy of p_. The associated triangle pointer is initialized to NULL
*/
	CLASS_DECLSPEC 
	Point2(const Point2& p_):
		coordX(p_.x()),
		coordY(p_.y()),
		coordZ(p_.z()),
		pAssociatedTriangle(NULL),
		customIndex(p_.customIndex)
	{
		
	}
	
#else

/** \brief Constructor
*
* @param x_ x-coordinate
* @param y_ y-coordinate
*/
	CLASS_DECLSPEC 
	Point2(const double x_,const double y_):
		coordX(x_),
		coordY(y_),
		pAssociatedTriangle(NULL),
		customIndex(-1)
	{
		BC("Point2 Constructor default");
	}
/** \brief Default constructor
*
* The coordinates are initialized to -999.
*/
	CLASS_DECLSPEC 
	Point2():
		coordX(INV),
		coordY(INV),
		pAssociatedTriangle(NULL),
		customIndex(-1)
	{
	}
/** \brief Copy constructor
*
* Create a point as a copy of p_. The associated triangle pointer is initialized to NULL
*/
	CLASS_DECLSPEC 
	Point2(const Point2& p_):
		coordX(p_.x()),
		coordY(p_.y()),
		pAssociatedTriangle(NULL),
		customIndex(p_.customIndex)
	{
	}
#endif

	CLASS_DECLSPEC 
	~Point2()
	{
	}
	
/** \brief Get the x-coordinate
*
* @return the x-coordinate
*/
	CLASS_DECLSPEC 
	double x() const
	{
		return coordX;
	}
/** \brief Get the y-coordinate
*
* @return the y-coordinate
*/
	CLASS_DECLSPEC 
	double y() const
	{
		return coordY;
	}

#if GEOM_PSEUDO3D==GEOM_TRUE
/** \brief Get the z-coordinate. 
*
* @return the z-coordinate
*/
	CLASS_DECLSPEC 
	double z() const
	{
		return coordZ;	
	}
#endif

#if GEOM_PSEUDO3D==GEOM_TRUE
/** \brief Get the x-, y- and z-coordinate
* 
* @param x_ x-coordinate
* @param y_ y-coordinate
* @param z_ z-coordinate
* 
*/
	CLASS_DECLSPEC 
	void xyz(double& x_,double& y_,double& z_) const
	{
		x_=coordX;
		y_=coordY;
		z_=coordZ;
	}
#endif

/** \brief Get the x- and y-coordinate
* 
* @param x_ x-coordinate
* @param y_ y-coordinate
* 
*/
	CLASS_DECLSPEC 
	void xy(double& x_,double& y_) const
	{
		x_=coordX;
		y_=coordY;
	}

/** \brief Less than operator
*
* Compares the x and y coordinates 
* 
* @note Although a point has a z-coordinate in the 2.5D version only x and y a compared by this method  
*/
	CLASS_DECLSPEC 
	bool operator<(const Point2& p) const
	{
		if(coordX<p.coordX) return true;
		if(coordX>p.coordX) return false;
		if(coordY<p.coordY) return true;
		return false;
	}
/** \brief Equality operator
*
* Compares the x and y coordinates 
* 
* @note Although a point has a z-coordinate in the 2.5D version only x and y a compared by this method 
*/
	CLASS_DECLSPEC 
	bool operator==(const Point2& p) const
	{
		if(coordX==p.coordX && coordY==p.coordY) return true;
		return false;
	}
/** \brief Inequality operator
*
* Compares the x and y coordinates 
* 
* @note Although a point has a z-coordinate in the 2.5D version only x and y a compared by this method 
*/
	CLASS_DECLSPEC 
	bool operator!=(const Point2& p) const
	{
		if(coordX!=p.coordX || coordY!=p.coordY) return true;
		return false;
	}
/** \brief Get the associated triangle
* 
* @return the associated triangle
*/
	CLASS_DECLSPEC 
	Triangle2* getIncidentTriangle() const
	{
		return pAssociatedTriangle;
	}


#if GEOM_PSEUDO3D==GEOM_TRUE
/** \brief Set the coordinates
*
* @param x_ x-coordinate
* @param y_ y-coordinate
* @param z_ z-coordinate
* 
* 
*/

	void set(const double x_,const double y_,const double z_)
	{
		coordX=x_;
		coordY=y_;
		coordZ=z_;
		pAssociatedTriangle=NULL;
	}
#else
/** \brief Set the coordinates
*
* @param x_ x-coordinate
* @param y_ y-coordinate
*/
	void set(const double x_,const double y_)
	{
		coordX=x_;
		coordY=y_;
		pAssociatedTriangle=NULL;
	}
	void change(const double x_,const double y_)
	{
		coordX=x_;
		coordY=y_;
	}
#endif

/** \brief Set the coordiantes
*
* @param pnt is the point whose coordinates are assigned to the current point
*/
#if GEOM_PSEUDO3D==GEOM_TRUE
	void set(const Point2& pnt)
	{
		coordX=pnt.x();
		coordY=pnt.y();
		coordZ=pnt.z();
		pAssociatedTriangle=NULL;
		this->customIndex=pnt.customIndex;
	}
#else
	void set(const Point2& pnt)
	{
		coordX=pnt.x();
		coordY=pnt.y();
		pAssociatedTriangle=NULL;
		this->customIndex=pnt.customIndex;
	}
#endif
/** \brief Set a custom index
*
* An arbitrary index can be assigned. 
* 
* @note This method is provided for the users' convenience. It has nothing
* to do with the internal data structures of Fade 2D and using this method
* is optional. By default this index is -1.
*/
	CLASS_DECLSPEC 
	void setCustomIndex(int customIndex_)
	{
		customIndex=customIndex_;	
	}
/** \brief Get the vertex index
*
* @return the custom index. 
*  
* @note The custom index defaults to -1. It is not the index of the point 
* in the triangulation (such an index does not exist) but an arbitrary
* value which can be set by the user. 
* 
* \see setCustomIndex(int customIndex_) 
*/
	CLASS_DECLSPEC 
	int getCustomIndex() const
	{
		return customIndex;
	}

/** \brief Associate a triangle with the point
*
* @param pT will be associated with the triangle
*/
	void setIncidentTriangle(Triangle2* pT)
	{
		pAssociatedTriangle=pT;
	}

Vector2 operator-(const Point2& other) const
{
#if GEOM_PSEUDO3D==GEOM_TRUE
	double xdiff(x()-other.x());
	double ydiff(y()-other.y());
	double zdiff(z()-other.z());
	return Vector2(xdiff,ydiff,zdiff);
#else
	double xdiff(x()-other.x());
	double ydiff(y()-other.y());
	return Vector2(xdiff,ydiff);
#endif

}


	
	friend std::ostream &operator<<(std::ostream &stream, const Point2& pnt);
	friend std::istream &operator>>(std::istream &stream, Point2& pnt);


protected:
friend class Dt2;
	double coordX;
	double coordY;
#if GEOM_PSEUDO3D==GEOM_TRUE
	double coordZ;
#endif
	Triangle2* pAssociatedTriangle;
	int customIndex;
};


inline std::ostream &operator<<(std::ostream &stream, const Point2& pnt)
{
#if GEOM_PSEUDO3D==GEOM_TRUE
	stream << "Point2 ("<<&pnt<<","<<pnt.customIndex<<"): "<<pnt.x()<<", "<<pnt.y()<<", "<<pnt.z();
#else
	stream << "Point2 ("<<&pnt<<","<<pnt.customIndex<<"): "<<pnt.x()<<", "<<pnt.y();
#endif


	return stream;
}

inline std::istream &operator>>(std::istream &stream, Point2& pnt)
{
#if GEOM_PSEUDO3D==GEOM_TRUE
	stream >> pnt.coordX >> pnt.coordY >> pnt.coordZ;
#else
	stream >> pnt.coordX >> pnt.coordY;
#endif
	return stream;
}

// Free functions

/** \brief Get the squared distance between two points in 2D
*
* @note the 2.5D version will not use the z-coordinate of the points
*/
inline
double sqDistance(const Point2& p0,const Point2& p1)
{
	double deltaX=p1.x()-p0.x();
	double deltaY=p1.y()-p0.y();
	return (deltaX*deltaX+deltaY*deltaY);
}


/** \brief Get the squared distance between two points in 2D
*
* @note the 2.5D version will not use the z-coordinate of the points
*/
inline
double sqDistance(const double x0,const double y0,const Point2& p1)
{
	double deltaX=p1.x()-x0;
	double deltaY=p1.y()-y0;
	return (deltaX*deltaX+deltaY*deltaY);
}

/** \brief Compute the midpoint of p0 and p1
*
* @note the 2.5D version will assign the arithmetic mean of p0.z() and p1.z()
* as z-coordinate. Use Fade2D::getHeight() to compute the height of a 
* point inside the triangulation.
*/

inline
Point2 center(const Point2& p0,const Point2& p1)
{
#if GEOM_PSEUDO3D==GEOM_TRUE
	return Point2((p0.x()+p1.x())/2.0,(p0.y()+p1.y())/2.0,(p0.z()+p1.z())/2.0);
#else
	return Point2((p0.x()+p1.x())/2.0,(p0.y()+p1.y())/2.0);
#endif
}


} // (namespace)
