// Assimp.h : subor pre nacitanie / ulozenie modelov
#pragma once

namespace AssimpFileHandler
{
	public class CAssimp
	{
	public:
		CAssimp();
		bool Import3DFromFile( const std::string& pFile);
		void logInfo(std::string logString);
		void logDebug(std::string logString);
		~CAssimp();
	private:
		void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale);
	protected:
		void createAILogger();
		void destroyAILogger();
	};
}