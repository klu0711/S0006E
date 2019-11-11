#pragma once
#include <cmath>

	class vec4
	{
	private:
		float vector[4];
        static float lerp(float first, float second, float by)
        {
            return first * (1 - by) + second * by;
        }
	public:
		vec4(float x, float y, float z, float w);
		vec4(float* xyzw);
		vec4();
		~vec4();
        static vec4 vLerp(vec4 first, vec4 second, float by)
        {
            float retX = lerp(first[0], second[0], by);
            float rety = lerp(first[1], second[1], by);
            float retz = lerp(first[2], second[2], by);
            float retw = lerp(first[3], second[3], by);
            return vec4(retX, rety, retz, retw);
        }
        static vec4 Slerp(vec4 first, vec4 second, float by)
        {
            first.normalize();
            second.normalize();
            double dot = first.dotProduct(second);

            if(dot < 0.0f)
            {
                second = second * -1;
                dot = -dot;
            }

            const double dotThreshold = 0.9995;
            if(dot > dotThreshold)
            {
                return vLerp(first, second, by);
            }

            double theta_0 = acos(dot);
            double theta = theta_0*by;
            double sin_theta = sin(theta);
            double sin_theta_0 = sin(theta_0);

            double s0 = cos(theta) - dot * sin_theta / sin_theta_0;
            double s1 = sin_theta / sin_theta_0;
            return (first * s0) + (second * s1);
        }
		vec4 operator+(const vec4& rhs) const;
		vec4 operator-(const vec4& rhs) const;
		vec4 operator*(const float& rhs) const;
		float operator[](const int index) const;
		float & operator[](const int index);
		float length() const;
		vec4 normalize() const;
		float dotProduct(const vec4& vec) const;
		vec4 crossProduct(const vec4& vec) const;
		float getVectorValue(int index);
		float* getPointer();

        float dot3(const vec4 &vec) const;
    };


	/// Constructor when no arguments are provided (Defaut constructor
	inline vec4::vec4()
	{
		vector[0] = 0;
		vector[1] = 0;
		vector[2] = 0;
		vector[3] = 1;
	}
	inline vec4::vec4(float *xyzw)
	{
        vector[0] = xyzw[0];
        vector[1] = xyzw[1];
        vector[2] = xyzw[2];
        vector[3] = xyzw[3];
	}
	/// Deconstructor
	inline vec4::~vec4()
	{

	}


	/// Operator overload for standard vector addition
	inline vec4 vec4::operator+(const vec4& rhs) const
	{
		return vec4(vector[0] + rhs.vector[0], vector[1] + rhs.vector[1], vector[2] + rhs.vector[2], vector[3] + rhs.vector[3]);
	}
	/// Operator overload for standard vector subtraction
	inline vec4 vec4::operator-(const vec4& rhs) const
	{
		return vec4(vector[0] - rhs.vector[0], vector[1] - rhs.vector[1], vector[2] - rhs.vector[2], 1);
	}
	/// Operator overload for the product of a vector and a scalar
	inline vec4 vec4::operator*(const float& rhs) const
	{
		return vec4(vector[0] * rhs, vector[1] * rhs, vector[2] * rhs, vector[3] * rhs );
	}
	/// Getter
	inline float vec4::operator[](const int index) const
	{
		return vector[index];
	}
	/// Setter
	inline float & vec4::operator[](const int index)
	{
		return vector[index];
	}
	/// Calculates the lenth of the vector
	inline float vec4::length() const
	{
		return sqrt(pow(vector[0], 2) + pow(vector[1], 2) + pow(vector[2], 2) +  pow(vector[3], 2));
	}
	/// Divides all the values of the vector with the length of the vector (normalization)
	inline vec4 vec4::normalize() const
	{
		float length = this->length();
		return vec4(vector[0] / length, vector[1] / length, vector[2] / length, vector[3] / length);
	}
	/// Returns a float of the dot product of the vectors
    inline float vec4::dot3(const vec4& vec) const
    {
        return ((vector[0] * vec.vector[0]) + (vector[1] * vec.vector[1]) + (vector[2] * vec.vector[2]));
    }
	inline float vec4::dotProduct(const vec4& vec) const
	{
		return ((vector[0] * vec.vector[0]) + (vector[1] * vec.vector[1]) + (vector[2] * vec.vector[2]) + (vector[3] * vec.vector[3]));
	}
	/// Returns a new vector with the cross product of the vectors
	inline vec4 vec4::crossProduct(const vec4& vec) const
	{
		return(vec4((vector[1]*vec[2])-(vector[2]*vec[1]), (vector[2]*vec[0])-(vector[0]*vec[2]),
						(vector[0]*vec[1])-(vector[1]*vec[0]),1));
	}
	inline float vec4::getVectorValue(int index)
	{
		return vector[index];
	}
	inline float * vec4::getPointer()
	{
		return vector;
	}
	/// standard constructor to set all four values of the vactor
	inline vec4::vec4(float x, float y, float z, float w)
	{
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
		vector[3] = w;

	}

