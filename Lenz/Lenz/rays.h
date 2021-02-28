#pragma once
#include <vector>
class rays
{

	struct rays_pack {
		std::vector<double> pY;
		std::vector<double> pZ;
		std::vector<double> xpos;
		std::vector<double> r; //distance to xAxis
	};

	public:
		rays();
		~rays();


	public:
		void initialize_rays(rays_pack*);


};

