// Model.h : subor pre kontrolu modelov
#pragma once
#include "Octree.h"
#include "SDFController.h"
#include "Assimp.h"

namespace ModelController
{
	using namespace std;
	using namespace AssimpFileHandler;
	using namespace SDFController;

	const int color_step = 4;

	public class CModel
	{
	public:
		CModel();
		~CModel();

		void logInfo(string logString);
		void logDebug(string logString);

		void LoadFile(string Filename);
		void ResetSettings();
		void CreateOctree();
		void SetColors();
		void GetBoundary(double &siz, double &x, double &y, double &z);
		void DrawModel();
		void ColorToRGB(int color, GLubyte &R, GLubyte &G, GLubyte &B);
		void HLSToRGB(double SDF_value, GLubyte &R, GLubyte &G, GLubyte &B);
		void setDrawMode(int mode);
		int getDrawMode();
		int GetTriangleCount();
		void ProcessPick(int x, int y);
		void ComputeSDF();

		bool loaded;
		bool show_octree;
		bool show_normals;
		Face* selected;

	private:
		CAssimp* Assimp;
		CSDFController* SDF_control;
		LinkedList<Face>* triangles;
		LinkedList<Vertex>* points;
		Octree* m_root;
		int draw_mode;						// 0 picking (default), 1 selected triangle, 2 SDF, 3 wireframe
		
	};
}