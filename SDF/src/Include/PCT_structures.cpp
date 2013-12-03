#include "PCT_structures.h"

/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a perpendicular vector from 2 given vectors by taking the cross product.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

PCTCVector3 Cross(PCTCVector3 vVector1, PCTCVector3 vVector2)
{
	PCTCVector3 vNormal;									// The vector to hold the cross product

	// The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)													// Get the X value
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));

	// The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));

	// The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vNormal;										// Return the cross product (Direction the polygon is facing - Normal)
}


/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the magnitude of a normal (or any other vector)
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Magnitude(PCTCVector3 vNormal)
{
	// This will give us the magnitude or "Norm" as some say, of our normal.
	// Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)  Where V is the vector
	double v = (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z);
	return (float)sqrt( v );
}


/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

PCTCVector3 Normalize(PCTCVector3 vNormal)
{
	float magnitude = Magnitude(vNormal);				// Get the magnitude of our normal

	// Now that we have the magnitude, we can divide our normal by that magnitude.
	// That will make our normal a total length of 1.  This makes it easier to work with too.

	vNormal.x /= magnitude;								// Divide the X value of our normal by it's magnitude
	vNormal.y /= magnitude;								// Divide the Y value of our normal by it's magnitude
	vNormal.z /= magnitude;								// Divide the Z value of our normal by it's magnitude

	// Finally, return our normalized normal.

	return vNormal;										// Return the new normal of length 1.
}


/////////////////////////////////////// NORMAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the normal of a polygon (The direction the polygon is facing)
/////
/////////////////////////////////////// NORMAL \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

PCTCVector3 Normal(PCTCVector3 vPolygon[])
{														// Get 2 vectors from the polygon (2 sides), Remember the order!
	PCTCVector3 vVector1 = vPolygon[2] - vPolygon[0];
	PCTCVector3 vVector2 = vPolygon[1] - vPolygon[0];

	PCTCVector3 vNormal = Cross(vVector1, vVector2);		// Take the cross product of our 2 vectors to get a perpendicular vector

	// Now we have a normal, but it's at a strange length, so let's make it length 1.

	vNormal = Normalize(vNormal);						// Use our function we created to normalize the normal (Makes it a length of one)

	return vNormal;										// Return our normal at our desired length
}


/////////////////////////////////// DOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This computers the dot product of 2 vectors
/////
/////////////////////////////////// DOT \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Dot(PCTCVector3 vVector1, PCTCVector3 vVector2)
{
	// The dot product is this equation: V1.V2 = (V1.x * V2.x  +  V1.y * V2.y  +  V1.z * V2.z)
	// In math terms, it looks like this:  V1.V2 = ||V1|| ||V2|| cos(theta)

			 //    (V1.x * V2.x        +        V1.y * V2.y        +        V1.z * V2.z)
	return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) );
}


/////////////////////////////////// ANGLE BETWEEN vectorS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This checks to see if a point is inside the ranges of a polygon
/////
/////////////////////////////////// ANGLE BETWEEN vectorS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

double AngleBetweenVectors(PCTCVector3 Vector1, PCTCVector3 Vector2)
{
	// Get the dot product of the vectors
	float dotProduct = Dot(Vector1, Vector2);

	// Get the product of both of the vectors magnitudes
	float vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ;

	float v = dotProduct / vectorsMagnitude;
	if (v > 1.0)
		v = 1.0;
	if (v < -1.0)
		v = -1.0;

	// Get the angle in radians between the 2 vectors
	double angle = acos( v );

	// Here we make sure that the angle is not a -1.#IND0000000 number, which means indefinate
	if(_isnan(angle))
		return 0;

	// Return the angle in radians
	return( angle );
}

double OrientedAngleBetweenVectors(PCTCVector3 Vector1, PCTCVector3 Vector2)
{
	double angle = AngleBetweenVectors(Vector1, Vector2);
	double dete = DeterminantInPlane(Vector1, Vector2, Cross(Vector1, Vector2));
	if (dete < 0)
		angle = -angle;
	return angle;
}

/////////////////////////////////// DISTANCE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This returns the distance between 2 3D points
/////
/////////////////////////////////// DISTANCE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

float Distance(PCTCVector3 vPoint1, PCTCVector3 vPoint2)
{
	// This is the classic formula used in beginning algebra to return the
	// distance between 2 points.  Since it's 3D, we just add the z dimension:
	//
	// Distance = sqrt(  (P2.x - P1.x)^2 + (P2.y - P1.y)^2 + (P2.z - P1.z)^2 )
	//

	double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
		(vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
		(vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) );

	// Return the distance between the 2 points
	return (float)distance;
}

float Distance(PCTCVector2 vPoint1, PCTCVector2 vPoint2)
{

	double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
		(vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y));

	// Return the distance between the 2 points
	return (float)distance;
}

float DeterminantInPlane(PCTCVector3 vVector1, PCTCVector3 vVector2, PCTCVector3 vVector3)
{

	return vVector1.x * vVector2.y * vVector3.z +	
		vVector1.z * vVector2.x * vVector3.y +
		vVector1.y * vVector2.z * vVector3.x -
		vVector1.z * vVector2.y * vVector3.x -
		vVector1.x * vVector2.z * vVector3.y -
		vVector1.y * vVector2.x * vVector3.z;

}

////////////