#include "Simplify engine.h"
#include<iostream>
#include<vector>

#define GetOtherInfo(otherinfo) \
if (otherinfo & _SHOW_DEBUG_INFO) { \
	this->ifShowTemp = true; \
} \
else { \
	this->ifShowTemp = false; \
}\
if(otherinfo & _SHOW_RESULT){\
	this->ifShowResult = true; \
}\
else{\
	this->ifShowResult = false; \
}



char petrickTable[MAX_TERM][MAX_TERM] = { 0 };
int petrickCountRow = 0;
char minSelect[MAX_TERM] = { 0 };
int minCount = 0;

SimplifyEngine::SimplifyEngine()
{
	ifShowTemp = false;
	bit = 4;
}

SimplifyEngine::SimplifyEngine(Graph& g, char otherInfo):primeImplicantTable(MAX_TERM)
{
	GetOtherInfo(otherInfo)
	for (int i = 0; i < primeImplicantTable.size(); ++i) {
		primeImplicantTable[i].resize(MAX_TERM);
	}
	int s = g.getbit();
	this->bit = 0;
	for (; s > 0; s >>= 1) {
		this->bit++;
	}
	this->bit--;
	for (int i = 0; i < g.must_1.size(); ++i) {
		this->minterms.push_back(new Term(this->bit, g.must_1[i]));
	}
	for (int i = 0; i < g.do_not_care.size(); ++i) {
		this->minterms.push_back(new Term(this->bit, g.do_not_care[i]));
		this->minterms.back()->setMark(_Do_Not_Care_Mark);
	}
}

SimplifyEngine::~SimplifyEngine()
{
	for (int i = 0; i < minterms.size(); ++i) {
		if (this->minterms[i] != NULL) {
			delete this->minterms[i];
		}
	}
	for (int i = 0; i < primeImplicant.size(); ++i) {
		if (this->primeImplicant[i] != NULL) {
			delete this->primeImplicant[i];
		}
	}
	for (int i = 0; i < essentialPrimeImplicant.size(); ++i) {
		if (this->essentialPrimeImplicant[i] != NULL) {
			delete this->essentialPrimeImplicant[i];
		}
	}
}

