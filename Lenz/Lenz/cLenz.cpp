#include "cLenz.h"
#include<iostream>

const double pi = std::acos(-1);

Lenz::Lenz(const double& R1, const double& R2, const double& n, const double& d, const double& nl) {
	lenz_param = (LenzParams*)malloc(sizeof(LenzParams));

	double a1 = -((R2 + R1 - d) * (R2 + R1 - d) - R2 * R2 + R1 * R1) / (2 * (R2 + R1 - d));
	double a2 = ((R2 + R1 - d) * (R2 + R1 - d) + R2 * R2 - R1 * R1) / (2 * (R2 + R1 - d));
	*lenz_param = LenzParams{R1, -R2, nl, d, a1, a2};
	matrix = glm::dmat2(1.0);
}

glm::dmat2 Lenz::createTMatrix(const double& d, const double& n) {
	return glm::dmat2(1.0, d / n, 1.0, 0.0);
}
glm::dmat2 Lenz::createRMatrix(const double& R, const double& n, const double& nf) {
	return glm::dmat2(1.0, 0.0, 1.0, -(nf - n) / R);
}
void Lenz::createLenz(const double& R, const double& n, const double& d, const double& nl) {
}


void Lenz::createLenzPoints(Lenz::points_array* output) {
	double a1 = -lenz_param->a1;
	double a2 = -lenz_param->a2;

	std::vector<double> r_values_1;
	//std::vector<double> r_values_2;

	for (double p = -(0.50) * pi; p <= (0.50) * pi; p += (0.05) * pi) {
		for (double t = -(0.50) * pi; t <= (0.5) * pi; t += (0.05) * pi) {
			double r = a1 * std::cos(t) - std::sqrt((a1 * a1) * std::cos(t) * std::cos(t) + lenz_param->R1 * lenz_param->R1 - a1 * a1);

			r_values_1.push_back(10 * r * std::cos(t));
			r_values_1.push_back(10 * r * std::sin(t) * std::cos(p));
			r_values_1.push_back(10 * r * std::sin(t) * std::sin(p));
		}
	}
	for (double p = -(0.50) * pi; p <= (0.50) * pi; p += (0.05) * pi) {
		for (double t = -(0.50) * pi; t <= (0.50) * pi; t += (0.05) * pi) {
			double r = a2 * std::cos(t) + std::sqrt((a2 * a2) * std::cos(t) * std::cos(t) + lenz_param->R2 * lenz_param->R2 - a2 * a2);

			r_values_1.push_back(10 * r * std::cos(t));
			r_values_1.push_back(10 * r * std::sin(t) * std::cos(p));
			r_values_1.push_back(10 * r * std::sin(t) * std::sin(p));
		}
	}

	int size_1 = r_values_1.size();

	double* p_a = new double[size_1];

	std::copy(r_values_1.begin(), r_values_1.end(), p_a);

	*output = points_array{ p_a, size_1 };
}



void Lenz::createLenzMatrix(const double& n) {
	glm::mat2 R2 = createRMatrix(lenz_param->R2, lenz_param->n, n);
	glm::mat2 R1 = createRMatrix(lenz_param->R1, n, lenz_param->n);
	glm::mat2 T21 = createTMatrix(lenz_param->d, lenz_param->n);
	matrix = R2 * T21 * R1;
}