// SDFSettings.h : nastavenia programu
#pragma once

public enum SDF_MODE{
    SDF_CPU = 0,
    SDF_GPU,
    SDF_GPU_DEBUG
};

public enum VISUAL_STATE{
	VISUAL_NONE = 0,
    VISUAL_DEFAULT,
    VISUAL_WIREFRAME,
    VISUAL_SDF,
	VISUAL_PICKING
};

public enum VISUAL_SDF_TYPE{
    VISUAL_NORMALIZED_0_1 = 0,
    VISUAL_NORMALIZED_MIN_1,
	VISUAL_NORMALIZED_0_MAX,
	VISUAL_NORMALIZED_MIN_MAX
};

public class CSettings
{
public:
	CSettings();

	// input
	SDF_MODE			SDF_Mode;				// ci pocitam na CPU / GPU
	unsigned int		SDF_Rays;				// pocet lucov
	float				SDF_Cone;				// velkost conu
	unsigned int		SDF_Smoothing_Radius;	// velkost conu
	bool				SDF_Smooth_Projected;	// ci vyhladit mesh, alebo projektnute body
	bool				SDF_Revert_Rays;		// ci aj obratene luce								x
	bool				SDF_Distribution;		// random (false) / uniform (true) distribucia
	unsigned int		OCTREE_Depth;			// hlbka octree
	unsigned int		OCTREE_Threshold;		// min trianglov na octree
	float				GPU_Work_Groups;		// nasobok jadier									- povolit null ?
	float				GPU_Work_Items;			// nasobok max work itemov							- povolit null
	VISUAL_STATE		VISUAL_State;			// co vykreslujem
	bool				VISUAL_Octree;			// ci kreslim octree
	bool				VISUAL_Normals;			// ci kreslim normaly
	bool				VISUAL_Axes;			// ci kreslim axe
	bool				VISUAL_Points;			// ci kreslim projektnute body						x
	bool				VISUAL_Smoothed;		// ci kreslim sdf na facoch alebo vertexoch
	unsigned int		VISUAL_Alpha;			// alpha vykreslenych hodnot
	VISUAL_SDF_TYPE		VISUAL_SDF_Type;		// ktore normalizovanie sdf kreslim


	// output
	unsigned int		INFO_Total_Triangles;	// vypis kolko mam trojuholnikov
	unsigned int		INFO_Total_Vertices;	// vypis kolko mam vertexov
	float				DEBUG_Min_SDF;			// min hodnota sdf
	float				DEBUG_Max_SDF;			// max hodnota sdf
	unsigned int DEBUG_Selected_Triangle;		// oznaceny trojuholnik
	unsigned int DEBUG_Selected_Quality;		// hodnota oznaceneho trojuholnika

	unsigned int		SDF_STATUS;				// percenta na kolko je sdf spocitane				x

};

extern CSettings *Nastavenia;
