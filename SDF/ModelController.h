// ModelController.h : subor pre kontrolu modelov
#pragma once
#include "Octree.h"
#include "Assimp.h"

namespace Controller
{
	using namespace std;
	using namespace Model;
	using namespace AssimpFileHandler;

	public class ModelController
	{
	public:
		ModelController();
		~ModelController();

		void logInfo(std::string logString);
		void logDebug(std::string logString);

		void LoadFile(std::string Filename);
		void CreateOctree();
		void SetColors();
		void GetBoundary(double &siz, double &x, double &y, double &z);
		void DrawModel();

		bool loaded;

	private:
		CAssimp* Assimp;
		vector<Face*> triangles;
		vector<Vertex*> points;
		Octree* m_root;
	};
}