// SDFSettings.cpp : nastavenia programu
#include "stdafx.h"
#include "SDFSettings.h"

CSettings *Nastavenia = new CSettings();

CSettings::CSettings()
{
	SDF_Mode = SDF_GPU;
	SDF_Rays = 30;
	SDF_Cone = 120.0f;
	SDF_Smoothing_Radius = 2;
	SDF_Revert_Rays = false;
	SDF_Distribution = true;
	OCTREE_Depth = 10;
	OCTREE_Threshold = 4;
	GPU_Work_Groups = 2.0f;
	GPU_Work_Items = 0.25f;

	VISUAL_State = VISUAL_DEFAULT;
	VISUAL_Octree = false;
	VISUAL_Normals = false;
	VISUAL_Axes = false;
	VISUAL_Points = false;
	VISUAL_Smoothed = true;
	VISUAL_Alpha = 255;
	VISUAL_SDF_Type = VISUAL_NORMALIZED_0_MAX;


	INFO_Total_Triangles = 0;
	INFO_Total_Vertices = 0;
	DEBUG_Min_SDF = 0;
	DEBUG_Max_SDF = 0;
	DEBUG_Selected_Triangle = 0;
	DEBUG_Selected_Quality = 0;

	SDF_STATUS = 0;
}