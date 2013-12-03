// This is the main DLL file.

#include "PointCloudTriangulation.h"

using namespace PointCloudTriangulation;

DeleunayTriangulator::DeleunayTriangulator(){
	kNeigh_ratio = 0.02;
	kNeigh_min = 8;
	kNeigh_max = 12;

	centerFactor_limit = 0.2;
	centerFactor_function_offset = 0.5;
	centerFactor_function_scale = 2.0;

	maxAddedPoints_MoreThanAngleTresh = 10;

	neighVis = new PCTNeighVisualization[0];
	neighVisSize = 0;
}

void DeleunayTriangulator::setKNeighParams(float _ratio, int _min, int _max){
	kNeigh_ratio = _ratio;
	kNeigh_min = _min;
	kNeigh_max = _max;
}

void DeleunayTriangulator::setCenterFactorParams(float _limit, float _offset, float _scale){
	centerFactor_limit = _limit;
	centerFactor_function_offset = _offset;
	centerFactor_function_scale = _scale;
}

void DeleunayTriangulator::computeLocalTriangulationFromPoints(int index, int numOfPoints, float * points, int &numOfIndices, int ** indices, std::vector<std::set<int>> globalNeighbourhoods, float * nor, bool visualization){
	PCTMeshGraph * pMesh = new PCTMeshGraph();
	pMesh->numOfVertices = numOfPoints;
	pMesh->pVerts = new PCTCVector3[numOfPoints];
	for (int i=0; i<numOfPoints; i++){
		pMesh->pVerts[i].x = points[i * 3];
		pMesh->pVerts[i].y = points[i * 3 + 1];
		pMesh->pVerts[i].z = points[i * 3 + 2];
	}

	Array2D<bool> E_local = computeLocalTriangulation(index, pMesh, globalNeighbourhoods, nor, visualization);
	
	std::vector<int> v_indices;
	for (int i=0; i<pMesh->numOfVertices; i++){
		for (int j=i; j<pMesh->numOfVertices; j++){
			if (i !=j && E_local[i][j]){
				for (int k=0; k<pMesh->numOfVertices; k++){
					if (i !=k && E_local[i][k] && j !=k && E_local[j][k]){
						bool addTriangle = true;

						for (int v=0; v<v_indices.size() / 3; v++){
							int v1 = v_indices[v * 3];
							int v2 = v_indices[v * 3 + 1];
							int v3 = v_indices[v * 3 + 2];
							if ((i == v1 && j == v2 && k == v3) || (i == v2 && j == v1 && k == v3) || (i == v3 && j == v1 && k == v2) || 
								(i == v1 && j == v3 && k == v2) || (i == v2 && j == v3 && k == v1) || (i == v3 && j == v2 && k == v1)){
								addTriangle = false;
								break;
							}
						}

						if (addTriangle){
							v_indices.push_back(i);
							v_indices.push_back(j);
							v_indices.push_back(k);
						}
					}
				}
			}
		}
	}

	*indices = new int[v_indices.size()];
	for (int i=0; i<v_indices.size(); i++){
		(*indices)[i] = v_indices[i];
	}

	numOfIndices = v_indices.size() / 3;
}