bool SimplifyEngine::simplify()
{

	std::vector<Term*> tempArray(this->minterms);
	std::vector<Term*> tempArray2;
	int count = 0;
	int circle = 0;

//begin : find all PrimeImplicants.

	while (tempArray.size())
	{
		if (this->ifShowTemp == true) {
			printf("circle times:%d\n", circle++);
		}
		else {
			circle++;
		}
		for (int i = 0; i < tempArray.size(); ++i) {
			bool get = false;

			//First, search for all minterms that can pair with 
			//the i minterm

			for (int j = i + 1; j < tempArray.size(); ++j) {
				int search = tempArray[j]->searchFor(*tempArray[i]);
				if (search == -1) {
					continue;
				}
				else {
					if (tempArray[i]->giveMark() == _No_Mark) {
						tempArray[i]->setMark(_Pair_Mark);
					}
					if(tempArray[j]->giveMark() == _No_Mark) {
						tempArray[j]->setMark(_Pair_Mark);
					}
					tempArray2.push_back(new Term(this->bit, (*tempArray[i]), search));
					get = true;

					//_Do_Not_Care_Mark should be inherited,
					//for we'll not include them in the primeImplicantTable

					if (tempArray[i]->giveMark() == _Do_Not_Care_Mark
						&& tempArray[j]->giveMark() == _Do_Not_Care_Mark) {
						tempArray2.back()->setMark(_Do_Not_Care_Mark);
					}

					//we don't allow repition in the minterms

					bool if_have_another = false;
					for (int i = 0; i < tempArray2.size() - 1; ++i) {
						if (*tempArray2.back() == *tempArray2[i]) {
							if_have_another = true;
							break;
						}
					}
					if (if_have_another == true) {
						delete tempArray2.back();
						tempArray2.pop_back();
					}
				}
			}
		}
		this->showTempArray(tempArray);
		for (int i = 0; i < tempArray.size(); ++i) {
			if (tempArray[i]->giveMark() == _No_Mark) {
				this->primeImplicant.push_back(new Term(this->bit, *tempArray[i]));
			}
			if (circle != 1) { delete tempArray[i]; }
			tempArray[i] = NULL;
		}
		
		tempArray.swap(tempArray2);
		tempArray2.clear();
	}
	this->gerneratePrimeImplicantTable();
	int tempcount = 0;
	int firsthit = 0;
	this->showtable();

//first reduce

	this->reduce_by_Only_One();
	this->showtable();

//second reduce1: row donimate

	std::vector<int> tempLabelArray;
	std::vector<int> tempLabelArray2;
	for (int i = 0; i < this->minterms.size(); ++i) {

		//first to find all 'X' in this row

		for (int j = 0; j < this->primeImplicant.size(); ++j) {
			if (this->primeImplicantTable[i][j] == _OCUP_SIGN) {
				tempLabelArray.push_back(j);
			}
		}
		if (tempLabelArray.size() == 0) {
			continue;
		}

		/*
		if another row contains all 'X' that i row contains, 
		then clear that 'another row'
		*/

		for (int j = 0; j < this->minterms.size(); ++j) {
			if (j == i) {
				continue;
			}
			else {
				for (int k = 0; k < this->primeImplicant.size(); ++k) {
					if (this->primeImplicantTable[j][k] == _OCUP_SIGN) {
						tempLabelArray2.push_back(k);
					}
				}
			}
			if (tempLabelArray2.size() == 0) {
				continue;
			}
			bool allFind = true;
			bool find[MAX_TERM] = { false };
			for (int k = 0; k < tempLabelArray.size(); ++k) {
				for (int l = 0; l < tempLabelArray2.size(); ++l) {
					if (tempLabelArray2[l] == tempLabelArray[k]) {
						find[k] = true;
					}
				}
			}
			for (int k = 0; k < tempLabelArray.size(); ++k) {
				if (find[k] == false) {
					allFind = false;
					break;
				}
			}
			if (allFind == true) {
				for (int k = 0; k < this->primeImplicant.size(); ++k) {
					this->primeImplicantTable[j][k] = 0;
				}
			}
			tempLabelArray2.clear();
		}
		tempLabelArray.clear();
	}
	this->showtable();

//second reduce2:colomn reduce, it's the same with the past step

	for (int i = 0; i < this->primeImplicant.size(); ++i) {
		if (this->primeImplicant[i]->giveMark() == _Esstinal_Prime_Mark) {
			continue;
		}
		for (int j = 0; j < this->minterms.size(); ++j) {
			if (this->primeImplicantTable[j][i] == _OCUP_SIGN) {
				tempLabelArray.push_back(j);
			}
		}
		for (int j = 0; j < this->primeImplicant.size(); ++j) {
			if (j == i ||
				this->primeImplicant[j]->giveMark() == _Esstinal_Prime_Mark) {
				continue;
			}
			bool allFind = false;
			for (int k = 0; k < this->minterms.size(); ++k) {
				if (this->primeImplicantTable[k][j] != _OCUP_SIGN) {
					continue;
				}
				for (int l = 0; l < tempLabelArray.size(); ++l) {
					if (k != tempLabelArray[l]) {
						allFind = false;
						continue;
					}
					else {
						allFind = true;
						break;
					}
				}
				if (allFind == false) {
					break;
				}
			}
			if (allFind == false) {
				continue;
			}
			else {
				for (int k = 0; k < this->minterms.size(); ++k) {
					this->primeImplicantTable[k][j] = 0;
				}
				this->primeImplicant[j]->setMark(_Do_Not_Use_Mark);
			}
		}
		tempLabelArray.clear();
		if (this->ifShowTemp == true) {
			printf("%d\n", i);
		}
		this->showtable();
	}
	this->showtable();

//do as first again

	this->reduce_by_Only_One();
	this->showtable();

//Pertick Method

	bool needPM = false;
	for (int i = 0; i < this->primeImplicant.size(); ++i) {
		if (this->primeImplicant[i]->giveMark() == _Do_Not_Use_Mark ||
			this->primeImplicant[i]->giveMark() == _Esstinal_Prime_Mark) {
			continue;
		}
		else {
			needPM = true;
			break;
		}
	}
	if (needPM == false) {
		this->show();
		return true;
	}
	else {
		this->petrickMethod();
	}
	this->show();
	return true;
}

