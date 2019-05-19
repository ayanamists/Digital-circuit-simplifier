/*
	Version 0.1, by Chenxi Li, 2019
	This version is not so fast, cause I use vector instead of array,
	however, it's more stable.
	If you want to learn something about this algorithm, 
	you can download that File:
	http://www.cs.columbia.edu/~cs6861/handouts/quine-mccluskey-handout.pdf
*/


#pragma once
#include"Graph.h"
#include"Formula.h"
#include<vector>
#define Random 
#define MAX_TERM 1024
#define _Not_Use NULL
#define _OCUP_SIGN 'X'
#define _NO_OTHER_INFO 0x0
#define _SHOW_DEBUG_INFO 0x1
#define _SHOW_RESULT 0x2
#define MAX_TEMP 1024

class SimplifyEngine {
public:
	SimplifyEngine();
	//Creat an random graph
	SimplifyEngine(Graph&, char otherInfo);
	//Use it to simplify a Graph
	~SimplifyEngine();
	bool simplify();
	//the mainly simplify is done in this function
	void show(); 
private:
	std::vector<Term*> minterms;
	std::vector<Term*> primeImplicant;
	std::vector<Term*> essentialPrimeImplicant;
	std::vector<std::vector<char> > primeImplicantTable;
	unsigned bit;
	bool ifShowTemp;
	bool ifShowResult;
	bool gerneratePrimeImplicantTable();
	bool petrickMethod();
	void showtable();
	void showTempArray(std::vector<Term*>);
	void reduce_by_Only_One();
};

class PetrickArray {
public:
	PetrickArray(char k) {
		count = 0;
		array[count++] = k;
	}
	PetrickArray(char k, PetrickArray p) {
		this->count = p.count;
		for (int i = 0; i < count; ++i) {
			this->array[i] = p.array[i];
		}
		this->add(k);
	}
	void add(char k) {
		for (int i = 0; i < this->count; ++i) {
			if (k == array[i]) {
				return;
			}
		}
		array[count++] = k;
	}
	int giveCount() {
		return count;
	}
	char give(int i) {
		return array[i];
	}
private:
	char array[8];
	int count;
};