void DeleunayTriangulator::computeGlobalTriangulationFromPoints(int numOfPoints, float * points, int &numOfIndices, int ** indices, float ** normals, bool visualization){
	PCTMeshGraph * pMesh = new PCTMeshGraph();
	pMesh->numOfVertices = numOfPoints;
	pMesh->pVerts = new PCTCVector3[numOfPoints];
	for (int i=0; i<numOfPoints; i++){
		pMesh->pVerts[i].x = points[i * 3];
		pMesh->pVerts[i].y = points[i * 3 + 1];
		pMesh->pVerts[i].z = points[i * 3 + 2];
	}

	if (visualization){
		neighVis = new PCTNeighVisualization[pMesh->numOfVertices];
		for (int i=0; i<pMesh->numOfVertices; i++){
			neighVis[i].isE_local_visualize = false;
		}

		neighVisSize = pMesh->numOfVertices;
	}

	*normals = new float[3 * pMesh->numOfVertices];
	float * nor = new float[3];

	std::vector<std::set<int>> globalNeighbourhoods = computeGlobalNeighbourhood(pMesh);
	Array2D<bool> E_global = Array2D<bool>(numOfPoints, numOfPoints, false);

	for (int i=0; i<pMesh->numOfVertices; i++){
		Array2D<bool> E_local = computeLocalTriangulation(i, pMesh, globalNeighbourhoods, nor, visualization);

		(*normals)[i * 3] = nor[0];
		(*normals)[i * 3 + 1] = nor[1];
		(*normals)[i * 3 + 2] = nor[2];

		for (int j=0; j<pMesh->numOfVertices; j++){
			if (E_local[i][j] == true){
				E_global[i][j] = true;
				E_global[j][i] = true;
			}
		}
	}

	std::vector<int> v_indices;
	for (int i=0; i<pMesh->numOfVertices; i++){
		for (int j=i; j<pMesh->numOfVertices; j++){
			if (i !=j && E_global[i][j]){
				for (int k=0; k<pMesh->numOfVertices; k++){
					if (i !=k && E_global[i][k] && j !=k && E_global[j][k]){
						bool addTriangle = true;

						for (int v=0; v<v_indices.size() / 3; v++){
							int v1 = v_indices[v * 3];
							int v2 = v_indices[v * 3 + 1];
							int v3 = v_indices[v * 3 + 2];
							if ((i == v1 && j == v2 && k == v3) || (i == v2 && j == v1 && k == v3) || (i == v3 && j == v1 && k == v2) || 
								(i == v1 && j == v3 && k == v2) || (i == v2 && j == v3 && k == v1) || (i == v3 && j == v2 && k == v1)){
									addTriangle = false;
									break;
							}
						}

						if (addTriangle){
							v_indices.push_back(i);
							v_indices.push_back(j);
							v_indices.push_back(k);
						}
					}
				}
			}
		}
	}

	// chose which orientation of normals is correct
    /////////////////////////////////////////////////////////////

    // find most top vertex and orient unit normals that it is pointing out of aabb

    float maxtop = FLT_MIN;
    float mintop = FLT_MAX;

    int maxidx = -1;
    int minidx = -1;

    for (int i=0; i<pMesh->numOfVertices; i++){

        if (pMesh->pVerts[i].y > maxtop){
            maxtop = pMesh->pVerts[i].y;
            maxidx = i;
        }

        if (pMesh->pVerts[i].y < mintop){
            mintop = pMesh->pVerts[i].y;
            minidx = i;
        }
    }

    PCTCVector3 aabb_axis = pMesh->pVerts[maxidx] - pMesh->pVerts[minidx];
    PCTCVector3 maxnorm = PCTCVector3((*normals)[maxidx * 3], (*normals)[maxidx * 3 + 1], (*normals)[maxidx * 3 + 2]);

    if (Dot(aabb_axis, pMesh->pVerts[maxidx] + maxnorm) > 0){
        // keep normal, it is oriented fine
    } else {
        // swap normal orientation
        maxnorm = maxnorm * -1;
    }

    (*normals)[maxidx * 3] = maxnorm.x;
    (*normals)[maxidx * 3 + 1] = maxnorm.y;
    (*normals)[maxidx * 3 + 2] = maxnorm.z;

    // distribute normal orientation over surafece

    int verticesChanged = 1;

    bool * orientedVertices = new bool[pMesh->numOfVertices];
    for (int i=0; i<pMesh->numOfVertices; i++){
        orientedVertices[i] = false;
    }
    orientedVertices[maxidx] = true;

    while (verticesChanged < pMesh->numOfVertices){
        // iterate all vertices and change if neighbour is oriented
        for (int i=0; i<pMesh->numOfVertices; i++){
            // itareta neighbours
            for (int j=0; j<pMesh->numOfVertices; j++){
                if (E_global[i][j] && !orientedVertices[i] && orientedVertices[j]){

                    PCTCVector3 normal_i((*normals)[i * 3], (*normals)[i * 3 + 1], (*normals)[i * 3 + 2]);
                    PCTCVector3 normal_j((*normals)[j * 3], (*normals)[j * 3 + 1], (*normals)[j * 3 + 2]);

                    if (Dot(normal_i, normal_j) > 0){
                        // keep normal, it is oriented fine
                    } else {
                        // swap normal orientation
                        (*normals)[i * 3] = -(*normals)[i * 3];
                        (*normals)[i * 3 + 1] = -(*normals)[i * 3 + 1];
                        (*normals)[i * 3 + 2] = -(*normals)[i * 3 + 2];
                    }

                    orientedVertices[i] = true;
                    verticesChanged++;

                    break;
                }
            }
        }
    }


    delete[] orientedVertices;


    for (int i=0; i<pMesh->numOfVertices; i++){
        (*normals)[i * 3] = -(*normals)[i * 3];
        (*normals)[i * 3 + 1] = -(*normals)[i * 3 + 1];
        (*normals)[i * 3 + 2] = -(*normals)[i * 3 + 2];

		if (visualization){
			neighVis[i].visNormals[0] = PCTCVector3((*normals)[i * 3], (*normals)[i * 3 + 1], (*normals)[i * 3 + 2]);
		}
    }

 	*indices = new int[v_indices.size()];
	for (int i=0; i<v_indices.size(); i++){
		(*indices)[i] = v_indices[i];
	}

	numOfIndices = v_indices.size() / 3;

}

