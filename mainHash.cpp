/*
 * mainHash.cpp
 *
 *  Created on: May 4, 2020
 *      Author: 13027
 */


#include <iostream>
#include "makeSeuss.hpp"
#include <time.h>
#include <stdlib.h>

using namespace std;
int main() {
	srand(time(NULL));
	makeSeuss k("DrSeuss.txt","Seussout.txt",true,true);
	makeSeuss m("GEChap1a.txt","GEChap1out.txt",false,true);

	makeSeuss n("DrSeuss.txt","Seussout2.txt",true,false);
	makeSeuss v("GEChap1a.txt","GEChap1out2.txt",false,false);
	makeSeuss xd("big.txt","bigOUT.txt",false,false);
	return 0;
}
