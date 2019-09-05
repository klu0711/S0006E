#pragma once
#include <math.h>
#include "Vector4D.h"
namespace {
	class Matrix4
	{
	private:
		float matrix[16];
	public:
		Matrix4();
		Matrix4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p);
		Matrix4(const Matrix4& mat);
		Matrix4(float mat[16]);
		~Matrix4();
		float operator[](const int index) const;
		float & operator[](const int index);
		Matrix4 operator*(const Matrix4& rhs) const;
		Vector4D operator*(const Vector4D& rhs) const;
		Matrix4 operator+(const Matrix4& rhs) const;
		Matrix4 operator*(const float scalar) const;
		static Matrix4 rotX(float angle);
		static Matrix4 rotY(float angle);
		static Matrix4 rotZ(float angle);
		static Matrix4 rotVec(Vector4D& vec, const float angle);
		static Matrix4 transpose(const Matrix4& mat);
		static Matrix4 inverse(const Matrix4& mat);
		float* getPointer();
		static Matrix4 getPositionMatrix(Vector4D inVector);
		static Matrix4 lookAt(Vector4D &R, Vector4D &U, Vector4D &D, Vector4D &pos);
		static Matrix4 perspectiveProjection(float n, float f, float r, float l,float t, float b);
	};

	/// Getter
	float Matrix4::operator[](const int index) const
	{
		return matrix[index];
	}
	/// Setter
	float & Matrix4::operator[](const int index)
	{
		return matrix[index];
	}
	/// Copy constructor
	Matrix4::Matrix4(const Matrix4& mat)
	{
		for (int i = 0; i < 16; i++)
		{
			matrix[i] = mat.matrix[i];
		}
	}
	/// Constructor for identity matrix
	Matrix4::Matrix4()
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
	Matrix4::Matrix4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
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
	Matrix4::Matrix4(float mat[16])
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
	Matrix4::~Matrix4()
	{

	}
	/// Rotation matrix around x-axis
	Matrix4 Matrix4::rotX(float angle)
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

		return Matrix4(returnMatrix);

	}
	/// Rotation matrix around y-axis
	Matrix4 Matrix4::rotY(float angle)
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

		return Matrix4(returnMatrix);

	}
	/// Rotation matrix around z-axis
	Matrix4 Matrix4::rotZ(float angle)
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

		return Matrix4(returnMatrix);
	}
	/// Rotate around a vector a certain, given angle
	Matrix4 Matrix4::rotVec(Vector4D& vec, float angle)
	{
		Vector4D normVec = vec.normalize();

		float C[16] = { 0,-normVec[2],normVec[1],0,
						normVec[2],0,-normVec[0],0,
						-normVec[1],normVec[0],0,0,
						0,0,0, 1 };
		Matrix4 matC(C);
		Matrix4 I;
		Matrix4 rotMat = I + (matC*sin(angle)) + (matC*matC)*(1 - cos(angle));
		rotMat.matrix[15] = 1;

		return rotMat;

	}
	/// Product of two matrices
	Matrix4 Matrix4::operator*(const Matrix4& rhs) const
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

		return Matrix4(returnMatrix);


	}
	/// Product of a matrix and a vector	
	Vector4D Matrix4::operator*(const Vector4D& rhs) const
	{
		return(Vector4D(
			rhs[0] * matrix[0] + rhs[1] * matrix[1] + rhs[2] * matrix[2] + rhs[3] * matrix[3],
			rhs[0] * matrix[4] + rhs[1] * matrix[5] + rhs[2] * matrix[6] + rhs[3] * matrix[7],
			rhs[0] * matrix[8] + rhs[1] * matrix[9] + rhs[2] * matrix[10] + rhs[3] * matrix[11],
			rhs[0] * matrix[12] + rhs[1] * matrix[13] + rhs[2] * matrix[14] + rhs[3] * matrix[15]));
	}
	/// Needed to perform rotation around a vector
	Matrix4 Matrix4::operator+(const Matrix4& rhs) const
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


		return Matrix4(returnVector);
	}
	/// Needed to perform rotation around a vector
	Matrix4 Matrix4::operator*(const float scalar) const
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

		return Matrix4(returnVector);

	}
	/// Transposes the given matrix and return the transpose as a new matrix
	Matrix4 Matrix4::transpose(const Matrix4& mat)
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

		return Matrix4(returnMatrix);
	}
	/// Calculate the invere of a matrix, if the determinant is zerothe function returns the same matrix otherwise the inverse is returned as a new matrix
	Matrix4 Matrix4::inverse(const Matrix4& mat)
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


		/// Calculate once to save 15 calculations below
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


		return Matrix4(out);

	}
	float * Matrix4::getPointer()
	{ 
		return matrix;
	}
	/// Print function used for testing the output from a matrix

	Matrix4 Matrix4::getPositionMatrix(Vector4D inVector) {
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
		tempArr[15] = inVector.getVectorValue(3);
		return Matrix4(tempArr);
	}
	/// Function for creating a "look at" matrix the glm librari already has one but by writing my own the project has less dependencies
	inline Matrix4 Matrix4::lookAt(Vector4D &R, Vector4D &U, Vector4D &D, Vector4D &pos)
	{
		float temp[16] = { R[0],R[1] ,R[2], 0,
							U[0],U[1],U[2],0,
							D[0],D[1],D[2],0,
							0,0,0,1 };
		 Matrix4 temp2 = Matrix4(temp);
		 Matrix4 temp3 = Matrix4::getPositionMatrix(Vector4D(-pos.getVectorValue(0), -pos.getVectorValue(1), -pos.getVectorValue(2),1));
		 

		return (temp2*temp3);
	}

	inline Matrix4 Matrix4::perspectiveProjection(float n, float f, float r, float l, float t, float b)
	{
		return Matrix4();
	}
	
}