void DeleunayTriangulator::getTangentPlanePCA(int numOfPoints, PCTCVector3 * points, PCTCVector3 * n, PCTCVector3 * ev_1, PCTCVector3 * ev_2, PCTCVector3 * ev_3){

	// get center of points
	PCTCVector3 center(0,0,0);
	for (int i=0; i < numOfPoints; i++){
		center = center + points[i];
	}
	center = center / (float)numOfPoints;

	// create matrix C1 and C2
	Array2D<double> C1(3, numOfPoints, 0.0);
	for (int i=0; i < numOfPoints; i++){
		C1[0][i] = points[i].x - center.x;
		C1[1][i] = points[i].y - center.y;
		C1[2][i] = points[i].z - center.z;
	}

	Array2D<double> C2(numOfPoints, 3, 0.0);
	for (int i=0; i < numOfPoints; i++){
		C2[i][0] = points[i].x - center.x;
		C2[i][1] = points[i].y - center.y;
		C2[i][2] = points[i].z - center.z;
	}

	// compute covariance matrix C as C1 * C2
	Array2D<double> C = C1 * C2;

	/* Symmetric matrix A => eigenvectors in columns of V, corresponding
   eigenvalues in d. A is 3x3 symmetric matrix */
	double A[3][3];
	double V[3][3];
	double d[3];

	//log(0, "matica C: ");
	//log(0, C);

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			A[i][j] = C[i][j] / (double)(numOfPoints);

	eigen_decomposition(A, V, d);

	PCTCVector3 ev0 = PCTCVector3((float)V[0][0], (float)V[1][0], (float)V[2][0]);
	PCTCVector3 ev1 = PCTCVector3((float)V[0][1], (float)V[1][1], (float)V[2][1]);
	PCTCVector3 ev2 = PCTCVector3((float)V[0][2], (float)V[1][2], (float)V[2][2]);

	if (d[0] > d[1]){
		if (d[1] > d[2]){
			*ev_1 = ev0;
			*ev_2 = ev1;
			*ev_3 = ev2;
		} else {
			*ev_1 = ev0;
			*ev_2 = ev2;
			*ev_3 = ev1;
		}
	} else {
		if (d[0] > d[2]){
			*ev_1 = ev1;
			*ev_2 = ev0;
			*ev_3 = ev2;
		} else {
			*ev_1 = ev1;
			*ev_2 = ev2;
			*ev_3 = ev0;
		}
	}

	*ev_1 = Normalize(*ev_1);
	*ev_2 = Normalize(*ev_2);
	*ev_3 = Normalize(*ev_3);

	*n = Normalize(Cross(*ev_1, *ev_2));
}

