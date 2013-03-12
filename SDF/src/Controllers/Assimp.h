// Assimp.h : subor pre nacitanie / ulozenie modelov
#pragma once
#include "StringHelper.h"
#include "Face.h"
#include "Mat4.h"

namespace AssimpFileHandler
{
	using namespace std;
	using namespace GenericStructures;
	using namespace MeshStructures;

	public class CAssimp
	{
	public:
		CAssimp();
		bool Import3DFromFile( const string& pFile);
		void logInfo(std::string logString);
		void logDebug(std::string logString);
		void LoadData(LinkedList<Face>* fc, LinkedList<Vertex>* pts);
		void SetScene(aiScene* scena);
		~CAssimp();
	private:
		void RecursiveLoad (const struct aiScene *sc, const struct aiNode* nd, LinkedList<Face>* fc, LinkedList<Vertex>* pts);
	protected:
		void createAILogger();
		void destroyAILogger();
	};
}