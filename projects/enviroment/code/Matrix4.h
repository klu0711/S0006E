#pragma once
#include <math.h>
#include "Vector4D.h"
	class mat4
	{
	private:
		float matrix[16];
	public:
		mat4();
		mat4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p);
		mat4(const mat4& mat);
		mat4(float mat[16]);
		~mat4();
		float operator[](const int index) const;
		float & operator[](const int index);
		mat4 operator*(const mat4& rhs) const;
		vec4 operator*(const vec4& rhs) const;
		mat4 operator+(const mat4& rhs) const;
		mat4 operator*(const float scalar) const;
		static mat4 Perspective(float fieldOfView, float aspectRatio, float nearClip, float farClip);
		vec4 getPositionVec();
		float returnFirst() {return matrix[0];}
		void operator=(const mat4& rhs);
		static mat4 rotX(float angle);
		static mat4 rotY(float angle);
		static mat4 rotZ(float angle);
		static mat4 rotVec(vec4& vec, const float angle);
		static mat4 getQmat(vec4& vec);
		static mat4 scaleMat(vec4& vec);
		static mat4 transpose(const mat4& mat);
		static mat4 inverse(const mat4& mat);
		float* getPointer();
		static mat4 getPositionMatrix(vec4& inVector);
		static mat4 lookAt(vec4 position, vec4 target, vec4 up);
		static mat4 perspectiveProjection(float n, float f, float r, float l,float t, float b);
		void print();
	};

	/// Getter
	inline float mat4::operator[](const int index) const
	{
		return matrix[index];
	}
	/// Setter
	inline float & mat4::operator[](const int index)
	{
		return matrix[index];
	}
	/// Copy constructor
	inline mat4::mat4(const mat4& mat)
	{
		for (int i = 0; i < 16; i++)
		{
			matrix[i] = mat.matrix[i];
		}
	}
	/// Constructor for identity matrix
	inline mat4::mat4()
	{
		matrix[0] = 1.0f;
		matrix[1] = 0.0f;
		matrix[2] = 0.0f;
		matrix[3] = 0.0f;

		matrix[4] = 0;
		matrix[5] = 1;
		matrix[6] = 0;
		matrix[7] = 0;

		matrix[8] = 0;
		matrix[9] = 0;
		matrix[10] = 1;
		matrix[11] = 0;

		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1;

	}