Array2D<bool> DeleunayTriangulator::computeLocalTriangulation(int i, PCTMeshGraph * pMesh, std::vector<std::set<int>> globalNeighbourhoods, float * nor, bool visualization){
		/////////////// params /////////////
		bool doProjectionToLocalPlane = false;
		int k = 0;
		//float radiusNeigh = (pModel->modelbb.x_max -pModel->modelbb.x_min + pModel->modelbb.y_max + pModel->modelbb.y_min + pModel->modelbb.z_max - pModel->modelbb.z_min) / 10.0;

		/////////////// neighbourhood ///////////////

		std::set<int> neighs = globalNeighbourhoods[i];

		/*PCTCVector3 normalAva = PCTCVector3(0,0,0);
		for (std::set<int>::iterator it1=neighs.begin(); it1!=neighs.end(); ++it1){
			for (std::set<int>::iterator it2=neighs.begin(); it2!=neighs.end(); ++it2){
				PCTCVector3 v1 = pMesh->pVerts[i] - pMesh->pVerts[*it1];
				PCTCVector3 v2 = pMesh->pVerts[i] - pMesh->pVerts[*it2];
				PCTCVector3 nor = Cross(v1, v2);

				normalAva = normalAva + nor;
			}
		}

		normalAva = normalAva / (float)neighs.size();*/
		

		// vyratame normalu tak, ze spravime std::vector (P - Pi) medzi vrcholom a kazdym vrcholom okolia
		// a potom cross medzi nimi a z toho priemerny vektor...

	
		//The projection of a point q = (x, y, z) onto a plane given by a point p = (a, b, c) and a normal n = (d, e, f) is
		//q_proj = q - dot(q - p, n) * n

		PCTCVector2 * pointsInPlane = new PCTCVector2[neighs.size()];
		PCTCVector3 * neighPoints = new PCTCVector3[neighs.size()];

		/*cloud_neighs_projected->width    = neighs.size();
		cloud_neighs_projected->height   = 1;
		cloud_neighs_projected->is_dense = false;
		cloud_neighs_projected->points.resize (cloud_neighs_projected->width * cloud_neighs_projected->height);*/

		// insert the point with index i - origin into neighs !!!
		neighs.insert(i);

		PCTCVector3 p = pMesh->pVerts[i]; // origin
		PCTCVector3 n;
		PCTCVector3 ev1;
		PCTCVector3 ev2;
		PCTCVector3 ev3;

		k = 0;
		for (std::set<int>::iterator it=neighs.begin(); it!=neighs.end(); ++it){
			neighPoints[k] = pMesh->pVerts[*it];
			k++;
		}

		/////////////// PCA ///////////////

		getTangentPlanePCA(neighs.size(), neighPoints, &n, &ev1, &ev2, &ev3);
		//getTangentPlanePCAWithSVD(neighs.size(), neighPoints, &n, &ev1, &ev2, &ev3);
		//getTangentPlanePCAUsingPCL(neighs.size(), neighPoints, &n, &ev1, &ev2, &ev3);


		/*PCTCVector3 p2 = p;
		k = 0
		while (Distance(p2, p) < 0.00001){
			p2 = pMesh->pVerts[k];
			k++;
		}

		PCTCVector3 p2_proj = ProjectPointIntoPlane(p2, p, n);*/

		//log(0, "normala tang. plochy: ", n);

		/////////////// PROJECTION INTO TANGENT PLANE ///////////////

		k=0;
		for (std::set<int>::iterator it=neighs.begin(); it!=neighs.end(); ++it)
		{
			PCTCVector3 q(pMesh->pVerts[*it].x, pMesh->pVerts[*it].y, pMesh->pVerts[*it].z); // ten co chcem projektovat

			PCTCVector3 q_proj = ProjectPointIntoPlane(q, p, n);

			PCTCVector2 q_projPlane = Space2Plane(q_proj, p, ev1, ev2);
			pointsInPlane[k] = q_projPlane;
	
			k++;
		}

		// !!! ratat Laplacian v 3D, triangulacia sa spravi v 2D ale potom sa vrcholy adekvatne spoja v 3D
		// deleunay triangulation in a plane using projected point cloud of neighs

		/////////////// DELEUNAY IN 2D ///////////////

		using namespace GEOM_FADE2D;

		int numPoints = neighs.size();
		std::vector<Point2> vInputPoints;

		Fade_2D dt(numPoints);

		k = 0;
		  for (std::set<int>::iterator it=neighs.begin(); it!=neighs.end(); ++it){
			  Point2 point;
			  point.change((double)pointsInPlane[k].x, (double)pointsInPlane[k].y);
			  vInputPoints.push_back(point);
			  k++;
		  }
		  
	  dt.insert(vInputPoints);
	  
	  std::vector<Triangle2*> vAllDelaunayTriangles;
	  dt.getTrianglePointers(vAllDelaunayTriangles);


		 // create one ring area from neighIndex of processed vertex

		Array2D<bool> E_local = Array2D<bool>(pMesh->numOfVertices, pMesh->numOfVertices, false);

		for(std::vector<Triangle2*>::iterator it=vAllDelaunayTriangles.begin();it!=vAllDelaunayTriangles.end();++it){
		   Triangle2* t(*it);
		   Point2* p0=t->getCorner(0);
		   Point2* p1=t->getCorner(1);
		   Point2* p2=t->getCorner(2);

		   int i0 = -1;
		   int i1 = -1;
		   int i2 = -1;

		   k = 0;
		   for (std::set<int>::iterator itt=neighs.begin(); itt!=neighs.end(); ++itt){
				if (Distance(pointsInPlane[k], PCTCVector2(p0->x(),p0->y())) < 0.00001){
					i0 = *itt;
				}
				if (Distance(pointsInPlane[k], PCTCVector2(p1->x(),p1->y())) < 0.00001){
					i1 = *itt;
				}
				if (Distance(pointsInPlane[k], PCTCVector2(p2->x(),p2->y())) < 0.00001){
					i2 = *itt;
				}
				k++;
		   }

		   if ((i0 != -1) && (i1 != -1) && (i2 != -1)){
			
			  E_local[i0][i1] = true;
			  E_local[i0][i2] = true;
			  E_local[i1][i0] = true;
			  E_local[i1][i2] = true;
			  E_local[i2][i0] = true;
			  E_local[i2][i1] = true;
		   }
			
		}

		if (visualization){

			for (int x=0; x<pMesh->numOfVertices; x++)
				for (int y=x; y<pMesh->numOfVertices; y++)
					if (E_local[x][y]){
						neighVis[i].edges.push_back(x);
						neighVis[i].edges.push_back(y);
					}


			neighVis[i].visNormals[0] = n;
			neighVis[i].visNormals[1] = ev1;
			neighVis[i].visNormals[2] = ev2;
			neighVis[i].visNormals[3] = ev3;

			neighVis[i].localNeighs = neighs;
			//neighVis[i].pointsInTangentPlane = pointsInPlane;
			neighVis[i].isE_local_visualize = true;

		}

		//delete[] pointsInPlane;
		//delete[] neighPoints;

		nor[0] = n.x;
		nor[1] = n.y;
		nor[2] = n.z;

		return E_local;
}

