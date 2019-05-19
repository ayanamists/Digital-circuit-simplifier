#include "generator.h"
#include<iostream>
#include<stdexcept>

Generator::Generator(char U_D , int* number_sequence = NULL, int count = 0)
{
	for (int i = 0; i < MAX_BIT; ++i) {
		this->J[i] = NULL;
		this->K[i] = NULL;
	}
	if (((U_D != u_and_down_UP_AS_1) && (U_D != u_and_d_UP_AS_0)
	 && (U_D != UP)) || count <= 0) {
		throw std::invalid_argument("In Gernerator::Generator, line:13");
	}
	this->U_D = U_D;
	int temp = number_sequence[0];
	if (U_D == UP) {
		this->count = count;
		for (int i = 0; i < count; ++i) {
			this->number_sequence[i] = number_sequence[i];
			if (temp < number_sequence[i]) {
				temp = number_sequence[i];
			}
		}
	}
	else {
		this->count = count;
		for (int i = 0; i < count; ++i) {
			this->number_sequence[i] = (U_D == u_and_d_UP_AS_0)? 
				number_sequence[i] << 1: (number_sequence[i] << 1) + 1;
			if (temp < number_sequence[i]) {
				temp = number_sequence[i];
			}
		}
		this->number_sequence[count] = (U_D == u_and_d_UP_AS_0) ?
			this->number_sequence[0] + 1 : this->number_sequence[0] - 1;
		for (int i = count + 1; i < count * 2; ++i) {
			this->number_sequence[i] = (U_D == u_and_d_UP_AS_0)?
				this->number_sequence[count * 2 - i] + 1:this->number_sequence[count * 2 - i] - 1;
		}
	}
	this->bit = 0;
	for (int i = 0; temp > 0; ++i) {
		bit++;
		temp >>= 1;
	}
	if (U_D == UP) {
		for (int i = 0; i < bit; ++i) {
			this->J[i] = new Graph(1 << bit);
			this->K[i] = new Graph(1 << bit);
		}
	}
	else {
		for (int i = 0; i < bit; ++i) {
			this->J[i] = new Graph(1 << (bit + 1));
			this->K[i] = new Graph(1 << (bit + 1));
		}
	}
}

Generator::~Generator()
{
	for (int i = 0; i < bit; ++i) {
		delete this->J[i];
		delete this->K[i];
	}
}

bool Generator::Generate()
{
	if (this->U_D == UP) {
		for (int i = 0; i < count; ++i) {
			for (int j = 0; j < bit; ++j) {
				unsigned check = 1 << j;
				unsigned thisNunberBit = (this->number_sequence[i] & check);
				unsigned nextNumberBit = this->number_sequence[(i == count - 1) ? 0 : i + 1] & check;
				if (thisNunberBit == check && nextNumberBit == check) {
					J[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _0_SIGN);
				}
				if (thisNunberBit == 0 && nextNumberBit == 0) {
					J[j]->setTempGraph(this->number_sequence[i], _0_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
				}
				if (thisNunberBit == 0 && nextNumberBit == check) {
					J[j]->setTempGraph(this->number_sequence[i], _1_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
				}
				if (thisNunberBit == check && nextNumberBit == 0) {
					J[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _1_SIGN);
				}
			}
		}
	}
	else {
		for (int i = 0; i < count; ++i) {
			for (int j = 0; j < bit; ++j) {
				unsigned check = 1 << (j + 1);
				unsigned thisNunberBit = (this->number_sequence[i] & check);
				unsigned nextNumberBit = this->number_sequence[(i == count - 1) ? 0 : i + 1] & check;
				if (thisNunberBit == check && nextNumberBit == check) {
					J[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _0_SIGN);
				}
				if (thisNunberBit == 0 && nextNumberBit == 0) {
					J[j]->setTempGraph(this->number_sequence[i], _0_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
				}
				if (thisNunberBit == 0 && nextNumberBit == check) {
					J[j]->setTempGraph(this->number_sequence[i], _1_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
				}
				if (thisNunberBit == check && nextNumberBit == 0) {
					J[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _1_SIGN);
				}
			}
		}
		for (int i = count; i < count * 2; ++i) {
			for (int j = 0; j < bit; ++j) {
				unsigned check = 1 << (j + 1);
				unsigned thisNunberBit = this->number_sequence[i] & check;
				unsigned nextNumberBit = this->number_sequence[(i == count * 2 - 1) ? count : i + 1] & check;
				if (thisNunberBit == check && nextNumberBit == check) {
					J[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _0_SIGN);
				}
				if (thisNunberBit == 0 && nextNumberBit == 0) {
					J[j]->setTempGraph(this->number_sequence[i], _0_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
				}
				if (thisNunberBit == 0 && nextNumberBit == check) {
					J[j]->setTempGraph(this->number_sequence[i], _1_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
				}
				if (thisNunberBit == check && nextNumberBit == 0) {
					J[j]->setTempGraph(this->number_sequence[i], _X_SIGN);
					K[j]->setTempGraph(this->number_sequence[i], _1_SIGN);
				}
			}
		}
	}
	for (int i = 0; i < bit; ++i) {
		if (!J[i]->makeGraph()) {
			return false;
		};
		if (!K[i]->makeGraph()) {
			return false;
		};
	}
	return true;
}

void Generator::show()
{
	for (int i = 0; i < this->bit; ++i) {
		printf("for Q_%d_J:\n", i);
		this->J[i]->show();
		printf("for Q_%d_K:\n", i);
		this->K[i]->show();
	}
}
