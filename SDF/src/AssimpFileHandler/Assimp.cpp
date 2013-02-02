// Assimp.cpp : subor pre nacitanie / ulozenie modelov

#include "stdafx.h"
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

		scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Fast);

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

	void CAssimp::LoadData(LinkedList<Face>* fc, LinkedList<Vertex>* pts)
	{
		glLoadIdentity();
		/*Vertex* v1 = new Vertex(10.0,20.0,0.0);
		Vertex* v2 = new Vertex(-10.0,20.0,0.0);
		Vertex* v3 = new Vertex(0.0,0.0,0.0);
		Face* fc = new Face(v1, v2, v3);
		points.push_back(v1);
		points.push_back(v2);
		points.push_back(v3);
		faces.push_back(fc);*/
		//faces.clear();
		//points.clear();
		RecursiveLoad(scene, scene->mRootNode, fc, pts);
	}

	void CAssimp::RecursiveLoad (const struct aiScene *sc, const struct aiNode* nd, LinkedList<Face>* fc, LinkedList<Vertex>* pts)
	{
		// mozno prerobit cez Matrix4f class a nie cez Opengl?
		glPushMatrix();
			aiMatrix4x4 matica = nd->mTransformation;
			matica.Transpose();
			glMultMatrixf((float*)&matica);

			GLfloat m[16]; 
			glGetFloatv(GL_MODELVIEW_MATRIX, m);

			// for all meshes assigned to this node
			for (unsigned int n = 0; n < nd->mNumMeshes; ++n)
			{
				const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
				const unsigned int n_faces = mesh->mNumFaces;
				const unsigned int n_vertices = mesh->mNumVertices;

				// tmp struktura z ktorej skopirujem pouzite meshe
				Face** tmp_faces = new Face* [n_faces];
				Vertex** tmp_points = new Vertex* [n_vertices];

				for(unsigned int i = 0; i < n_faces; i++)
					tmp_faces[i] = NULL;

				for(unsigned int i = 0; i < n_vertices; i++)
					tmp_points[i] = NULL;


				for (unsigned int t = 0; t < n_faces; ++t)
				{
					const struct aiFace* face = &mesh->mFaces[t];

					// ignoruj to co neni trojuholnik - nema to normalu a neda sa z toho pocitat SDF
					if(face->mNumIndices != 3)
						continue;

					for(unsigned int i = 0; i < 3; i++)			// go through all vertices in face
					{
						int vertexIndex = face->mIndices[i];	// get index for current vertex
						if(tmp_points[vertexIndex] == NULL)
						{
							// aplikuj maticu transformacie
							double a = mesh->mVertices[vertexIndex].x;
							double b = mesh->mVertices[vertexIndex].y;
							double c = mesh->mVertices[vertexIndex].z;

							double x = a*m[0] + b*m[4] + c*m[8] + m[12];
							double y = a*m[1] + b*m[5] + c*m[9] + m[13];
							double z = a*m[2] + b*m[6] + c*m[10] + m[14];

							tmp_points[vertexIndex] = new Vertex(x, y, z);
						}
					}
					tmp_faces[t] = new Face(tmp_points[face->mIndices[0]], tmp_points[face->mIndices[1]], tmp_points[face->mIndices[2]]);
				}

				for(unsigned int i = 0; i < n_faces; i++)
				{
					LinkedList<Face>* u = new LinkedList<Face>(tmp_faces[i]);
					fc->InsertToEnd(u);
					tmp_faces[i] = NULL;
				}
				delete [] tmp_faces;

				for(unsigned int i = 0; i < n_vertices; i++)
				{
					if(tmp_points[i] != NULL)
					{
						LinkedList<Vertex>* u = new LinkedList<Vertex>(tmp_points[i]);
						pts->InsertToEnd(u);
						tmp_points[i] = NULL;
					}
				}
				delete [] tmp_points;
			}

			// for all children
			for (unsigned int n = 0; n < nd->mNumChildren; ++n)
			{
				RecursiveLoad(sc, nd->mChildren[n], fc, pts);
			}
		glPopMatrix();
	}

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