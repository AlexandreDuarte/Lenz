#include "cLenz.h"
#include<iostream>

const double pi = std::acos(-1);



Lenz::~Lenz()
{
	delete lenz_param;
	delete output;
}

glm::dmat2 Lenz::createTMatrix(const double& d, const double& n) {
	return glm::dmat2(1.0, d / n, 1.0, 0.0);
}
glm::dmat2 Lenz::createRMatrix(const double& R, const double& n, const double& nf) {
	return glm::dmat2(1.0, 0.0, 1.0, -(nf - n) / R);
}
void Lenz::createLenz(const double& R, const double& n, const double& d, const double& nl) {
}

void Biconvex::initializeLenz(const double& xpos, const double& R1, const double& R2, const double& n, const double& d, const double& nl) {
	double a1 = ((R2 + R1 - d) * (R2 + R1 - d) - R2 * R2 + R1 * R1) / (2 * (R2 + R1 - d));
	double a2 = ((R2 + R1 - d) * (R2 + R1 - d) + R2 * R2 - R1 * R1) / (2 * (R2 + R1 - d));

	double offset;

	if (R2 > R1)
		offset = -(R1-a1) + d/2;
	else 
		offset = -(R1-a1) + d/2;

	
	
	lenz_param = new LenzParams{ xpos, R1, -R2, nl, d, a1, a2, offset };
	matrix = glm::dmat2(1.0);
	output = nullptr;
}

void ConcaveConvex::initializeLenz(const double& xpos, const double& R1, const double& R2, const double& n, const double& d, const double& nl) {
	double a1;
	double a2;

	double offset = 0;


	if (R1 > 0 && R2 > 0) {

		a1 = -((R2 - d - R1) * (R2 - d - R1) + R1 * R1 - R2 * R2) / (2 * (R2 - d - R1));

		a2 = (R1 * R1 - R2 * R2 - (R1 + d - R2) * (R1 + d - R2)) / (2 * (R1 + d - R2));


		offset = (R1 - a1) + d/2;

		/*if (R1 < R2) {
			a2 = ((R1 - R2 + d) * (R1 - R2 + d) + R2 * R2 - R1 * R1) / (2 * (R1 - R2 + d));

			a1 = -((R1 - R2 + d) * (R1 - R2 + d) - R2 * R2 + R1 * R1) / (2 * (R1 - R2 + d));

			offset = d / 2 - R2 - a2;
		}
		else {
			a2 = ((R1 - R2 + d) * (R1 - R2 + d) + R2 * R2 - R1 * R1) / (2 * (R1 - R2 + d));

			a1 = -((R1 - R2 + d) * (R1 - R2 + d) - R2 * R2 + R1 * R1) / (2 * (R1 - R2 + d));

			offset = d / 2 - R2 - a2;

		}*/

	}
	else if (R1 < 0 && R2 < 0) {
		double R11 = -R1;
		double R22 = -R2;


		a1 = ((R22 - d - R11) * (R22 - d - R11) + R11 * R11 - R22 * R22) / (2 * (R22 - d - R11));

		a2 = -(R11 * R11 - R22 * R22 - (R11 + d - R22) * (R11 + d - R22)) / (2 * (R11 + d - R22));


		offset = (R11 + a1) + d / 2;
	}

	lenz_param = new LenzParams{ xpos, R1, R2, nl, d, a1, a2, offset };
	matrix = glm::dmat2(1.0);
	output = nullptr;
}

void ConcaveConvex::createLenzPoints() {
	double a1 = lenz_param->a1;
	double a2 = lenz_param->a2;

	std::vector<double> r_values_1;

	double signal = -1;

	if (lenz_param->R1 < 0 && lenz_param->R2 < 0) {
		signal = 1;
	}

	for (double p = -(0.50) * pi; p <= (0.50) * pi; p += (0.05) * pi) {
		for (double t = -(0.50) * pi; t <= (0.5) * pi; t += (0.05) * pi) {
			double r = a1 * std::cos(t) + signal * std::sqrt((a1 * a1) * std::cos(t) * std::cos(t) + lenz_param->R1 * lenz_param->R1 - a1 * a1);

			r_values_1.push_back((r * std::cos(t) - signal * lenz_param->offset + lenz_param->xpos));
			r_values_1.push_back(r * std::sin(t) * std::cos(p));
			r_values_1.push_back(r * std::sin(t) * std::sin(p));
			
			r_values_1.push_back(std::sin(-signal * t) * std::cos(-signal * p) / 10);//SHADE (illusion of depth)
		}
		for (double t = (0.50) * pi; t >= -(0.50) * pi; t -= (0.05) * pi) {
			double r = a2 * std::cos(t) + signal * std::sqrt((a2 * a2) * std::cos(t) * std::cos(t) + lenz_param->R2 * lenz_param->R2 - a2 * a2);

			r_values_1.push_back((r * std::cos(t) - signal * lenz_param->offset + lenz_param->xpos));
			r_values_1.push_back((r * std::sin(t)) * std::cos(p));
			r_values_1.push_back((r * std::sin(t)) * std::sin(p));

			r_values_1.push_back(std::sin(-signal * t) * std::cos(-signal * p) / 3);//SHADE (illusion of depth)

		}

	}

	
	int size_ind = r_values_1.size();


	double* p_a = new double[size_ind];

	std::copy(r_values_1.begin(), r_values_1.end(), p_a);

	unsigned int* ind = new unsigned int[(size_ind)]; // (size_ind / 4) * 4 ---- / nº values(3(x, y, z) + 1 (shade)) * points (4)
	int size = 0;
	for (unsigned int k = 0; k < 42; k++) {
		for (unsigned int i = 0; i < 42; i++) {

			*(ind + size) = i + 0 + (k) *(42);

			*(ind + size + 1) = i + (k + 1) * (42);


			size += 2;
		}
	}

	output = new points_array{ p_a, ind ,size_ind };
}


