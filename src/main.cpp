#include<iostream>
#include"generator.h"
#include"Simplify engine.h"

int main() {
	int a[8] = {101 ,2,3,4,5,0,8,7};
	try {
		Generator G(u_and_d_UP_AS_0, a, 8);
		G.Generate();
		G.show();
		for (int i = 0; i < 4; ++i) {
			std::cout << "J_" << i << std::endl;
			SimplifyEngine* s = new SimplifyEngine(*(G.J[i]), _SHOW_RESULT);
			SimplifyEngine* s2 = new SimplifyEngine(*(G.K[i]), _SHOW_RESULT);
			s->simplify();
			std::cout << "K_" << i << std::endl;
			s2->simplify();
			delete s;
			delete s2;
		}
	}
	catch(std::invalid_argument a) {
		std::cout << a.what() << std::endl;
	}
	catch (...) {}
}
