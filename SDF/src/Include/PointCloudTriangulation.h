#pragma once

using namespace System;

#include <vector>
#include <set>

#include "PCT_structures.h"
#include "l_eig3.h"
#include <fade2d/Fade_2D.h>
#include <fade2d/Point2.h>

namespace PointCloudTriangulation {
	struct DeleunayTriangulator {
	public:
		DeleunayTriangulator();
		void setKNeighParams(float _ratio, int _min, int _max);
		void setCenterFactorParams(float _limit, float _offset, float _scale);

		// returns local deleunay triangulation, where normal is computed using PCA but normal orientation is unknown
		void computeLocalTriangulationFromPoints(int index, int numOfPoints, float * points, int &numOfIndices, int ** indices, std::vector<std::set<int>> globalNeighbourhoods, float * nor, bool visualization = false);
		// returns global triangulation composed of local deleunay triangulations. normals are computed using PCA and orientation choice is made like this:
		// most side vertex's normal has to direct out of aabb and orientations of neighbouring vertices are choosen using per triangle connectivity... 
		void computeGlobalTriangulationFromPoints(int numOfPoints, float * points, int &numOfIndices, int ** indices, float ** normals, bool visualization = false);
		std::vector<std::set<int>> computeGlobalNeighbourhood(PCTMeshGraph * pMesh);
		int getKNeigh(PCTMeshGraph * pMesh);
		PCTNeighVisualization * neighVis;

		int neighVisSize;
		int neighPCIndex;
	private:
		float kNeigh_ratio; // ratio of numOfPoints that is taken as one ring neighbourhood
		int kNeigh_min; // min vertices in neighbourhood
		int kNeigh_max; // max vertices in neighbourhood

		float centerFactor_limit;  // center factor cant go under this value, because when it is near 0, it makes long distances too short
		float centerFactor_function_offset; // to move values from intrrval <-1, 1> to <0, 1>
		float centerFactor_function_scale;

		int maxAddedPoints_MoreThanAngleTresh;

		Array2D<bool> computeLocalTriangulation(int i, PCTMeshGraph * pMesh, std::vector<std::set<int>> globalNeighbourhoods, float * nor, bool visualization = false);
		void findClosestNeighWithCentering(int i, PCTMeshGraph * pMesh, std::set<int> &neighs, PCTCVector3 &cm, int * distances);
		bool checkLocalNeighAngles(int i, PCTMeshGraph * pMesh, std::set<int> &neighs);
		void getTangentPlanePCA(int numOfPoints, PCTCVector3 * points, PCTCVector3 * n, PCTCVector3 * ev_1, PCTCVector3 * ev_2, PCTCVector3 * ev_3);
		PCTCVector2 Space2Plane(PCTCVector3 q, PCTCVector3 o, PCTCVector3 e1, PCTCVector3 e2);
		PCTCVector3 Plane2Space(PCTCVector2 q, PCTCVector3 o,PCTCVector3 e1, PCTCVector3 e2);
		PCTCVector3 ProjectPointIntoPlane(PCTCVector3 q, PCTCVector3 o, PCTCVector3 n);
	};
}
