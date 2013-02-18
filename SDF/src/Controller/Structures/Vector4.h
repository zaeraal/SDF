// Vector4.h : 3D vektor/bos
#pragma once
#include <cmath>

namespace Structures
{
    struct Vector4
    {
		public:
        double X;
        double Y;
        double Z;
        double W;

		Vector4 (): X(0), Y(0), Z(0), W(0) {}
		Vector4 (double x, double y, double z, double w = 0) { Init(x,y,z,w); }

		void Init (double x, double y, double z, double w = 0)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

        double Length () { return sqrt(X * X + Y * Y + Z * Z + W * W); }

		void Normalize ()
		{
			double length = Length();
			X = X/length;
			Y = Y/length;
			Z = Z/length;
			W = W/length;
		}
    };
	
	#pragma region Operators

	// sum of 2 vectors
	inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
	{
		Vector4 result;
		result.X = lhs.X + rhs.X;
		result.Y = lhs.Y + rhs.Y;
		result.Z = lhs.Z + rhs.Z;
		result.W = lhs.W + rhs.W;
		return result;
	}

	// substraction of 2 vectors
	inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
	{
		Vector4 result;
		result.X = lhs.X - rhs.X;
		result.Y = lhs.Y - rhs.Y;
		result.Z = lhs.Z - rhs.Z;
		result.W = lhs.W - rhs.W;
		return result;
	}

	// multplication of vector and number
	inline Vector4 operator*(const Vector4& lhs, const double& rhs)
	{
		Vector4 result;
		result.X = lhs.X * rhs;
		result.Y = lhs.Y * rhs;
		result.Z = lhs.Z * rhs;
		result.W = lhs.W * rhs;
		return result;
	}
	// multplication of number and vector
	inline Vector4 operator*(const double& lhs, const Vector4& rhs)
	{
		return rhs*lhs;
	}

	// division of vector and number
	inline Vector4 operator/(const Vector4& lhs, const double& rhs)
	{
		Vector4 result;
		result.X = lhs.X / rhs;
		result.Y = lhs.Y / rhs;
		result.Z = lhs.Z / rhs;
		result.W = lhs.W / rhs;
		return result;
	}

	// Dot Product
	inline double operator*(const Vector4& lhs, const Vector4& rhs)
	{
		return ( lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W );
	}

	// 3D Cross Product
	inline Vector4 operator%(const Vector4& lhs, const Vector4& rhs)
	{
		Vector4 result;
		result.X = lhs.Y * rhs.Z - lhs.Z * rhs.Y;
		result.Y = lhs.Z * rhs.X - lhs.X * rhs.Z;
		result.Z = lhs.X * rhs.Y - lhs.Y * rhs.X;
		return result;
	}
	#pragma endregion
}
