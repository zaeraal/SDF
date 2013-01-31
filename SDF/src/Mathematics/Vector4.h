// Vector4.h : 3D vektor/bos
#pragma once

namespace Mathematics
{
    class Vector4
    {
		public:
        double X;
        double Y;
        double Z;
        double W;

        double Length();
        Vector4(double x, double y, double z, double w = 0);

		Vector4& operator+=(const Vector4& rhs);
		inline Vector4 operator+(const Vector4& rhs);

		Vector4& operator-=(const Vector4& rhs);
		inline Vector4 operator-(const Vector4& rhs);

		Vector4& operator*=(const double& rhs);
		inline Vector4 operator*(const double& rhs);

		Vector4& operator/=(const double& rhs);
		inline Vector4 operator/(const double& rhs);

		/// Dot Product
		double& operator*=(const Vector4& rhs);
		inline double operator%(const Vector4& rhs);

		/// 3D Cross Product
		Vector4& operator%=(const Vector4& rhs);
    };
}
