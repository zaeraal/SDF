// Assimp.h : subor pre nacitanie / ulozenie modelov
#pragma once
#include "StringHelper.h"
#include "Face.h"

namespace AssimpFileHandler
{
	using namespace std;
	using namespace Model;
	using namespace Controller;

	public class CAssimp
	{
	public:
		CAssimp();
		bool Import3DFromFile( const string& pFile);
		void logInfo(std::string logString);
		void logDebug(std::string logString);
		void LoadData(LinkedList<Face>* fc, LinkedList<Vertex>* pts);
		~CAssimp();
	private:
		void RecursiveLoad (const struct aiScene *sc, const struct aiNode* nd, LinkedList<Face>* fc, LinkedList<Vertex>* pts);
	protected:
		void createAILogger();
		void destroyAILogger();
	};
}