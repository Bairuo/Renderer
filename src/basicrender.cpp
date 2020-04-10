#include "basicrender.h"
#include <glm/glm.hpp>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>

static const GLuint squareIndices[6] =
{
    0, 1, 2,
    1, 2, 3
};

static const GLfloat squareVertices[12]
{
    -0.5f, 0.5f, 0,
    0.5f, 0.5f, 0,
    -0.5f, -0.5f, 0,
    0.5f, -0.5f, 0
};

static const GLfloat cubeVertices[] =
{
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

static int icoSpIndex = 0;
static std::vector<glm::vec3> icoSpPositions;
static std::vector<int> icoSpIndices;
static std::map<int64_t, int> middlePointIndexCache;

static int addVertex(glm::vec3 p)
{
	float length = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	icoSpPositions.push_back(glm::vec3(p.x / length, p.y / length, p.z / length));
	return icoSpIndex++;
}

static int getMiddlePoint(int p1, int p2)
{
	int64_t smallerIndex = std::min(p1, p2);
	int64_t greaterIndex = std::max(p1, p2);
	int64_t key = (smallerIndex << 32) + greaterIndex;

	if (middlePointIndexCache.count(key)) {
		return middlePointIndexCache[key];
	}

	glm::vec3 point1 = icoSpPositions[p1];
	glm::vec3 point2 = icoSpPositions[p2];
	glm::vec3 middle{(point1.x + point2.x) / 2,
					(point1.y + point2.y) / 2,
					(point1.z + point2.z) / 2};

	int i = addVertex(middle);
	middlePointIndexCache[key] = i;
	return i;
}

static void createIcoSphere(const int level)
{
	float t = (1.0f + sqrtf(5.0f)) / 2;

	addVertex(glm::vec3(-1, t, 0));
	addVertex(glm::vec3(1, t, 0));
	addVertex(glm::vec3(-1, -t, 0));
	addVertex(glm::vec3(1, -t, 0));

	addVertex(glm::vec3(0, -1, t));
	addVertex(glm::vec3(0, 1, t));
	addVertex(glm::vec3(0, -1, -t));
	addVertex(glm::vec3(0, 1, -t));

	addVertex(glm::vec3(t, 0, -1));
	addVertex(glm::vec3(t, 0, 1));
	addVertex(glm::vec3(-t, 0, -1));
	addVertex(glm::vec3(-t, 0, 1));

	std::vector<TriangleIndices> faces;

	// 5 faces around point 0
	faces.push_back(TriangleIndices(0, 11, 5));
	faces.push_back(TriangleIndices(0, 5, 1));
	faces.push_back(TriangleIndices(0, 1, 7));
	faces.push_back(TriangleIndices(0, 7, 10));
	faces.push_back(TriangleIndices(0, 10, 11));

	// 5 adjacent faces
	faces.push_back(TriangleIndices(1, 5, 9));
	faces.push_back(TriangleIndices(5, 11, 4));
	faces.push_back(TriangleIndices(11, 10, 2));
	faces.push_back(TriangleIndices(10, 7, 6));
	faces.push_back(TriangleIndices(7, 1, 8));

	// 5 faces around point 3
	faces.push_back(TriangleIndices(3, 9, 4));
	faces.push_back(TriangleIndices(3, 4, 2));
	faces.push_back(TriangleIndices(3, 2, 6));
	faces.push_back(TriangleIndices(3, 6, 8));
	faces.push_back(TriangleIndices(3, 8, 9));

	// 5 adjacent faces
	faces.push_back(TriangleIndices(4, 9, 5));
	faces.push_back(TriangleIndices(2, 4, 11));
	faces.push_back(TriangleIndices(6, 2, 10));
	faces.push_back(TriangleIndices(8, 6, 7));
	faces.push_back(TriangleIndices(9, 8, 1));

	for (int i = 0; i < level; i++)
	{
		std::vector<TriangleIndices> faces2;

		for (auto tri : faces)
		{
			int a = getMiddlePoint(tri.v1, tri.v2);
			int b = getMiddlePoint(tri.v2, tri.v3);
			int c = getMiddlePoint(tri.v3, tri.v1);

			faces2.push_back(TriangleIndices(tri.v1, a, c));
			faces2.push_back(TriangleIndices(tri.v2, b, a));
			faces2.push_back(TriangleIndices(tri.v3, c, b));
			faces2.push_back(TriangleIndices(a, b, c));
		}
		faces = faces2;
	}

	for (auto tri : faces)
	{
		icoSpIndices.push_back(tri.v1);
		icoSpIndices.push_back(tri.v2);
		icoSpIndices.push_back(tri.v3);
	}
}

GLuint getIcoSphereVAO(const unsigned int level, unsigned int &triNum)
{
	static bool init = false;

	static GLuint VAO;
	static GLuint VBO;
	static GLuint EBO;

	if (init)
	{
		return VAO;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind Vertex Array Object
	glBindVertexArray(VAO);

	createIcoSphere(level);

	// copy to vertex buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * icoSpPositions.size(), &icoSpPositions[0], GL_STATIC_DRAW);

	// Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * icoSpIndices.size(), &icoSpIndices[0], GL_STATIC_DRAW);

	triNum = icoSpIndices.size();

	// Set the vertex attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLAT, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	init = true;

	return VAO;
}

GLuint getCubeVAO()
{
	static bool init = false;

	static GLuint VAO;
	static GLuint VBO;
	static GLuint EBO;

	if (init)
	{
		return VAO;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind Vertex Array Object
	glBindVertexArray(VAO);

	// copy to vertex buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set the vertex attribute pointer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	init = true;

	return VAO;
}

GLuint getSquareVAO()
{
    static bool init = false;

    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

    if(init)
    {
        return VAO;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind Vertex Array Object
    glBindVertexArray(VAO);

    // copy to vertex buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    // Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    // Set the vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLAT, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    init = true;

    return VAO;
}

