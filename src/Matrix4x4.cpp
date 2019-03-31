#include <memory.h>
#include "Vector4D.h"
#include "Matrix4x4.h"
#include <cmath>

Matrix4x4::Matrix4x4()
{
	SetIdentity();
}

Matrix4x4::Matrix4x4(const Matrix4x4& rhs)
{
	size_t matrix_size = sizeof(m_element);

	memcpy_s((&m_element[0][0]), matrix_size, rhs.ToPtr(), matrix_size);
}

Matrix4x4::~Matrix4x4()
{

}

const float* Matrix4x4::ToPtr() const
{
	return &(m_element[0][0]);
}

void Matrix4x4::SetZero()
{
	memset(&m_element, 0, sizeof(m_element));
}

void Matrix4x4::SetIdentity()
{
	SetZero();
	m_element[0][0] = m_element[1][1] = m_element[2][2] = m_element[3][3] = 1.0f;
}

Matrix4x4 Matrix4x4::operator* (const Matrix4x4& rhs) const
{
	Matrix4x4 result;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			result.m_element[row][col] = 0;
			for (int index = 0; index < 4; index++)
			{
				result.m_element[row][col] += m_element[row][index] * rhs.m_element[index][col];
			}
		}
	}
	return result;
}

Vector4D Matrix4x4::operator* (const Vector4D& rhs) const
{
	Vector4D returnMe(0.0, 0.0, 0.0);
	int row;
	int col;
	for (row = 2; row >= 0; --row)
	{
		for (col = 2; col >= 0; --col)
		{
			returnMe[row] += m_element[row][col] * rhs[col];
		}
	}

	for (row = 0; row < 3; row++)
	{
		returnMe[row] += m_element[row][3];
	}
	return returnMe;
}

void Matrix4x4::SetRotationXAxis(float degrees)
{
	SetIdentity();
	m_element[1][1] = cos(degrees);
	m_element[1][2] = -sin(degrees);
	m_element[2][1] = sin(degrees);
	m_element[2][2] = cos(degrees);
}

void Matrix4x4::SetRotationYAxis(float degrees)
{
	SetIdentity();
	m_element[0][0] = cos(degrees);
	m_element[2][0] = -sin(degrees);
	m_element[0][1] = sin(degrees);
	m_element[2][1] = cos(degrees);
}

void Matrix4x4::SetRotationZAxis(float degrees)
{
	SetIdentity();
	m_element[0][0] = cos(degrees);
	m_element[0][1] = -sin(degrees);
	m_element[1][0] = sin(degrees);
	m_element[1][1] = cos(degrees);
}

void Matrix4x4::SetTranslate(const Vector4D& tvec)
{
	SetIdentity();
	m_element[3][0] = tvec[0];
	m_element[3][1] = tvec[1];
	m_element[3][2] = tvec[2];
	m_element[3][3] = tvec[3];
}

void Matrix4x4::SetScale(float sx, float sy, float sz)
{
	SetIdentity();
	m_element[0][0] = sx;
	m_element[1][1] = sy;
	m_element[2][2] = sz;
}

void Matrix4x4::SetViewMatrix(const Vector4D& camera_position, const Vector4D& view_vector, const Vector4D& up_vector)
{
	Vector4D right_vector = view_vector.CrossProduct(up_vector);
	right_vector.Normalise();

	m_element[0][0] = right_vector[0];
	m_element[1][0] = right_vector[1];
	m_element[2][0] = right_vector[2];
	m_element[3][0] = camera_position[0];

	m_element[0][1] = up_vector[0];
	m_element[1][1] = up_vector[1];
	m_element[2][1] = up_vector[2];
	m_element[3][1] = camera_position[1];

	m_element[0][2] = view_vector[0];
	m_element[1][2] = view_vector[1];
	m_element[2][2] = view_vector[2];
	m_element[3][2] = camera_position[2];

	m_element[0][3] = 0.0;
	m_element[1][3] = 0.0;
	m_element[2][3] = 0.0;
	m_element[3][3] = 1.0;
}
