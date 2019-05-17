#pragma once
#include<iostream>
#include"Graph.h"
#define UP 'U'
#define u_and_down_UP_AS_1 'D'
#define u_and_d_UP_AS_0 'd'
#define MAX_NUMBER 1000
#define MAX_BIT 10

class Generator {
public:
	Generator(char U_D, int* number_sequence, int count);
	~Generator();
	bool Generate();
	void show();
	Graph* J[MAX_BIT];
	Graph* K[MAX_BIT];
private:
	char U_D;
	unsigned number_sequence[MAX_NUMBER];
	int count;
	unsigned bit;
};