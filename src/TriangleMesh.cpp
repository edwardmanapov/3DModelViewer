//Created for Graphics I and II
//Author: Minsi Chen

#include "TriangleMesh.h"
#include "OBJFileReader.h"
#include "OGLWindow.h"
#include <gl\GL.h>


TriangleMesh::TriangleMesh()
{
	m_vertices = NULL;
	m_triangles = NULL;
	m_numTriangles = 0;
	m_numVertices = 0;
}


TriangleMesh::~TriangleMesh()
{
	CleanUp();
}

void TriangleMesh::Render(const Matrix4x4* trans)
{
	//TODO: Advanced Feature
	//Use the OpenGL fixed pipeline to render a lit mesh
	//Note: for per-face lighting you need to compute the normal for each triangle see ComputeTriangleNormals
	// for per-vertex lighting you need to compute the normal for each vertex see ComputeVertexNormals

	int i;

	// Render the TriangleMesh using the vertices and triangle data stored in m_vertices and m_triangle
	for (i = 0; i < m_numTriangles; i++) {
		int* vertex_index;
		vertex_index = (int *)m_triangles[i].GetVertexIndices();

		Vector4D vertex1(m_vertices[vertex_index[0]].m_position[0],
						 m_vertices[vertex_index[0]].m_position[1],
						 m_vertices[vertex_index[0]].m_position[2]);

		Vector4D vertex2(m_vertices[vertex_index[1]].m_position[0],
						 m_vertices[vertex_index[1]].m_position[1],
						 m_vertices[vertex_index[1]].m_position[2]);

		Vector4D vertex3(m_vertices[vertex_index[2]].m_position[0],
						 m_vertices[vertex_index[2]].m_position[1],
						 m_vertices[vertex_index[2]].m_position[2]);

		// Transform the triangle mesh using the given transformation trans
		vertex1 = (*trans)*vertex1;
		vertex2 = (*trans)*vertex2;
		vertex3 = (*trans)*vertex3;

		glBegin(GL_TRIANGLES);

		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(vertex1[0], vertex1[1], vertex1[2]);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(vertex2[0], vertex2[1], vertex2[2]);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(vertex3[0], vertex3[1], vertex3[2]);

		glEnd();
	}
}

void TriangleMesh::ComputeTriangleNormals()
{
	//TODO: Advanced feature
	//Compute the normal for each triangle stored in m_triangles
}

void TriangleMesh::ComputeVertexNormals()
{
	//TODO: Advanced feature
	//Compute the normal for each vertex stored in m_vertices
}

bool TriangleMesh::LoadMeshFromOBJFile(const char* filename)
{
	//First pass: reading the OBJ to gether info related to the mesh
	int numVertices = 0;
	int numNormals = 0;
	int numTexCoords = 0;
	int numTriangles = 0;

	if (OBJFileReader::firstPassOBJRead(filename, &numVertices, &numNormals, &numTexCoords, &numTriangles))
	{
		printf("Error parsing file: %s\n", filename);
		return false;
	}

	//allocate memory for m_vertices and m_triangles based on the number of vertices and triangles from the first pass
	if (m_vertices && m_triangles)
	{
		printf("Vertex array and triangle array have already been allocated.\n");
		return false;
	}

	m_vertices = new Vertex[numVertices];
	m_triangles = new Triangle[numTriangles];

	if (OBJFileReader::secondPassOBJRead(filename, m_vertices, m_triangles))
	{
		printf("Error parsing file: %s\n", filename);
		return false;
	}

	ComputeTriangleNormals();
	ComputeVertexNormals();

	m_numTriangles = numTriangles;
	m_numVertices = numVertices;

	return true;
}

void TriangleMesh::CleanUp()
{
	if (m_vertices)
		delete[] m_vertices;
	if (m_triangles)
		delete[] m_triangles;

	m_vertices = NULL;
	m_triangles = NULL;
	m_numTriangles = 0;
	m_numVertices = 0;
}