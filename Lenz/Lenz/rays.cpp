#include "rays.h"


rays::rays() {

}

rays::~rays() {

}

void rays::initialize_rays(rays_pack* rP) {

	rP->r.reserve(20 * 20);

	rP->xpos.reserve(20 * 20);
	rP->pY.reserve(20 * 20);
	rP->pZ.reserve(20 * 20);

	for (double z = -1; z < 1; z += 0.1) {
		for (double y = -1; y < 1; y += 0.1) {
			rP->xpos.push_back(-1);
			rP->r.push_back(sqrt(z*z + y*y));
			rP->pY.push_back(y);
			rP->pZ.push_back(z);
		}
	}
}