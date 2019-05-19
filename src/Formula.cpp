#include "Formula.h"
#include<iostream>

Formula::Formula(int bit)
{
	this->bit = bit;
}

void Formula::change(int bit, char value)
{
	if (bit < 0 || bit >= this->bit) {
		std::cout << "invade input(Formula::change)" << std::endl;
		return;
	}
	this->value[bit] = value;
}

char Formula::get(int bit)
{
	if (bit < 0 || bit >= this->bit) {
		std::cout << "invade input(Formula::get)" << std::endl;
		return 's';
	}
	return this->value[bit];
}

unsigned Formula::get()
{
	unsigned sum = 0;
	for (int i = 0; i < bit; ++i) {
		if (this->value[i] == _X_SIGN) {
			return NOT_A_MIN;
		}
		sum += (this->value[i] - '0') << i;
	}
	return sum;
}

Term::Term(int bit, unsigned value):Formula(bit)
{
	for (int i = 0; i < bit; value >>= 1, i++) {
		if (value % 2) {
			this->value[i] = _1_SIGN;
		}
		else {
			this->value[i] = _0_SIGN;
		}
	}
	this->mark = _No_Mark;
}

Term::Term(int bit, Term & temp, int position):Formula(bit)
{
	for (int i = 0; i < bit; ++i) {
		this->value[i] = temp.value[i];
	}
	if (position >= 0) {
		this->value[position] = _X_SIGN;
	}
	this->mark = _No_Mark;
}

void Term::show()
{
	for (int i = bit - 1; i >= 0; --i) {
		std::cout << value[i];
	}
	std::cout << " " << mark << std::endl;
}

int Term::searchFor(Term target)
{
	int differentCount = 0;
	for (int i = 0; i < target.bit; ++i) {
		if (this->value[i] != target.value[i]) {
			differentCount++;
		}
	}
	if (differentCount == 1) {
		for (int i = 0; i < this->bit; ++i) {
			if (this->value[i] != target.value[i]) {
				return i;
			}
		}
	}
	else {
		return -1;
	}
}

bool Term::givetruth(unsigned v)
{
	for (int i = 0; i < bit; ++i, v >>= 1) {
		if (this->value[i] == _X_SIGN) {
			continue;
		}
		else if (this->value[i]-'0' == v % 2) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}

void Term::setMark(char m)
{
	this->mark = m;
}

char Term::giveMark()
{
	return this->mark;
}

Term Term::operator=(Term & a)
{
	this->bit = a.bit;
	for (int i = 0; i < this->bit; ++i) {
		this->value[i] = a.value[i];
	}
	this->mark = a.mark;
	return *this;
}

bool Term::operator==(Term&a)
{
	bool ret = true;
	if (this->bit != a.bit) {
		return false;
	}
	for (int i = 0; i < this->bit; ++i) {
		if (this->value[i] != a.value[i]) {
			return false;
		}
	}
	return ret;
}
