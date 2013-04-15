// kratky subor s funkciami na matiku
#include "stdafx.h"
#include "MathHelper.h"

// get radians from degrees
void GetRadians(float& AngleX, float& Angley)
{
	AngleX = AngleX * float(M_PI / 180.0);
	Angley = Angley * float(M_PI / 180.0);
}

//get degrees from radians
void GetDegrees(float& AngleX, float& Angley)
{
	AngleX = AngleX * float(180.0 / M_PI);
	Angley = Angley * float(180.0 / M_PI);
}

Vector4 CalcRayFromAngle(float AngleX, float Angley)
{
	Vector4 result;
	GetRadians(AngleX, Angley);
	result.X = sin(Angley) * sin(AngleX);
	result.Y = cos(Angley);
	result.Z = sin(Angley) * cos(AngleX);
	return result;
}

void CalcAnglesFromRay(Vector4 ray, float& AngleX, float& Angley)
{
	float r = sqrt(ray.X*ray.X + ray.Y*ray.Y + ray.Z*ray.Z);
	AngleX = atan2(ray.Z, ray.X);
	Angley = acos(ray.Y / r);
}

// prienik luca a trojuholnika
// p - zdroj luca, d - smer luca, v0,v1,v2 - body trojuholniku
bool rayIntersectsTriangle(Vector4 p, Vector4 d, Vector4 v0, Vector4 v1, Vector4 v2, float& t)
{
	Vector4 e1 = v1 - v0;
	Vector4 e2 = v2 - v0;
	float a,f,u,v;

	Vector4 h = d % e2;
	a = e1 * h;

	if (a > -0.00001 && a < 0.00001)
		return false;

	f = 1.0f / a;
	Vector4 s = p - v0;
	u = f * (s*h);

	if (u < 0.0 || u > 1.0)
		return false;

	Vector4 q = s % e1;
	v = f * (d*q);

	if (v < 0.0 || u + v > 1.0)
		return false ;

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	t = f * (e2*q);

	if (t > 0.00001) // ray intersection (v smere luca)
		return true;

	else // this means that there is a line intersection
		 // but not a ray intersection
		 return false;
}

void FINDMINMAX(float X, float Y, float Z, float& min, float& max)
{
	min = max = X;   

	if(Y<min) min=Y;
	if(Y>max) max=Y;

	if(Z<min) min=Z;
	if(Z>max) max=Z;
}

// extra pomocna funkcia
void getVMinVMax(float normal, float maxbox, float v, float& vmin, float& vmax)
{
	if(normal>0.0f)
	{
		vmin=-maxbox - v;
		vmax= maxbox - v;
	}
	else
	{
		vmin= maxbox - v;
		vmax=-maxbox - v;
	}
}

bool planeBoxOverlap(Vector4 normal, Vector4 vert, float maxbox)
{
	Vector4 vmin,vmax;
	float _vmin = 0,_vmax = 0;

	getVMinVMax(normal.X, maxbox, vert.X, _vmin, _vmax); vmin.X = _vmin; vmax.X = _vmax;
	getVMinVMax(normal.Y, maxbox, vert.Y, _vmin, _vmax); vmin.Y = _vmin; vmax.Y = _vmax;
	getVMinVMax(normal.Z, maxbox, vert.Z, _vmin, _vmax); vmin.Z = _vmin; vmax.Z = _vmax;

	if((normal*vmin)>0.0f) return 0;
	if((normal*vmax)>=0.0f) return 1;
	return 0;
}

bool AXISTEST(float a, float b, float fa, float fb, float boxhalfsize, float k1, float k2, float l1, float l2)
{
	float min = 0;
	float max = 0;
	float p0 = a*k1 - b*k2;
	float p2 = a*l1 - b*l2;

    if(p0<p2) {min=p0; max=p2;}
	else {min=p2; max=p0;}

	float rad = fa * boxhalfsize + fb * boxhalfsize;
	if(min>rad || max<-rad)
		return 0;

	return 1;
}

// main function for triangle box overlaping
bool triBoxOverlap(Vector4 boxcenter, float boxhalfsize, Vector4 p0, Vector4 p1, Vector4 p2)
{
  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */

   Vector4 v0,v1,v2;
   Vector4 e0,e1,e2;
   Vector4 normal;
   float min,max,fex,fey,fez;

   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
   v0 = p0 - boxcenter;
   v1 = p1 - boxcenter;
   v2 = p2 - boxcenter;

   /* compute triangle edges */
   e0 = v1 - v0;      /* tri edge 0 */
   e1 = v2 - v1;      /* tri edge 1 */
   e2 = v0 - v2;      /* tri edge 2 */

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = fabs(e0.X);
   fey = fabs(e0.Y);
   fez = fabs(e0.Z);

   if (!AXISTEST(e0.Z, e0.Y, fez, fey, boxhalfsize, v0.Y, v0.Z, v2.Y, v2.Z)) return false;
   if (!AXISTEST(e0.Z, e0.X, fez, fex, boxhalfsize, -v0.X, -v0.Z, -v2.X, -v2.Z)) return false;
   if (!AXISTEST(e0.Y, e0.X, fey, fex, boxhalfsize, v1.X, v1.Y, v2.X, v2.Y)) return false;


   fex = fabs(e1.X);
   fey = fabs(e1.Y);
   fez = fabs(e1.Z);

   if (!AXISTEST(e1.Z, e1.Y, fez, fey, boxhalfsize, v0.Y, v0.Z, v2.Y, v2.Z)) return false;
   if (!AXISTEST(e1.Z, e1.X, fez, fex, boxhalfsize, -v0.X, -v0.Z, -v2.X, -v2.Z)) return false;
   if (!AXISTEST(e1.Y, e1.X, fey, fex, boxhalfsize, v0.X, v0.Y, v1.X, v1.Y)) return false;


   fex = fabs(e2.X);
   fey = fabs(e2.Y);
   fez = fabs(e2.Z);

   if (!AXISTEST(e2.Z, e2.Y, fez, fey, boxhalfsize, v0.Y, v0.Z, v1.Y, v1.Z)) return false;
   if (!AXISTEST(e2.Z, e2.X, fez, fex, boxhalfsize, -v0.X, -v0.Z, -v1.X, -v1.Z)) return false;
   if (!AXISTEST(e2.Y, e2.X, fey, fex, boxhalfsize, v1.X, v1.Y, v2.X, v2.Y)) return false;

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */


   /* test in X-direction */
   FINDMINMAX(v0.X,v1.X,v2.X,min,max);
   if(min>boxhalfsize || max<-boxhalfsize)
	   return false;

   /* test in Y-direction */
   FINDMINMAX(v0.Y,v1.Y,v2.Y,min,max);
   if(min>boxhalfsize || max<-boxhalfsize)
	   return false;

   /* test in Z-direction */
   FINDMINMAX(v0.Z,v1.Z,v2.Z,min,max);
   if(min>boxhalfsize || max<-boxhalfsize)
	   return false;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */

   normal = e0 % e1;
   if(!planeBoxOverlap(normal,v0,boxhalfsize))
	   return false;

   return true;   /* box and triangle overlaps */

}