PCTCVector3 DeleunayTriangulator::ProjectPointIntoPlane(PCTCVector3 q, PCTCVector3 o, PCTCVector3 n){
	PCTCVector3 q_proj = q - n * Dot(q - o, n); // ten dot je distance
	return q_proj;
}

PCTCVector2 DeleunayTriangulator::Space2Plane(PCTCVector3 q, PCTCVector3 o, PCTCVector3 e1, PCTCVector3 e2){

	Array2D< float > M = Array2D< float >(2, 3, 0.0f);
	M[0][0] = e1.x; M[1][0] = e2.x; 
	M[0][1] = e1.y; M[1][1] = e2.y;
	M[0][2] = e1.z; M[1][2] = e2.z;

	Array2D< float > S = Array2D< float >(3, 1, 0.0f);
	S[0][0] = q.x - o.x; S[0][1] = q.y - o.y; S[0][2] = q.z - o.z;

	Array2D< float > P = Array2D< float >(1, 2, 0.0f);

	P = M * S;

	PCTCVector2 pp;
	pp.x = P[0][0];
	pp.y = P[0][1];

	return pp;
}

PCTCVector3 DeleunayTriangulator::Plane2Space(PCTCVector2 q, PCTCVector3 o, PCTCVector3 e1, PCTCVector3 e2){

	Array2D< float > M = Array2D< float >(2, 3, 0.0f);
	M[0][0] = e1.x; M[1][0] = e2.x; 
	M[0][1] = e1.y; M[1][1] = e2.y;
	M[0][2] = e1.z; M[1][2] = e2.z;

	Array2D< float > S = Array2D< float >(3, 1, 0.0f);

	Array2D< float > P = Array2D< float >(1, 2, 0.0f);
	P[0][0] = q.x; P[0][1] = q.y;

	S = P * M;

	PCTCVector3 ss;
	ss.x = S[0][0];
	ss.y = S[0][1];
	ss.z = S[0][2];

	return o + ss;
}

