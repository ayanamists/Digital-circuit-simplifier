#pragma once
#include<iostream>
#include"Graph.h"
#define MAX_NUMBER_ 8
#define NOT_A_MIN 0
#define _No_Mark 'q'
#define _Pair_Mark 'r'
#define _Esstinal_Prime_Mark 'e'
#define _Prime_Mark 'p'
#define _Do_Not_Use_Mark 'n'
#define _Do_Not_Care_Mark 'j'

class Formula {
public:
	Formula(int bit);
	void change(int bit, char value);
	char get(int bit);
	unsigned get();
protected:
	unsigned bit;
	char value[MAX_NUMBER_];
	/*
		this array is used to describle the implicant.
		for example, 01x is C'B and 011 is C'BA
	*/
};

class Term : public Formula {
//Term is a Formula with truth value and mark.
public:
	Term(int, unsigned);
	Term(int, Term&, int j = -1);
	void show();
	int searchFor(Term target);
	bool givetruth(unsigned);
	void setMark(char);
	char giveMark();
	Term operator =(Term&);
	bool operator ==(Term&);
private:
	char mark;
};