#pragma once
#include<iostream>
#include<vector>
#define _0_SIGN '0'
#define _1_SIGN '1'
#define _X_SIGN 'x'
#define MAX_GRAPH 8
#define MAX_ELEMENT 256


class Graph {
public:
	Graph(int count);
	~Graph();
	bool makeGraph();
	void setTempGraph(int,char);
	void show();
	int getbit();
	std::vector<int> must_0;
	std::vector<int> must_1;
	std::vector<int> do_not_care;
private:
	int bit;
	std::vector<char> tempGraph;
};