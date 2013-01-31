// Face.h : subor pre pracu s facetmi

namespace Model
{
	using namespace std;

	public class Face
	{
	public:
		Face();
		Face(Vertex* v1, Vertex* v2, Vertex* v3, Vertex* normala, vector<Face*> sused);
		~Face();

		Vertex*					v[3];
		Vertex*					normal;
		vector<Face*>			susedia;					// susedia
		int						farba;						// pre picking
		//SDF					value;
	};
}