// return true if we have to add next neigh vertex
bool DeleunayTriangulator::checkLocalNeighAngles(int i, PCTMeshGraph * pMesh, std::set<int> &neighs){
	std::vector<int> vecNeighs;
	for (std::set<int>::iterator it=neighs.begin(); it!=neighs.end(); ++it){
		vecNeighs.push_back(*it);
	}
	// angles in one ring area from vertex i to others
	std::vector<float> angles = std::vector<float>(neighs.size());
	//vector<int> indices_index2order = std::vector<int>(neighs.size());
	std::vector<int> indices_order2index = std::vector<int>(neighs.size());
	int swapIndex;
	for (int j=0; j < neighs.size(); j++){
		float angle = OrientedAngleBetweenVectors(pMesh->pVerts[i] - pMesh->pVerts[vecNeighs[0]], pMesh->pVerts[i] - pMesh->pVerts[vecNeighs[j]]);
		angles[j] = angle;
		indices_order2index[j] = j;
	}

	// sort angles, get indices
	for (int j=0; j<neighs.size(); j++){
		float minAngle = FLT_MAX;
		for (int k=j; k<neighs.size(); k++){
			if (angles[k] < minAngle){
				minAngle = angles[k];
				swapIndex = k;
			}
		}
		float tmp = angles[j];
		angles[j] = angles[swapIndex];
		angles[swapIndex] = tmp;
		int tmp2 = indices_order2index[j];
		indices_order2index[j] = indices_order2index[swapIndex];
		indices_order2index[swapIndex] = tmp2;
	}

	// check if there is angle of 2 consequent std::vectors > 90
	for (int j=0; j<neighs.size(); j++){
		int idx = indices_order2index[j];
		int nextIdx = indices_order2index[(j + 1) % neighs.size()];
		//PCTCVector3 vec0 = pMesh->pVerts[i] - pMesh->pVerts[vecNeighs[0]];
		PCTCVector3 vec1 = pMesh->pVerts[i] - pMesh->pVerts[vecNeighs[idx]];
		PCTCVector3 vec2 = pMesh->pVerts[i] - pMesh->pVerts[vecNeighs[nextIdx]];
		//float angle01 = AngleBetweenVectors(vec0, vec1);
		//float angle02 = AngleBetweenVectors(vec0, vec2);
		float angle = OrientedAngleBetweenVectors(vec1, vec2);

		angles[j] = angle;
		if (angles[j] > PI / 2)
			return true;
	}

	return false;
}

