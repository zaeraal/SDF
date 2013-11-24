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
#include "Point2.h"

#include "common.h"
#if GEOM_PSEUDO3D==GEOM_TRUE 
	namespace GEOM_FADE25D {
#elif GEOM_PSEUDO3D==GEOM_FALSE
	namespace GEOM_FADE2D {
#else
	#error GEOM_PSEUDO3D is not defined 
#endif 

class GeomTest; // FWD

class Bbox2
{
public:
	Bbox2(GeomTest* pGeomTest_=NULL):minX(DBL_MAX),minY(DBL_MAX),maxX(DBL_MIN),maxY(DBL_MIN),bValid(false),pGeomTest(pGeomTest_)
	{
	}

	Bbox2(std::vector<Point2>::const_iterator start_it,std::vector<Point2>::const_iterator end_it,GeomTest* pGeomTest_=NULL):minX(DBL_MAX),minY(DBL_MAX),maxX(DBL_MIN),maxY(DBL_MIN),pGeomTest(pGeomTest_)
	{
		add(start_it,end_it);
	}
	
	bool isValid()
	{
		return bValid;	
	}

	inline void treatPointForValidBox(const Point2& p)
	{
		double x,y;
		p.xy(x,y);
		if(x<minX) minX=x;
			else if(x>maxX) maxX=x;
		if(y<minY) minY=y;
			else if(y>maxY) maxY=y;
	}

	inline void treatPointForInvalidBox(const Point2& p)
	{
		p.xy(minX,minY);
		p.xy(maxX,maxY);
		bValid=true;
	}

	// Returns true iff the bounding box changes
	bool add(std::vector<Point2>::const_iterator start_it,std::vector<Point2>::const_iterator end_it)
	{
		if(start_it==end_it) return false;
		if(bValid)
		{
			double oldMinX(minX),oldMinY(minY),oldMaxX(maxX),oldMaxY(maxY);
			for(;start_it!=end_it;++start_it) treatPointForValidBox(*start_it);
			if(oldMinX!=minX || oldMinY!=minY || oldMaxX!=maxX || oldMaxY!=maxY ) return true;
			return false;
		}
		else
		{
			treatPointForInvalidBox(*start_it);
			++start_it;
			for(;start_it!=end_it;++start_it) treatPointForValidBox(*start_it);
			return true;
		}
	}

	bool add(double * coordinates,int numCoordinates)
	{
		if(numCoordinates==0) return false;
		double oldMinX(minX),oldMinY(minY),oldMaxX(maxX),oldMaxY(maxY);
		double x(coordinates[0]);
		double y(coordinates[1]);
		if(x<minX) minX=x;
		if(x>maxX) maxX=x;		
		if(y<minY) minY=y;
		if(y>maxY) maxY=y;		
		
		int siz(numCoordinates/2);
		for(int i=0;i<siz;++i)
		{
			double x(coordinates[2*i]);
			double y(coordinates[2*i+1]);
			if(x<minX) minX=x;
				else if(x>maxX) maxX=x;		
			if(y<minY) minY=y;
				else if(y>maxY) maxY=y;		
		}
		bValid=true;
		if(oldMinX!=minX || oldMinY!=minY || oldMaxX!=maxX || oldMaxY!=maxY ) return true;
			else return false;	
	}


	// Returns true iff the bounding box changes
	bool add(const Point2& p)
	{
		//std::cout<<"Add point: "<<p<<std::endl;
		if(bValid)
		{
			double oldMinX(minX),oldMinY(minY),oldMaxX(maxX),oldMaxY(maxY);
			treatPointForValidBox(p);
			if(oldMinX!=minX || oldMinY!=minY || oldMaxX!=maxX || oldMaxY!=maxY ) return true;
				else return false;	
		}
		else
		{
			treatPointForInvalidBox(p);
			return true;	
		}
	}
	
	void doubleTheBox();
		
	Bbox2 operator+(const Bbox2& b)
	{
		if(b.minX<minX) minX=b.minX;
		if(b.maxX>maxX) maxX=b.maxX;
		if(b.minY<minY) minY=b.minY;
		if(b.maxY>maxY) maxY=b.maxY;
		return *this;
	}
	bool isValid() const
	{
		return minX<DBL_MAX;	
	}
	Point2 getMinPoint() const
	{
#if GEOM_PSEUDO3D==GEOM_TRUE
		return Point2(minX,minY,0);
#else
		return Point2(minX,minY);
#endif	
	}
	Point2 getMaxPoint() const
	{
#if GEOM_PSEUDO3D==GEOM_TRUE
		return Point2(maxX,maxY,0);
#else
		return Point2(maxX,maxY);
#endif	
	}
	double getMinCoord() const
	{
		if(minX<minY) return minX;
			else return minY;
	}
	double getMaxCoord() const
	{
		if(maxX>maxY) return maxX;
			else return maxY;
	}
	double getRangeX() const
	{
		return maxX-minX;
	}
	double getRangeY() const
	{
		return maxY-minY;
	}
	double getMaxRange() const
	{
		double range0=getRangeX();
		double range1=getRangeY();
		if(range0>range1) std::swap(range0,range1);
		return range1;	
	}
	double getMaxSqRange() const
	{
		return getMaxCoord()-getMinCoord(); 
	}
	double get_minX() {return minX;}
	double get_minY() {return minY;}
	double get_maxX() {return maxX;}
	double get_maxY() {return maxY;}
	
protected:
	double minX,minY;
	double maxX,maxY;
	bool bValid;
	friend std::ostream &operator<<(std::ostream &stream, Bbox2& pC);
	GeomTest* pGeomTest;
};


inline std::ostream &operator<<(std::ostream &stream, Bbox2& pC)
{
	stream<<"Bbox2: ("<<pC.minX<<","<<pC.minY<<") -> ("<<pC.maxX<<","<<pC.maxY<<"), rangeX="<<pC.getRangeX()<<", rangeY="<<pC.getRangeY();	
	return stream;
}



} // (namespace)