void Biconvex::createLenzPoints() {
	double a1 = lenz_param->a1;
	double a2 = lenz_param->a2;


	std::vector<double> r_values_1;

	bool sign = true;

	for (double p = -(0.50) * pi; p <= (0.50) * pi; p += (0.05) * pi) {

		for (double t = -(0.50) * pi; t <= (0.50) * pi; t += (0.05) * pi) {
			double r = -a2 * std::cos(t) + std::sqrt((a2 * a2) * std::cos(t) * std::cos(t) + lenz_param->R2 * lenz_param->R2 - a2 * a2);
			r_values_1.push_back(-r * std::cos(t) + lenz_param->offset + lenz_param->xpos);
			r_values_1.push_back(r * std::sin(t) * std::cos(p));
			r_values_1.push_back(r * std::sin(t) * std::sin(p));
			
			r_values_1.push_back(std::sin(-t) * std::cos(-p) / 10); //SHADE (illusion of depth)
		}
		sign = true;
		for (double t = -(0.50) * pi; t <= (0.5) * pi; t += (0.05) * pi) {
			double r = a1 * std::cos(t) - std::sqrt((a1 * a1) * std::cos(t) * std::cos(t) + lenz_param->R1 * lenz_param->R1 - a1 * a1);
			r_values_1.push_back(-r * std::cos(t) + lenz_param->offset + lenz_param->xpos);
			r_values_1.push_back(r * std::sin(t) * std::cos(p));
			r_values_1.push_back(r * std::sin(t) * std::sin(p));

			r_values_1.push_back(std::sin(t) * std::cos(p) / 3); //SHADE (illusion of depth)
		}
	}

	int size_r = r_values_1.size();

	double* p_a = new double[size_r];
	std::copy(r_values_1.begin(), r_values_1.end(), p_a);

	unsigned int* ind = new unsigned int[size_r];
	int size = 0;
	for (unsigned int k = 0; k < 42; k++) {
		for (unsigned int i = 0; i < 42; i++) {

			*(ind + size) = i + 0 + (k) * 42;

			*(ind + size + 1) = i + (k + 1) * 42;


			size += 2;
		}
	}

	output = new points_array{ p_a, ind , size_r };
}

void Biconcave::initializeLenz(const double& xpos, const double& R1, const double& R2, const double& n, const double& d, const double& nl) {
	lenz_param = new LenzParams{ xpos, R1, R2, nl, d, d/2, d/2, 0};
	matrix = glm::dmat2(1.0);
}
void Biconcave::createLenzPoints() {
	double a1 = -lenz_param->R1 + 3*lenz_param->d/4;
	double a2 = lenz_param->R2 - 3*lenz_param->d/4;

	std::vector<double> r_values_1;

	for (double p = -(0.50) * pi; p <= (0.50) * pi; p += (0.05) * pi) {
		for (double t = -(0.50) * pi; t <= (0.5) * pi; t += (0.05) * pi) {
			double r = a1 * std::cos(t) + std::sqrt((a1 * a1) * std::cos(t) * std::cos(t) + lenz_param->R1 * lenz_param->R1 - a1 * a1);

			r_values_1.push_back((r * std::cos(t) - (lenz_param->R1 + a1) - lenz_param->d / 2));
			r_values_1.push_back(r * std::sin(t) * std::cos(p));
			r_values_1.push_back(r * std::sin(t) * std::sin(p));

			r_values_1.push_back(std::sin(-t) * std::cos(-p) / 10); //SHADE (illusion of depth)
		}
		for (double t = -(0.50) * pi; t <= (0.50) * pi; t += (0.05) * pi) {
			double r = a2 * std::cos(t) - std::sqrt((a2 * a2) * std::cos(t) * std::cos(t) + lenz_param->R2 * lenz_param->R2 - a2 * a2);

			r_values_1.push_back((r * std::cos(t) + (lenz_param->R2 - a2) + lenz_param->d / 2));
			r_values_1.push_back((r * std::sin(t)) * std::cos(p));
			r_values_1.push_back((r * std::sin(t)) * std::sin(p));

			r_values_1.push_back(std::sin(-t) * std::cos(-p) / 3); //SHADE (illusion of depth)
		}
	}

	int size_1 = r_values_1.size();

	double* p_a = new double[size_1];
	std::copy(r_values_1.begin(), r_values_1.end(), p_a);

	unsigned int* ind = new unsigned int[size_1];
	int size = 0;
	for (unsigned int k = 0; k < 42; k++) {
		for (unsigned int i = 0; i < 42; i++) {

			*(ind + size) = i + 0 + (k) * 42;

			*(ind + size + 1) = i + (k + 1) * 42;


			size += 2;
		}
	}



	std::cout << size << std::endl;

	output = new points_array{ p_a, ind ,size_1 };
}





void Lenz::createLenzMatrix(const double& n) {
	glm::mat2 R2 = createRMatrix(lenz_param->R2, lenz_param->n, n);
	glm::mat2 R1 = createRMatrix(lenz_param->R1, n, lenz_param->n);
	glm::mat2 T21 = createTMatrix(lenz_param->d, lenz_param->n);
	matrix = R2 * T21 * R1;
}