void SimplifyEngine::show()
{
	if (this->ifShowResult == false) {
		return;
	}
	for (int i = 0; i < this->essentialPrimeImplicant.size(); ++i) {
		this->essentialPrimeImplicant[i]->show();
	}
}

bool SimplifyEngine::gerneratePrimeImplicantTable()
{
	for (int i = 0; i < minterms.size(); ++i) {
		if (this->minterms[i]->giveMark() == _Do_Not_Care_Mark) {
			delete this->minterms[i];
			this->minterms[i] = NULL;
		}
	}
	std::vector<Term*> temp(this->minterms);
	this->minterms.clear();
	for (int i = 0; i < temp.size(); ++i) {
		if (temp[i] != NULL) {
			this->minterms.push_back(temp[i]);
		}
	}
	for (int i = 0; i < this->minterms.size(); ++i) {
		for (int j = 0; j < this->primeImplicant.size(); ++j) {
			if (this->primeImplicant[j]->
				givetruth(this->minterms[i]->get())) {
				this->primeImplicantTable[i][j] = _OCUP_SIGN;
			}
		}
	}
	return true;
}

bool SimplifyEngine::petrickMethod()
{
	//use the last Charcter of column as count;
	//First Gernerate petrickTable
	for (int i = 0; i < this->minterms.size(); ++i) {
		bool ifEmpty = true;
		for (int j = 0; j < this->primeImplicant.size(); ++j) {
			if (this->primeImplicantTable[i][j] == _OCUP_SIGN) {
				ifEmpty = false;
				break;
			}
		}
		if (ifEmpty == true) {
			continue;
		}
		else {
			petrickCountRow++;
			petrickTable[petrickCountRow - 1][MAX_TERM - 1] = 0;
			for (int j = 0; j < this->primeImplicant.size(); ++j) {
				if (this->primeImplicantTable[i][j] == 'X') {
					int petrickCountColumn = petrickTable[petrickCountRow - 1][MAX_TERM - 1];
					petrickTable[petrickCountRow - 1][petrickCountColumn] = j;
					petrickTable[petrickCountRow - 1][MAX_TERM - 1]++;
				}
			}
		}
	}

	/*
		Then, use the (P_1+P_2)(P_3+P_4) = P_1P_3 + P_1P_4 + P_2P_3 + P_2P_4
		to gernerate all possible terms we can use
	*/

	PetrickArray* temp[MAX_TEMP] = { NULL };
	PetrickArray* temp2[MAX_TEMP] = { NULL };
	int tempCount = 0;
	int tempCount2 = 0;
	for (int i = 0; i < petrickTable[0][MAX_TERM - 1]; ++i) {
		temp[tempCount++] = new PetrickArray(petrickTable[0][i]);
	}
	for (int i = 1; i < petrickCountRow; ++i) {
		for (int j = 0; j < tempCount; ++j) {
			for (int k = 0; k < petrickTable[i][MAX_TERM - 1]; ++k) {
				temp2[tempCount2++] = new PetrickArray(petrickTable[i][k], *temp[j]);
			}
			delete temp[j];
			temp[j] = NULL;
		}
		tempCount = 0;
		for (int j = 0; j < tempCount2; ++j) {
			temp[j] = temp2[j];
			temp2[j] = NULL;
			tempCount++;
		}
		tempCount2 = 0;
	}

	//Finally, search for the smallest, that is the anwser

	int minCount = temp[0]->giveCount();
	for (int i = 0; i < tempCount; ++i) {
		if (temp[i]->giveCount() < minCount) {
			minCount = temp[i]->giveCount();
		}
	}
	PetrickArray* min = NULL;
	for (int i = 0; i < tempCount; ++i) {
		if (temp[i]->giveCount() == minCount) {
			min = temp[i];
			break;
		}
	}
	for (int i = 0; i < min->giveCount(); ++i) {
		this->essentialPrimeImplicant.push_back(
			new Term(this->bit,*(this->primeImplicant[min->give(i)])));
		this->essentialPrimeImplicant.back()->setMark(_Esstinal_Prime_Mark);
	}
	for (int i = 0; i < tempCount; ++i) {
		delete temp[i];
	}
	return true;
}