inline mat4 mat4::Perspective(float fieldOfView, float aspectRatio, float nearClip, float farClip)
{

    mat4 result;

    result = mat4(
            1.0f/(aspectRatio * tan(fieldOfView/2)), 0, 0, 0,
            0, 1.0f/(tan(fieldOfView/2)), 0, 0,
            0, 0, -((nearClip + farClip) / (farClip - nearClip)), -1,
            0, 0, ( (-2 * farClip * nearClip) / (farClip - nearClip)), 0
    );

    return result;
}
	inline mat4::mat4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
	{
		matrix[0] = a;
		matrix[1] = b;
		matrix[2] = c;
		matrix[3] = d;
		matrix[4] = e;
		matrix[5] = f;
		matrix[6] = g;
		matrix[7] = h;
		matrix[8] = i;
		matrix[9] = j;
		matrix[10] = k;
		matrix[11] = l;
		matrix[12] = m;
		matrix[13] = n;
		matrix[14] = o;
		matrix[15] = p;
	}
	/// Constructor that takes an array and builds a new matrix
	inline mat4::mat4(float mat[16])
	{
		matrix[0] = mat[0];
		matrix[1] = mat[1];
		matrix[2] = mat[2];
		matrix[3] = mat[3];
		matrix[4] = mat[4];
		matrix[5] = mat[5];
		matrix[6] = mat[6];
		matrix[7] = mat[7];
		matrix[8] = mat[8];
		matrix[9] = mat[9];
		matrix[10] = mat[10];
		matrix[11] = mat[11];
		matrix[12] = mat[12];
		matrix[13] = mat[13];
		matrix[14] = mat[14];
		matrix[15] = mat[15];

	}
	/// Deconstructor
	inline mat4::~mat4()
	{

	}
	/// Rotation matrix around x-axis
	inline mat4 mat4::rotX(float angle)
	{
		const float cosAng = cos(angle);
		const float sinAng = sin(angle);
		float returnMatrix[16];

		returnMatrix[0] = 1;
		returnMatrix[1] = 0;
		returnMatrix[2] = 0;
		returnMatrix[3] = 0;

		returnMatrix[4] = 0;
		returnMatrix[5] = cosAng;
		returnMatrix[6] = -sinAng;
		returnMatrix[7] = 0;

		returnMatrix[8] = 0;
		returnMatrix[9] = sinAng;
		returnMatrix[10] = cosAng;
		returnMatrix[11] = 0;

		returnMatrix[12] = 0;
		returnMatrix[13] = 0;
		returnMatrix[14] = 0;
		returnMatrix[15] = 1;

		return mat4(returnMatrix);

	}
	/// Rotation matrix around y-axis
	inline mat4 mat4::rotY(float angle)
	{
		const float cosAng = cos(angle);
		const float sinAng = sin(angle);
		float returnMatrix[16];

		returnMatrix[0] = cosAng;
		returnMatrix[1] = 0;
		returnMatrix[2] = sinAng;
		returnMatrix[3] = 0;

		returnMatrix[4] = 0;
		returnMatrix[5] = 1;
		returnMatrix[6] = 0;
		returnMatrix[7] = 0;

		returnMatrix[8] = -sinAng;
		returnMatrix[9] = 0;
		returnMatrix[10] = cosAng;
		returnMatrix[11] = 0;

		returnMatrix[12] = 0;
		returnMatrix[13] = 0;
		returnMatrix[14] = 0;
		returnMatrix[15] = 1;

		return mat4(returnMatrix);

	}
	/// Rotation matrix around z-axis
	inline mat4 mat4::rotZ(float angle)
	{
		const float cosAng = cos(angle);
		const float sinAng = sin(angle);
		float returnMatrix[16];

		returnMatrix[0] = cosAng;
		returnMatrix[1] = -sinAng;
		returnMatrix[2] = 0;
		returnMatrix[3] = 0;

		returnMatrix[4] = sinAng;
		returnMatrix[5] = cosAng;
		returnMatrix[6] = 0;
		returnMatrix[7] = 0;

		returnMatrix[8] = 0;
		returnMatrix[9] = 0;
		returnMatrix[10] = 1;
		returnMatrix[11] = 0;

		returnMatrix[12] = 0;
		returnMatrix[13] = 0;
		returnMatrix[14] = 0;
		returnMatrix[15] = 1;

		return mat4(returnMatrix);
	}
	/// Rotate around a vector a certain, given angle
	inline mat4 mat4::rotVec(vec4& vec, float angle)
	{
		vec4 normVec = vec.normalize();

		float C[16] = { 0,-normVec[2],normVec[1],0,
						normVec[2],0,-normVec[0],0,
						-normVec[1],normVec[0],0,0,
						0,0,0, 1 };
		mat4 matC(C);
		mat4 I;
		mat4 rotMat = I + (matC*sin(angle)) + (matC*matC)*(1 - cos(angle));
		rotMat.matrix[15] = 1;

		return rotMat;

	}
	inline mat4 mat4::getQmat(vec4 &vec)
	{
	    float xx = vec[0] * vec[0];
	    float xy = vec[0] * vec[1];
	    float xz = vec[0] * vec[2];
	    float xw = vec[0] * vec[3];

        float yy = vec[1] * vec[1];
        float yz = vec[1] * vec[2];
        float yw = vec[1] * vec[3];

        float zz = vec[2] * vec[2];
        float zw = vec[2] * vec[3];

        return mat4(1-2*(yy+zz), 2*(xy-zw), 2*(xz+yw), 0,
                        2*(xy+zw), 1-2*(xx+zz), 2*(yz-xw), 0,
                        2*(xz-yw), 2*(yz+xw), 1-2*(xx+yy), 0,
                        0,0,0,1);

    }
	inline mat4 mat4::scaleMat(vec4 &vec)
	{
        float C[16] = { vec[0], 0, 0, 0,
                        0, vec[1] , 0, 0,
                        0, 0, vec[2], 0,
                        0, 0, 0, 1 };
        return C;
	}
	/// Product of two matrices
	inline mat4 mat4::operator*(const mat4& rhs) const
	{
		float returnMatrix[16];

		returnMatrix[0] = (matrix[0] * rhs.matrix[0]) + (matrix[1] * rhs.matrix[4]) + (matrix[2] * rhs.matrix[8]) + (matrix[3] * rhs.matrix[12]);
		returnMatrix[1] = (matrix[0] * rhs.matrix[1]) + (matrix[1] * rhs.matrix[5]) + (matrix[2] * rhs.matrix[9]) + (matrix[3] * rhs.matrix[13]);
		returnMatrix[2] = (matrix[0] * rhs.matrix[2]) + (matrix[1] * rhs.matrix[6]) + (matrix[2] * rhs.matrix[10]) + (matrix[3] * rhs.matrix[14]);
		returnMatrix[3] = (matrix[0] * rhs.matrix[3]) + (matrix[1] * rhs.matrix[7]) + (matrix[2] * rhs.matrix[11]) + (matrix[3] * rhs.matrix[15]);

		returnMatrix[4] = (matrix[4] * rhs.matrix[0]) + (matrix[5] * rhs.matrix[4]) + (matrix[6] * rhs.matrix[8]) + (matrix[7] * rhs.matrix[12]);
		returnMatrix[5] = (matrix[4] * rhs.matrix[1]) + (matrix[5] * rhs.matrix[5]) + (matrix[6] * rhs.matrix[9]) + (matrix[7] * rhs.matrix[13]);
		returnMatrix[6] = (matrix[4] * rhs.matrix[2]) + (matrix[5] * rhs.matrix[6]) + (matrix[6] * rhs.matrix[10]) + (matrix[7] * rhs.matrix[14]);
		returnMatrix[7] = (matrix[4] * rhs.matrix[3]) + (matrix[5] * rhs.matrix[7]) + (matrix[6] * rhs.matrix[11]) + (matrix[7] * rhs.matrix[15]);

		returnMatrix[8] = (matrix[8] * rhs.matrix[0]) + (matrix[9] * rhs.matrix[4]) + (matrix[10] * rhs.matrix[8]) + (matrix[11] * rhs.matrix[12]);
		returnMatrix[9] = (matrix[8] * rhs.matrix[1]) + (matrix[9] * rhs.matrix[5]) + (matrix[10] * rhs.matrix[9]) + (matrix[11] * rhs.matrix[13]);
		returnMatrix[10] = (matrix[8] * rhs.matrix[2]) + (matrix[9] * rhs.matrix[6]) + (matrix[10] * rhs.matrix[10]) + (matrix[11] * rhs.matrix[14]);
		returnMatrix[11] = (matrix[8] * rhs.matrix[3]) + (matrix[9] * rhs.matrix[7]) + (matrix[10] * rhs.matrix[11]) + (matrix[11] * rhs.matrix[15]);

		returnMatrix[12] = (matrix[12] * rhs.matrix[0]) + (matrix[13] * rhs.matrix[4]) + (matrix[14] * rhs.matrix[8]) + (matrix[15] * rhs.matrix[12]);
		returnMatrix[13] = (matrix[12] * rhs.matrix[1]) + (matrix[13] * rhs.matrix[5]) + (matrix[14] * rhs.matrix[9]) + (matrix[15] * rhs.matrix[13]);
		returnMatrix[14] = (matrix[12] * rhs.matrix[2]) + (matrix[13] * rhs.matrix[6]) + (matrix[14] * rhs.matrix[10]) + (matrix[15] * rhs.matrix[14]);
		returnMatrix[15] = (matrix[12] * rhs.matrix[3]) + (matrix[13] * rhs.matrix[7]) + (matrix[14] * rhs.matrix[11]) + (matrix[15] * rhs.matrix[15]);

		return mat4(returnMatrix);


	}
	/// Product of a matrix and a vector	
	inline vec4 mat4::operator*(const vec4& rhs) const
	{
		return(vec4(
			rhs[0] * matrix[0] + rhs[1] * matrix[1] + rhs[2] * matrix[2] + rhs[3] * matrix[3],
			rhs[0] * matrix[4] + rhs[1] * matrix[5] + rhs[2] * matrix[6] + rhs[3] * matrix[7],
			rhs[0] * matrix[8] + rhs[1] * matrix[9] + rhs[2] * matrix[10] + rhs[3] * matrix[11],
			rhs[0] * matrix[12] + rhs[1] * matrix[13] + rhs[2] * matrix[14] + rhs[3] * matrix[15]));
	}
	/// Needed to perform rotation around a vector
	inline mat4 mat4::operator+(const mat4& rhs) const
	{
		float returnVector[16];
		/// adds all the corresponding elements in both matricies to eachother
		returnVector[0] = matrix[0] + rhs.matrix[0];
		returnVector[1] = matrix[1] + rhs.matrix[1];
		returnVector[2] = matrix[2] + rhs.matrix[2];
		returnVector[3] = matrix[3] + rhs.matrix[3];
		returnVector[4] = matrix[4] + rhs.matrix[4];
		returnVector[5] = matrix[5] + rhs.matrix[5];
		returnVector[6] = matrix[6] + rhs.matrix[6];
		returnVector[7] = matrix[7] + rhs.matrix[7];
		returnVector[8] = matrix[8] + rhs.matrix[8];
		returnVector[9] = matrix[9] + rhs.matrix[9];
		returnVector[10] = matrix[10] + rhs.matrix[10];
		returnVector[11] = matrix[11] + rhs.matrix[11];
		returnVector[12] = matrix[12] + rhs.matrix[12];
		returnVector[13] = matrix[13] + rhs.matrix[13];
		returnVector[14] = matrix[14] + rhs.matrix[14];
		returnVector[15] = matrix[15] + rhs.matrix[15];


		return mat4(returnVector);
	}
	/// Needed to perform rotation around a vector
	inline mat4 mat4::operator*(const float scalar) const
	{
		float returnVector[16];
		returnVector[0] = matrix[0] * scalar;
		returnVector[1] = matrix[1] * scalar;
		returnVector[2] = matrix[2] * scalar;
		returnVector[3] = matrix[3] * scalar;
		returnVector[4] = matrix[4] * scalar;
		returnVector[5] = matrix[5] * scalar;
		returnVector[6] = matrix[6] * scalar;
		returnVector[7] = matrix[7] * scalar;
		returnVector[8] = matrix[8] * scalar;
		returnVector[9] = matrix[9] * scalar;
		returnVector[10] = matrix[10] * scalar;
		returnVector[11] = matrix[11] * scalar;
		returnVector[12] = matrix[12] * scalar;
		returnVector[13] = matrix[13] * scalar;
		returnVector[14] = matrix[14] * scalar;
		returnVector[15] = matrix[15] * scalar;

		return mat4(returnVector);

	}
	inline vec4 mat4::getPositionVec()
	{
	    return vec4(matrix[3], matrix[7], matrix[11], matrix[15]);
	}

	inline void mat4::operator=(const mat4 & rhs)
	{
		matrix[0] = rhs.matrix[0];
		matrix[1] = rhs.matrix[1];
		matrix[2] = rhs.matrix[2];
		matrix[3] = rhs.matrix[3];

		matrix[4] = rhs.matrix[4];
		matrix[5] = rhs.matrix[5];
		matrix[6] = rhs.matrix[6];
		matrix[7] = rhs.matrix[7];

		matrix[8] = rhs.matrix[8];
		matrix[9] = rhs.matrix[9];
		matrix[10] = rhs.matrix[10];
		matrix[11] = rhs.matrix[11];

		matrix[12] = rhs.matrix[12];
		matrix[13] = rhs.matrix[13];
		matrix[14] = rhs.matrix[14];
		matrix[15] = rhs.matrix[15];
	}
	/// Transposes the given matrix and return the transpose as a new matrix
	inline mat4 mat4::transpose(const mat4& mat)
	{
		float returnMatrix[16];
		returnMatrix[0] = mat.matrix[0];
		returnMatrix[4] = mat.matrix[1];
		returnMatrix[8] = mat.matrix[2];
		returnMatrix[12] = mat.matrix[3];

		returnMatrix[1] = mat.matrix[4];
		returnMatrix[5] = mat.matrix[5];
		returnMatrix[9] = mat.matrix[6];
		returnMatrix[13] = mat.matrix[7];

		returnMatrix[2] = mat.matrix[8];
		returnMatrix[6] = mat.matrix[9];
		returnMatrix[10] = mat.matrix[10];
		returnMatrix[14] = mat.matrix[11];

		returnMatrix[3] = mat.matrix[12];
		returnMatrix[7] = mat.matrix[13];
		returnMatrix[11] = mat.matrix[14];
		returnMatrix[15] = mat.matrix[15];

		return mat4(returnMatrix);
	}
	/// Calculate the invere of a matrix, if the determinant is zerothe function returns the same matrix otherwise the inverse is returned as a new matrix
	inline mat4 mat4::inverse(const mat4& mat)
	{
		float inv[16], determinant, out[16];

		inv[0] = mat.matrix[5] * mat.matrix[10] * mat.matrix[15] -
			mat.matrix[5] * mat.matrix[11] * mat.matrix[14] -
			mat.matrix[9] * mat.matrix[6] * mat.matrix[15] +
			mat.matrix[9] * mat.matrix[7] * mat.matrix[14] +
			mat.matrix[13] * mat.matrix[6] * mat.matrix[11] -
			mat.matrix[13] * mat.matrix[7] * mat.matrix[10];

		inv[4] = -mat.matrix[4] * mat.matrix[10] * mat.matrix[15] +
			mat.matrix[4] * mat.matrix[11] * mat.matrix[14] +
			mat.matrix[8] * mat.matrix[6] * mat.matrix[15] -
			mat.matrix[8] * mat.matrix[7] * mat.matrix[14] -
			mat.matrix[12] * mat.matrix[6] * mat.matrix[11] +
			mat.matrix[12] * mat.matrix[7] * mat.matrix[10];

		inv[8] = mat.matrix[4] * mat.matrix[9] * mat.matrix[15] -
			mat.matrix[4] * mat.matrix[11] * mat.matrix[13] -
			mat.matrix[8] * mat.matrix[5] * mat.matrix[15] +
			mat.matrix[8] * mat.matrix[7] * mat.matrix[13] +
			mat.matrix[12] * mat.matrix[5] * mat.matrix[11] -
			mat.matrix[12] * mat.matrix[7] * mat.matrix[9];

		inv[12] = -mat.matrix[4] * mat.matrix[9] * mat.matrix[14] +
			mat.matrix[4] * mat.matrix[10] * mat.matrix[13] +
			mat.matrix[8] * mat.matrix[5] * mat.matrix[14] -
			mat.matrix[8] * mat.matrix[6] * mat.matrix[13] -
			mat.matrix[12] * mat.matrix[5] * mat.matrix[10] +
			mat.matrix[12] * mat.matrix[6] * mat.matrix[9];

		inv[1] = -mat.matrix[1] * mat.matrix[10] * mat.matrix[15] +
			mat.matrix[1] * mat.matrix[11] * mat.matrix[14] +
			mat.matrix[9] * mat.matrix[2] * mat.matrix[15] -
			mat.matrix[9] * mat.matrix[3] * mat.matrix[14] -
			mat.matrix[13] * mat.matrix[2] * mat.matrix[11] +
			mat.matrix[13] * mat.matrix[3] * mat.matrix[10];

		inv[5] = mat.matrix[0] * mat.matrix[10] * mat.matrix[15] -
			mat.matrix[0] * mat.matrix[11] * mat.matrix[14] -
			mat.matrix[8] * mat.matrix[2] * mat.matrix[15] +
			mat.matrix[8] * mat.matrix[3] * mat.matrix[14] +
			mat.matrix[12] * mat.matrix[2] * mat.matrix[11] -
			mat.matrix[12] * mat.matrix[3] * mat.matrix[10];

		inv[9] = -mat.matrix[0] * mat.matrix[9] * mat.matrix[15] +
			mat.matrix[0] * mat.matrix[11] * mat.matrix[13] +
			mat.matrix[8] * mat.matrix[1] * mat.matrix[15] -
			mat.matrix[8] * mat.matrix[3] * mat.matrix[13] -
			mat.matrix[12] * mat.matrix[1] * mat.matrix[11] +
			mat.matrix[12] * mat.matrix[3] * mat.matrix[9];

		inv[13] = mat.matrix[0] * mat.matrix[9] * mat.matrix[14] -
			mat.matrix[0] * mat.matrix[10] * mat.matrix[13] -
			mat.matrix[8] * mat.matrix[1] * mat.matrix[14] +
			mat.matrix[8] * mat.matrix[2] * mat.matrix[13] +
			mat.matrix[12] * mat.matrix[1] * mat.matrix[10] -
			mat.matrix[12] * mat.matrix[2] * mat.matrix[9];

		inv[2] = mat.matrix[1] * mat.matrix[6] * mat.matrix[15] -
			mat.matrix[1] * mat.matrix[7] * mat.matrix[14] -
			mat.matrix[5] * mat.matrix[2] * mat.matrix[15] +
			mat.matrix[5] * mat.matrix[3] * mat.matrix[14] +
			mat.matrix[13] * mat.matrix[2] * mat.matrix[7] -
			mat.matrix[13] * mat.matrix[3] * mat.matrix[6];

		inv[6] = -mat.matrix[0] * mat.matrix[6] * mat.matrix[15] +
			mat.matrix[0] * mat.matrix[7] * mat.matrix[14] +
			mat.matrix[4] * mat.matrix[2] * mat.matrix[15] -
			mat.matrix[4] * mat.matrix[3] * mat.matrix[14] -
			mat.matrix[12] * mat.matrix[2] * mat.matrix[7] +
			mat.matrix[12] * mat.matrix[3] * mat.matrix[6];

		inv[10] = mat.matrix[0] * mat.matrix[5] * mat.matrix[15] -
			mat.matrix[0] * mat.matrix[7] * mat.matrix[13] -
			mat.matrix[4] * mat.matrix[1] * mat.matrix[15] +
			mat.matrix[4] * mat.matrix[3] * mat.matrix[13] +
			mat.matrix[12] * mat.matrix[1] * mat.matrix[7] -
			mat.matrix[12] * mat.matrix[3] * mat.matrix[5];

		inv[14] = -mat.matrix[0] * mat.matrix[5] * mat.matrix[14] +
			mat.matrix[0] * mat.matrix[6] * mat.matrix[13] +
			mat.matrix[4] * mat.matrix[1] * mat.matrix[14] -
			mat.matrix[4] * mat.matrix[2] * mat.matrix[13] -
			mat.matrix[12] * mat.matrix[1] * mat.matrix[6] +
			mat.matrix[12] * mat.matrix[2] * mat.matrix[5];

		inv[3] = -mat.matrix[1] * mat.matrix[6] * mat.matrix[11] +
			mat.matrix[1] * mat.matrix[7] * mat.matrix[10] +
			mat.matrix[5] * mat.matrix[2] * mat.matrix[11] -
			mat.matrix[5] * mat.matrix[3] * mat.matrix[10] -
			mat.matrix[9] * mat.matrix[2] * mat.matrix[7] +
			mat.matrix[9] * mat.matrix[3] * mat.matrix[6];

		inv[7] = mat.matrix[0] * mat.matrix[6] * mat.matrix[11] -
			mat.matrix[0] * mat.matrix[7] * mat.matrix[10] -
			mat.matrix[4] * mat.matrix[2] * mat.matrix[11] +
			mat.matrix[4] * mat.matrix[3] * mat.matrix[10] +
			mat.matrix[8] * mat.matrix[2] * mat.matrix[7] -
			mat.matrix[8] * mat.matrix[3] * mat.matrix[6];

		inv[11] = -mat.matrix[0] * mat.matrix[5] * mat.matrix[11] +
			mat.matrix[0] * mat.matrix[7] * mat.matrix[9] +
			mat.matrix[4] * mat.matrix[1] * mat.matrix[11] -
			mat.matrix[4] * mat.matrix[3] * mat.matrix[9] -
			mat.matrix[8] * mat.matrix[1] * mat.matrix[7] +
			mat.matrix[8] * mat.matrix[3] * mat.matrix[5];

		inv[15] = mat.matrix[0] * mat.matrix[5] * mat.matrix[10] -
			mat.matrix[0] * mat.matrix[6] * mat.matrix[9] -
			mat.matrix[4] * mat.matrix[1] * mat.matrix[10] +
			mat.matrix[4] * mat.matrix[2] * mat.matrix[9] +
			mat.matrix[8] * mat.matrix[1] * mat.matrix[6] -
			mat.matrix[8] * mat.matrix[2] * mat.matrix[5];

		/// Calculate the determinant
		determinant = mat.matrix[0] * inv[0] + mat.matrix[1] * inv[4] + mat.matrix[2] * inv[8] + mat.matrix[3] * inv[12];
		/// If the determinant is zero the matrix is not inversible therefor the function returns the same matrix again


		/// Calculate once to save 16 calculations below
		determinant = 1.0 / determinant;

		/// Divide each element with the determinant
		out[0] = inv[0] * determinant;
		out[1] = inv[1] * determinant;
		out[2] = inv[2] * determinant;
		out[3] = inv[3] * determinant;
		out[4] = inv[4] * determinant;
		out[5] = inv[5] * determinant;
		out[6] = inv[6] * determinant;
		out[7] = inv[7] * determinant;
		out[8] = inv[8] * determinant;
		out[9] = inv[9] * determinant;
		out[10] = inv[10] * determinant;
		out[11] = inv[11] * determinant;
		out[12] = inv[12] * determinant;
		out[13] = inv[13] * determinant;
		out[14] = inv[14] * determinant;
		out[15] = inv[15] * determinant;


		return mat4(out);

	}
	inline float * mat4::getPointer()
	{ 
		return matrix;
	}
	/// Print function used for testing the output from a matrix

	inline 	mat4 mat4::getPositionMatrix(vec4& inVector) {
		float tempArr[16];
		tempArr[0] = 1;
		tempArr[1] = 0;
		tempArr[2] = 0;
		tempArr[3] = inVector.getVectorValue(0);
		

		tempArr[4] = 0;
		tempArr[5] = 1;
		tempArr[6] = 0;
		tempArr[7] = inVector.getVectorValue(1);

		tempArr[8] = 0;
		tempArr[9] = 0;
		tempArr[10] = 1;
		tempArr[11] = inVector.getVectorValue(2);

		tempArr[12] = 0;
		tempArr[13] = 0;
		tempArr[14] = 0;
		tempArr[15] = 1;
		return mat4(tempArr);
	}
	/// Function for creating a "look at" matrix. The glm library already has one but by writing my own the project has less dependencies
	inline mat4 mat4::lookAt(vec4 position, vec4 target, vec4 up)
	{
		vec4 cameraForward = (target - position);
		vec4 cameraUp = up.normalize3();
		vec4 cameraRight = (cameraForward.crossProduct(cameraUp)).normalize3();
        cameraUp = cameraRight.crossProduct(cameraForward);

		float temp[] = {cameraRight.getVectorValue(0), cameraRight.getVectorValue(1), cameraRight.getVectorValue(2), -(cameraRight.dot3(position)),
                        cameraUp.getVectorValue(0), cameraUp.getVectorValue(1), cameraUp.getVectorValue(2), -(cameraUp.dot3(position)),
						-cameraForward.getVectorValue(0), -cameraForward.getVectorValue(1) , -cameraForward.getVectorValue(2), cameraForward.dot3(position),
						0,0,0,1};
		return mat4(mat4::transpose(temp));
		 
	}

	inline mat4 mat4::perspectiveProjection(float n, float f, float r, float l, float t, float b)
	{
		return mat4();
	}
	
