// Assimp.cpp : subor pre nacitanie / ulozenie modelov

#include "stdafx.h"
#include "StringHelper.h"
#include "Assimp.h"

namespace AssimpFileHandler
{
	// the global Assimp scene object
	const aiScene* scene = NULL;

	// Create an instance of the Importer class
	Assimp::Importer importer;

	// inicializuj assimp a jeho premenne
	CAssimp::CAssimp()
	{
		createAILogger();
		logInfo("App started!");
	}

	// naimportuj model do pamete
	bool CAssimp::Import3DFromFile( const std::string& pFile)
	{
		//check if file exists
		std::ifstream fin(pFile.c_str());
		if(!fin.fail())
		{
			fin.close();
		}
		else
		{
			//MessageBox(NULL, ("Couldn't open file: " + pFile).c_str() , L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			logInfo( importer.GetErrorString());
			return false;
		}

		scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);

		// If the import failed, report it
		if( !scene)
		{
			logInfo( importer.GetErrorString());
			return false;
		}

		// Now we can access the file's contents.
		logInfo("Import of scene " + pFile + " succeeded.");

		// We're done. Everything will be cleaned up by the importer destructor
		return true;
	}

	// zapis info log do suboru
	void CAssimp::logInfo(std::string logString)
	{
		//Will add message to File with "info" Tag
		Assimp::DefaultLogger::get()->info(logString.c_str());
	}

	// zapis debug log do suboru
	void CAssimp::logDebug(std::string logString)
	{
		//Will add message to File with "debug" Tag
		Assimp::DefaultLogger::get()->debug(logString.c_str());
	}

	void CAssimp::recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale)	{ }

	// Shutdown
	CAssimp::~CAssimp()
	{
		logInfo("App ended!");
		destroyAILogger();
	}

	// vytvor logger na zapis log vypisov
	void CAssimp::createAILogger()
	{
		//Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
		Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;

		// Create a logger instance for Console Output
		Assimp::DefaultLogger::create("",severity, aiDefaultLogStream_STDOUT);

		// Create a logger instance for File Output (found in project folder or near .exe)
		Assimp::DefaultLogger::create("log.txt",severity, aiDefaultLogStream_FILE);

		// Now I am ready for logging my stuff
		Assimp::DefaultLogger::get()->info("this is my info-call");
	}

	// ukonci logger a tym ulozi subor
	void CAssimp::destroyAILogger()
	{
		// Kill it after the work is done
		Assimp::DefaultLogger::kill();
	}
}