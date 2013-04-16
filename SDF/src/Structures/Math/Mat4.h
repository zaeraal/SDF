// Mat.h : 4x4 matice
#pragma once
#include "Vector4.h"

namespace MathStructures
{
	const float Identity[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    struct Mat4
    {
		public:
        float M[16];

		Mat4 ()
		{
			for (int i = 0; i < 16; i++)
				M[i] = Identity[i];
		}
		Mat4 (float m[16])	// wtf, to nezobere proste float?
		{
			for (int i = 0; i < 16; i++)
				M[i] = m[i];
		}
		Mat4 (float a0, float a1, float a2, float a3,
			  float b0, float b1, float b2, float b3,
			  float c0, float c1, float c2, float c3,
			  float d0, float d1, float d2, float d3)
		{
			M[0] = a0;		M[1] = a1;		M[2] = a2;		M[3] = a3;
			M[4] = b0;		M[5] = b1;		M[6] = b2;		M[7] = b3;
			M[8] = c0;		M[9] = c1;		M[10] = c2;		M[11] = c3;
			M[12] = d0;		M[13] = d1;		M[14] = d2;		M[15] = d3;
		}
		Mat4 (Vector4& a, Vector4& b, Vector4& c)
		{
			M[0] = a.X;		M[1] = a.Y;		M[2] = a.Z;		M[3] = a.W;
			M[4] = b.X;		M[5] = b.Y;		M[6] = b.Z;		M[7] = b.W;
			M[8] = c.X;		M[9] = c.Y;		M[10] = c.Z;	M[11] = c.W;
			M[12] = 0.0;	M[13] = 0.0;	M[14] = 0.0;	M[15] = 1.0;
		}

		void Transpose ()
		{
			// skopiruje udaje
			Mat4 t = Mat4(M);
			M[0] = t.M[0];		M[1] = t.M[4];		M[2] = t.M[8];		M[3] = t.M[12];
			M[4] = t.M[1];		M[5] = t.M[5];		M[6] = t.M[9];		M[7] = t.M[13];
			M[8] = t.M[2];		M[9] = t.M[6];		M[10] = t.M[10];	M[11] = t.M[14];
			M[12] = t.M[3];		M[13] = t.M[7];		M[14] = t.M[11];	M[15] = t.M[15];
		}
    };
	
	#pragma region Operators

	// sum of 2 matrixes
	inline Mat4 operator+(const Mat4& lhs, const Mat4& rhs)
	{
		Mat4 result;
		for (int i = 0; i < 16; i++)
			result.M[i] = lhs.M[i] + rhs.M[i];

		return result;
	}

	// substraction of 2 matrixes
	inline Mat4 operator-(const Mat4& lhs, const Mat4& rhs)
	{
		Mat4 result;
		for (int i = 0; i < 16; i++)
			result.M[i] = lhs.M[i] - rhs.M[i];

		return result;
	}

	// multplication of matrix and number
	inline Mat4 operator*(const Mat4& lhs, const float& rhs)
	{
		Mat4 result;
		for (int i = 0; i < 16; i++)
			result.M[i] = lhs.M[i] * rhs;

		return result;
	}
	// multplication of number and matrix
	inline Mat4 operator*(const float& lhs, const Mat4& rhs)
	{
		return rhs*lhs;
	}

	// division of matrix and number
	inline Mat4 operator/(const Mat4& lhs, const float& rhs)
	{
		Mat4 result;
		for (int i = 0; i < 16; i++)
			result.M[i] = lhs.M[i] / rhs;

		return result;
	}

	// multiplication of 2 matrixes
	inline Mat4 operator*(const Mat4& lhs, const Mat4& rhs)
	{
		return Mat4(
			lhs.M[0] * rhs.M[0] + lhs.M[1] * rhs.M[4] + lhs.M[2] * rhs.M[8] + lhs.M[3] * rhs.M[12],
			lhs.M[0] * rhs.M[1] + lhs.M[1] * rhs.M[5] + lhs.M[2] * rhs.M[9] + lhs.M[3] * rhs.M[13],
			lhs.M[0] * rhs.M[2] + lhs.M[1] * rhs.M[6] + lhs.M[2] * rhs.M[10] + lhs.M[3] * rhs.M[14],
			lhs.M[0] * rhs.M[3] + lhs.M[1] * rhs.M[7] + lhs.M[2] * rhs.M[11] + lhs.M[3] * rhs.M[15],

			lhs.M[4] * rhs.M[0] + lhs.M[5] * rhs.M[4] + lhs.M[6] * rhs.M[8] + lhs.M[7] * rhs.M[12],
			lhs.M[4] * rhs.M[1] + lhs.M[5] * rhs.M[5] + lhs.M[6] * rhs.M[9] + lhs.M[7] * rhs.M[13],
			lhs.M[4] * rhs.M[2] + lhs.M[5] * rhs.M[6] + lhs.M[6] * rhs.M[10] + lhs.M[7] * rhs.M[14],
			lhs.M[4] * rhs.M[3] + lhs.M[5] * rhs.M[7] + lhs.M[6] * rhs.M[11] + lhs.M[7] * rhs.M[15],
				
			lhs.M[8] * rhs.M[0] + lhs.M[9] * rhs.M[4] + lhs.M[10] * rhs.M[8] + lhs.M[11] * rhs.M[12],
			lhs.M[8] * rhs.M[1] + lhs.M[9] * rhs.M[5] + lhs.M[10] * rhs.M[9] + lhs.M[11] * rhs.M[13],
			lhs.M[8] * rhs.M[2] + lhs.M[9] * rhs.M[6] + lhs.M[10] * rhs.M[10] + lhs.M[11] * rhs.M[14],
			lhs.M[8] * rhs.M[3] + lhs.M[9] * rhs.M[7] + lhs.M[10] * rhs.M[11] + lhs.M[11] * rhs.M[15],
				
			lhs.M[12] * rhs.M[0] + lhs.M[13] * rhs.M[4] + lhs.M[14] * rhs.M[8] + lhs.M[15] * rhs.M[12],
			lhs.M[12] * rhs.M[1] + lhs.M[13] * rhs.M[5] + lhs.M[14] * rhs.M[9] + lhs.M[15] * rhs.M[13],
			lhs.M[12] * rhs.M[2] + lhs.M[13] * rhs.M[6] + lhs.M[14] * rhs.M[10] + lhs.M[15] * rhs.M[14],
			lhs.M[12] * rhs.M[3] + lhs.M[13] * rhs.M[7] + lhs.M[14] * rhs.M[11] + lhs.M[15] * rhs.M[15]
		);
	}

	// multiplication of vector and matrix
	inline Vector4 operator*(const Vector4& a, const Mat4& b)
	{
		Vector4 result = Vector4(
			a.X * b.M[0] + a.Y * b.M[4] + a.Z * b.M[8] + a.W * b.M[12],
			a.X * b.M[1] + a.Y * b.M[5] + a.Z * b.M[9] + a.W * b.M[13],
			a.X * b.M[2] + a.Y * b.M[6] + a.Z * b.M[10] + a.W * b.M[14],
			a.X * b.M[3] + a.Y * b.M[7] + a.Z * b.M[11] + a.W * b.M[15]
		);
		return result;
	}

	// multiplication of matrix and vector
	inline Vector4 operator*(const Mat4& a, const Vector4& b)
	{
		return Vector4(
			a.M[0] * b.X + a.M[1] * b.Y + a.M[2] * b.Z + a.M[3] * b.W,
			a.M[4] * b.X + a.M[5] * b.Y + a.M[6] * b.Z + a.M[7] * b.W,
			a.M[8] * b.X + a.M[9] * b.Y + a.M[10] * b.Z + a.M[11] * b.W,
			a.M[12] * b.X + a.M[13] * b.Y + a.M[14] * b.Z + a.M[15] * b.W
		);
	}

	#pragma endregion
}
