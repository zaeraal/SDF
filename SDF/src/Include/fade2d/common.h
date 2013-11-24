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

#include <string>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <iterator>

// DLL IMPORT/EXPORT MACRO
#if defined (_WIN32)
	#if defined(FADE2D_EXPORT)
		#define  CLASS_DECLSPEC __declspec(dllexport) 
	#else
		#define  CLASS_DECLSPEC __declspec(dllimport) 
	#endif 
#else 
	#define CLASS_DECLSPEC
#endif

#define GEOM_TRUE 100
#define GEOM_FALSE 99

// GEOM_PSEUDO3D is defined by the library project when the lib is build.
// In contrast, under Linux we specify it manually.
#ifndef GEOM_PSEUDO3D
//#define GEOM_PSEUDO3D GEOM_TRUE
#define GEOM_PSEUDO3D GEOM_FALSE
#endif


//#define GEOM_BENCHCOUNTING
#ifdef GEOM_BENCHCOUNTING
		#define BC(x) benchCounter(x);
#else
		#define BC(x)
#endif

#if GEOM_PSEUDO3D==GEOM_TRUE
	namespace GEOM_FADE25D {
#elif GEOM_PSEUDO3D==GEOM_FALSE
	namespace GEOM_FADE2D {
#else
	#error GEOM_PSEUDO3D is not defined 
#endif 
	

enum ZoneLocation
{
	ZL_GLOBAL,
	ZL_INSIDE,
	ZL_OUTSIDE,
	ZL_GROW,
	ZL_RESULT_UNION,
	ZL_RESULT_INTERSECTION,
	ZL_RESULT_DIFFERENCE,
	ZL_RESULT_SYM_DIFFERENCE
};

template<class T_PTR> struct func_ltDerefPtr
{
	bool operator()(T_PTR p0,T_PTR p1) const {return *p0<*p1;}
};



} // (namespace)

