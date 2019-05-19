#include "Graph.h"
#include<iostream>

void myitoa(int num, char* str)
{
	int a, i = 0, j = 0;
	while (num > 0)
	{
		a = num % 2;
		str[j++] = a + '0';     

		num /= 2;
	}

	while (j > 0)
	{
		str[i++] = str[--j];     
	}

	str[i] = '\0';               
}

Graph::Graph(int count):tempGraph(MAX_ELEMENT,'x')
{
	bit = count;
}

Graph::~Graph()
{
}

bool Graph::makeGraph()
{
	for (int i = 0; i < bit; ++i) {
		if (this->tempGraph[i] == _0_SIGN) {
			this->must_0.push_back(i);
		}
		if (this->tempGraph[i] == _1_SIGN) {
			this->must_1.push_back(i);
		}
		if (this->tempGraph[i] == _X_SIGN) {
			this->do_not_care.push_back(i);
		}
	}
	return true;
}

void Graph::setTempGraph(int locate, char t)
{
	if (t != _0_SIGN && t != _1_SIGN && t != _X_SIGN) {
		throw std::invalid_argument("In Graph::setTempGraph, line 31");
	}
	this->tempGraph[locate] = t;
}

void Graph::show()
{
	printf("Must be zero:\n");
	char temp[20] = { 0 };
	for (int i = 0; i < this->must_0.size(); ++i){
#ifdef _WINSDOWS_
		_itoa_s(this->must_0[i], temp, 2);
#else
		myitoa(this->must_0[i], temp);
#endif

		printf("%04s\n", temp);
	}
	printf("Must be one:\n");
	for (int i = 0; i < this->must_1.size(); ++i) {
#ifdef _WINDOWS_
		_itoa_s(this->must_1[i], temp, 2);
#else
		myitoa(this->must_1[i], temp);
#endif 
		printf("%04s\n", temp);
	}
	printf("We don't care:\n");
	for (int i = 0; i < this->do_not_care.size(); ++i) {
#ifdef _WINDOWS_
		_itoa_s(this->do_not_care[i], temp, 2);
#else
		myitoa(this->do_not_care[i], temp);
#endif
		printf("%04s\n", temp);
	}
}

int Graph::getbit()
{
	return this->bit;
}

