#pragma once

#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Lenz
{

public:
	struct points_array {
		double* p_array;
		int size;
	};

	struct LenzParams {
		double R1;
		double R2;
		double n;
		double d;
		double a1;
		double a2;
	};


public:
	glm::dmat2 matrix;
	LenzParams* lenz_param;

public:
	Lenz(const double&, const double&, const double&, const double&, const double&);

	glm::dmat2 createTMatrix(const double&, const double&);
	glm::dmat2 createRMatrix(const double&, const double&, const double&);
	void createLenz(const double&, const double&, const double&, const double&);
	void createLenzMatrix(const double&);

	void createLenzPoints(Lenz::points_array* output);
};