void SimplifyEngine::showtable()
{
	if (this->ifShowTemp == false) {
		return;
	}
	for (int i = 0; i < this->minterms.size(); ++i) {
		for (int j = 0; j < this->primeImplicant.size(); ++j) {
			std::cout << this->primeImplicantTable[i][j] << " ";
		}
		std::cout << "\n";
	}
}

void SimplifyEngine::reduce_by_Only_One()
{
	int tempcount = 0;
	int firsthit = 0;
	
	//First, to find the essentials.
	for (int i = 0; i < this->minterms.size(); i++) {
		if (this->minterms[i] == NULL) {
			continue;
		}
		for (int j = 0; j < this->primeImplicant.size(); ++j) {
			if (this->primeImplicantTable[i][j] == _OCUP_SIGN) {
				if (tempcount == 0) {
					firsthit = j;
					tempcount++;
				}
				else {
					tempcount++;
				}

			}
		}
		if (tempcount == 1) {
			this->primeImplicant[firsthit]->setMark(_Esstinal_Prime_Mark);
		}
		tempcount = 0;

		/*

		if one colomn is essential, we clear all rows that colomn contains
		in this stage we also check the if the essentials we find is already in the
		essentialImpilcant.

		*/
		for (int i = 0; i < this->primeImplicant.size(); ++i) {
			if (this->primeImplicant[i]->giveMark() == _Esstinal_Prime_Mark) {
				bool ifHave = false;
				for (int j = 0; j < this->minterms.size(); ++j) {
					if (this->primeImplicantTable[j][i] == _OCUP_SIGN) {
						for (int k = 0; k < this->primeImplicant.size(); ++k) {
							this->primeImplicantTable[j][k] = '\x00';
						}
					}
				}
				for (int j = 0; j < this->essentialPrimeImplicant.size(); ++j) {
					if (*(this->essentialPrimeImplicant[j]) ==
						*(this->primeImplicant[i])) {
						ifHave = true;
						break;
					}
				}
				if (ifHave) {
					continue;
				}
				this->essentialPrimeImplicant.push_back(
					new Term(this->bit, *(this->primeImplicant[firsthit])));
			}
		}
	}
	this->showtable();
	
	//Finally, we check if the PTable is empty

	bool allEmpty = true;
	for (int i = 0; i < this->minterms.size(); ++i) {
		for (int j = 0; j < this->primeImplicant.size(); ++j) {
			if (this->primeImplicantTable[i][j] == _OCUP_SIGN) {
				allEmpty = false;
				break;
			}
		}
	}
	if (allEmpty == true) {
		for (int i = 0; i < this->primeImplicant.size(); ++i) {
			if (this->primeImplicant[i]->giveMark() != _Esstinal_Prime_Mark) {
				this->primeImplicant[i]->setMark(_Do_Not_Use_Mark);
			}
		}
	}
}

void SimplifyEngine::showTempArray(std::vector<Term*> tempArray){
	if (ifShowTemp == false) {
		return;
	}
	printf("array1\n");
	for (int i = 0; i < tempArray.size(); ++i) {
		tempArray[i]->show();
	}

}