std::vector<std::set<int>> DeleunayTriangulator::computeGlobalNeighbourhood(PCTMeshGraph * pMesh){

	std::vector<std::set<int>> globalNeighbourhoods;

	int kneigh = getKNeigh(pMesh);

	for (int i=0; i < pMesh->numOfVertices; i++){

		std::set<int> neighs;
		int * distances = new int[pMesh->numOfVertices];
		for (int j=0; j<pMesh->numOfVertices; j++){
			distances[j] = -1;
		}

		int neighIndex = -1;

		for (int j=0; j<pMesh->numOfVertices; j++){
			distances[j] = Distance(pMesh->pVerts[i], pMesh->pVerts[j]);
		}

		PCTCVector3 cm = pMesh->pVerts[i];
		// find kneigh nearest neighbours
		for (int k=0; k < kneigh; k++){
			findClosestNeighWithCentering(i, pMesh, neighs, cm, distances);
		}

		// if angles between 2 consequent neighs is more than 90degree, maximum add 5
		int numAngleLimit = 0;

		while (checkLocalNeighAngles(i, pMesh, neighs) && numAngleLimit < 5){
			findClosestNeighWithCentering(i, pMesh, neighs, cm, distances);
			numAngleLimit++;
		}

		//int * neighbours = new int[kneigh];
		//findNearestKNeighbours(pMesh, i, kneigh, neighbours);
		globalNeighbourhoods.push_back(neighs);

		delete[] distances;
	}

	return globalNeighbourhoods;
}

int DeleunayTriangulator::getKNeigh(PCTMeshGraph * pMesh){
	return std::min(std::max((int)(pMesh->numOfVertices * kNeigh_ratio), kNeigh_min), kNeigh_max);
}

void DeleunayTriangulator::findClosestNeighWithCentering(int i, PCTMeshGraph * pMesh, std::set<int> &neighs, PCTCVector3 &cm, int * distances){
	float minDistance = FLT_MAX;
	int minIndex = -1;
	// factor, which penalyzes points near center of mass of the neighs
	float centerFactor = 1.0;
	for (int j=0; j<pMesh->numOfVertices; j++){
		if (i != j){


			PCTCVector3 vector1 = cm - pMesh->pVerts[i];
			PCTCVector3 vector2 = cm - pMesh->pVerts[j];

			float dotProduct = Dot(vector1, vector2);

			float vectorsMagnitude = Magnitude(vector1) * Magnitude(vector2) ;

			float v = dotProduct / vectorsMagnitude;
			if (v > 1.0)
				v = 1.0;
			if (v < -1.0)
				v = -1.0;

			centerFactor = (-v / centerFactor_function_scale) + centerFactor_function_offset;
			// if new vertex is oriented near cm, centerFactor is close to 0.0...if the vertex is on the other side, center Factor is close to 1.0
			//OrientedAngleBetweenVectors(cm - pMesh->pVerts[i], cm - pMesh->pVerts[j]);
			if (centerFactor == 0 && neighs.size() == 0 || vectorsMagnitude == 0) // if there is only one vertex, so cm = pVerts[i] and centerFactor = 0.0 ... we should change it to 1.0
				centerFactor = 1.0;

			if (centerFactor < centerFactor_limit && centerFactor >= 0)
				centerFactor = centerFactor_limit;

			if (centerFactor > -centerFactor_limit && centerFactor < 0)
				centerFactor = -centerFactor_limit;

			if (distances[j] * centerFactor < minDistance && (neighs.find(j) == neighs.end())){
				minDistance = distances[j] * centerFactor;
				minIndex = j;
			}
		}
	}
	neighs.insert(minIndex);
	// compute center of mass of neighbourhood
	cm = PCTCVector3(0,0,0);
	for (std::set<int>::iterator it=neighs.begin(); it!=neighs.end(); ++it){
		cm = cm + pMesh->pVerts[*it];
	}
	cm = cm / neighs.size();
}
