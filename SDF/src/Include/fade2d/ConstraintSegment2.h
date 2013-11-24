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
#include <set>

#include "common.h"
#if GEOM_PSEUDO3D==GEOM_TRUE 
	namespace GEOM_FADE25D {
#elif GEOM_PSEUDO3D==GEOM_FALSE
	namespace GEOM_FADE2D {
#else
	#error GEOM_PSEUDO3D is not defined 
#endif 

class Point2; // FWD
class ConstraintGraph2; // FWD


enum ConstraintInsertionStrategy
{
	CIS_KEEP_DELAUNAY,
	CIS_IGNORE_DELAUNAY
};


class ConstraintSegment2
{
public:
	ConstraintSegment2(Point2* p0_,Point2* p1_,ConstraintInsertionStrategy cis_);
	~ConstraintSegment2();
	CLASS_DECLSPEC
	Point2* getSrc() const;
	CLASS_DECLSPEC
	Point2* getTrg() const;
	CLASS_DECLSPEC
	bool isAlive() const;
	ConstraintInsertionStrategy getCIS() const;
	CLASS_DECLSPEC
	bool operator<(const ConstraintSegment2& pOther) const;
	void split(Point2* pSplit);
	void addOwner(ConstraintGraph2* pOwner);
	void removeOwner(ConstraintGraph2* pOwner);
	CLASS_DECLSPEC
	void getChildrenRec(std::vector<ConstraintSegment2*>& vChildConstraintSegments);
	CLASS_DECLSPEC
	void getChildrenAndSplitPoint(ConstraintSegment2*& pCSeg0,ConstraintSegment2*& pCSeg1,Point2*& pSplitPoint);
protected:
	Point2 *p0,*p1;
	ConstraintInsertionStrategy cis;
	std::set<ConstraintGraph2*> sOwners;
	std::vector<ConstraintSegment2*> vChildren;		
};

} // NAMESPACE
