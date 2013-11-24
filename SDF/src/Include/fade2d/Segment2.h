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


//#include "TrPoint.h"
/** \brief Segment
*
*/
class Segment2
{
protected:
	Point2 src,trg;
public:
/** Create a Segment2
*
* @param src First endpoint (source)
* @param trg Second endpoint (target)
*/
	CLASS_DECLSPEC
	Segment2(Point2 src,Point2 trg);
/** Get the source point
*
* @return the source point
*/
	CLASS_DECLSPEC
	Point2 getSrc() const;
/** Get the target point
*
* @return the target point
*/
	CLASS_DECLSPEC
	Point2 getTrg() const;

	CLASS_DECLSPEC
	void swapSrcTrg();

	CLASS_DECLSPEC
	friend std::ostream &operator<<(std::ostream &stream, Segment2 seg);
	
	
};


} // (namespace)
