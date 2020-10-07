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
		unsigned int* ind;
		int size;
	};

	struct LenzParams {
		double xpos;
		double R1;
		double R2;
		double n;
		double d;
		double a1;
		double a2;
		double offset;
	};

public:
	~Lenz();


public:
	glm::dmat2 matrix;
	LenzParams* lenz_param;
	points_array* output;

public:

	virtual void initializeLenz(const double& ,const double&, const double&, const double&, const double&, const double&) = 0;
	glm::dmat2 createTMatrix(const double&, const double&);
	glm::dmat2 createRMatrix(const double&, const double&, const double&);
	void createLenz(const double&, const double&, const double&, const double&);
	void createLenzMatrix(const double&);

	virtual void createLenzPoints() = 0;

	void cross_product(const double* values, double* out) {
		double *a = new double[3], *b = new double[3];

		for (int i = 0; i < 3; i++) {
			*(a + i) = *(values + 3 + i) - *(values + i);
			*(b + i) = *(values + 6 + i) - *(values + i);
		}
		

		*(out) = *(a + 0 + 1) * *(b + 2) - *(a + 2) * *(b + 1);
		*(out + 1) = *(a + 2) * *(b + 0) - *(a + 0) * *(b + 2);
		*(out + 2) = *(a + 0) * *(b + 1) - *(a + 1) * *(b + 0);



		delete[] a;
		delete[] b;


	}
};
class Biconvex : public Lenz {
	void createLenzPoints();
	void initializeLenz(const double&, const double&, const double&, const double&, const double&, const double&);
};
class Biconcave : public Lenz {
	void createLenzPoints();
	void initializeLenz(const double&, const double&, const double&, const double&, const double&, const double&);
};
class ConcaveConvex : public Lenz {
	void createLenzPoints();
	void initializeLenz(const double&, const double&, const double&, const double&, const double&, const double&);
};

