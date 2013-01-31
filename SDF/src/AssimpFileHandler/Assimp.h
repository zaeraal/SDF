// Assimp.h : subor pre nacitanie / ulozenie modelov
#pragma once
#include "StringHelper.h"
#include "Face.h"

namespace AssimpFileHandler
{
	using namespace std;
	using namespace Model;

	public class CAssimp
	{
	public:
		CAssimp();
		bool Import3DFromFile( const string& pFile);
		void logInfo(std::string logString);
		void logDebug(std::string logString);
		void LoadData(vector<Face*>& fc, vector<Vertex*>& pts);
		~CAssimp();
	private:
		void RecursiveLoad (const struct aiScene *sc, const struct aiNode* nd);
		vector<Face*> faces;
		vector<Vertex*> points;
	protected:
		void createAILogger();
		void destroyAILogger();
	};
}