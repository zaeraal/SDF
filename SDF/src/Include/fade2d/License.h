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
#include "Fade_2D.h"
 
/* This is the free license of Fade2D. It is valid for personal non-commercial,
   non-military, scientific research. Technically, the 2D version is unlimited
   (while the 2.5D version is an evaluation version). 
*/


namespace{
struct License
{
	License()
	{
	GEOM_LIC::setLic(
	"[NonCommercialResearch]",
	"[ConstrainedDelaunay,true],[2D,true],[25D,limited],[DelaunayMeshing,true]",
	"",
	"ab8dbb88",
	"90665a3c");
	}
};
License lic;
}



 