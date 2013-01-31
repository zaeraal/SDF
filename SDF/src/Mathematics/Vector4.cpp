// Vector4.cpp : 3D vektor/bos
#include "stdafx.h"
#include "Vector4.h"

namespace Mathematics
{
	double Vector4::Length()
	{
		return sqrt(X * X + Y * Y + Z * Z + W * W);
	}

	Vector4::Vector4(double x, double y, double z, double w)
	{
		X = x;
		Y = y;
		Z = z;
		W = w;
	}

	Vector4& Vector4::operator+=(const Vector4& rhs) 
	{
		(*this).X += rhs.X;
		(*this).Y += rhs.Y;
		(*this).Z += rhs.Z;
		(*this).W += rhs.W;
		return *this;
	}
	inline Vector4 Vector4::operator+(const Vector4& rhs)
	{
		(*this) += rhs;
		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& rhs) 
	{
		(*this).X -= rhs.X;
		(*this).Y -= rhs.Y;
		(*this).Z -= rhs.Z;
		(*this).W -= rhs.W;
		return *this;
	}
	inline Vector4 Vector4::operator-(const Vector4& rhs)
	{
		(*this) -= rhs;
		return *this;
	}

	Vector4& Vector4::operator*=(const double& rhs) 
	{
		(*this).X *= rhs;
		(*this).Y *= rhs;
		(*this).Z *= rhs;
		(*this).W *= rhs;
		return *this;
	}
	inline Vector4 Vector4::operator*(const double& rhs)
	{
		(*this) *= rhs;
		return *this;
	}

	Vector4& Vector4::operator/=(const double& rhs) 
	{
		(*this).X /= rhs;
		(*this).Y /= rhs;
		(*this).Z /= rhs;
		(*this).W /= rhs;
		return *this;
	}
	inline Vector4 Vector4::operator/(const double& rhs)
	{
		(*this) /= rhs;
		return *this;
	}

	/// Dot Product
	double& Vector4::operator*=(const Vector4& rhs)
	{
		double result = (*this).X * rhs.X + (*this).Y * rhs.Y + (*this).Z * rhs.Z + (*this).W * rhs.W;
		return result;
	}
	inline double Vector4::operator%(const Vector4& rhs)
	{
		return (*this) *= rhs;
	}

    /// 3D Cross Product
	Vector4& Vector4::operator%=(const Vector4& rhs)
	{
		double x = (*this).Y * rhs.Z - (*this).Z * rhs.Y;
		double y = (*this).Z * rhs.X - (*this).X * rhs.Z;
		double z = (*this).X * rhs.Y - (*this).Y * rhs.X;

		(*this).X = x;
		(*this).Y = y;
		(*this).Z = z;
		(*this).W = 0;

		return *this;
	}
	inline Vector4 operator%(Vector4& lhs, const Vector4& rhs)
	{
		lhs %= rhs;
		return lhs;
	}
}