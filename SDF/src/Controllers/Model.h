// Model.h : subor pre kontrolu modelov
#pragma once
#include "Octree.h"
#include "SDFController.h"
#include "Assimp.h"
#include "MathHelper.h"

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
		void LoadAssimp(aiScene* scene);
		//float GetSDF(const struct aiFace* face, bool smoothed);
		float* GetSDF(int& size, bool smoothed);
		void ResetSettings();
		void AssignNumber();
		void ComputeBoundary();
		void CreateOctree();
		void SetColors();
		void ComputeSusedov();
		void GetBoundary(float &siz, float &x, float &y, float &z);
		void DrawModel();
		void ColorToRGB(int color, GLubyte &R, GLubyte &G, GLubyte &B);
		void HLSToRGB(float SDF_value, GLubyte &R, GLubyte &G, GLubyte &B);
		void setDrawMode(int mode);
		int getDrawMode();
		void ProcessPick(int x, int y);
		void ComputeSDF();
		void TriangulatePoints();
		void ReloadOctreeData();

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

		// rozmery modelu
		Vector4		b_stred;				// center of minN - maxN
		float		b_size;					// size for octree
		float		b_sf;					// scale factor
		float		b_max;					// diagonala v octree
	};
}