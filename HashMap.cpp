/*
 * HashMap.cpp
 *
 *  Created on: May 4, 2020
 *      Author: 13027
 * Joseph Loporto Hashmap assignment
 */


#include "hashMap.hpp"
#include "hashNode.hpp"
#include <iostream>
#include <math.h>
using namespace std;

hashMap::hashMap(bool hash1, bool coll1) {
	primedex = 15;
	mapSize = kprimes[primedex];
	map = new hashNode*[mapSize];
	for(int i = 0;i<mapSize;i++){map[i] = NULL;} // Init array to null
	hashfn = hash1;
	collfn = coll1;
	numKeys = 0;
	collisions = 0;
	hashcoll = 0;
}
void hashMap::addKeyValue(string k, string v) {
	//if((numKeys*10) >= (mapSize*7)){ // if map is over 70% full rehash
		//reHash(); // A bit redundant but just following the instructions
	//}

	if(findKey(k) >= 0){
		map[findKey(k)]->addValue(v);
	}else{
		numKeys++;
		int index = getIndex(k);
		map[index] = new hashNode(k,v);
	}

	return;
}
int hashMap::getIndex(string k) {
	if((numKeys*10) >= (mapSize*7)){ // if map is over 70% full rehash
		reHash();
	}
	int curIndex;
	if(hashfn){
		curIndex = abs(calcHash1(k)) % mapSize;
	}else{
		curIndex = abs(calcHash2(k)) % mapSize;
	}

	if(map[curIndex] == NULL){
		return curIndex;
	}
	hashcoll++;
	if(collfn){
		return coll1(0,curIndex,k);
	}else{
		return coll2(0,curIndex,k);
	}


}

int hashMap::calcHash1(string k){
	// First, my fun playground hashing algorithm using the modified inital states of SHA-512 as a starting point (Ive definitely butchered them past functionality).
	// Which word is used depends on the length of the string and the last symbol. Since alot of words start the same but words that-
	// end the same usually vary in content signifigantly.
	// After the initial state is calculated, it will be modified by subtracting the prime at kprimes[k.at(i)] if i is odd, and adding if i is even.
	const int states[8] = {0x03bcc908,
	0x4caa73b,
	0xe94f82b,
	0x5f1d36f1,
	0x4de682d1,
	0x2b3e6c1f,
	0x141bd6b,
	0x37e2179
	};
	int state = states[(k.at(k.length()-1) * k.length()) % 8]; // Find state word to use based on length of string and last character

	for(int i = 0;i < k.length();i++){
		if(i % 2){
			state += kprimes[k.at(i)]; 
		}else{
			state -= kprimes[k.at(i)];
		}
	}
	return state;
}

int hashMap::calcHash2(string k){
	// Another playground algoritms this time using multiplication and division
	// of unsigned ints which will be cast to an int. Why? for fun and to see if one state does anything cool
	unsigned int state = 0xDEADBEEF;

	for(int i = 0;i < k.length();i++){
		if(i % 2){
			state *= kprimes[k.at(i)];
		}else{
			state /= kprimes[k.at(i)];
		}
	}
	return (int) state;
}



void hashMap::getClosestPrime() {
	// Sets mapSize to the prime closest to double itself
	// Has a hard cap after the thousandth prime aka 7879
	mapSize *= 2;
	for(;primedex<maxprimedex;primedex++){
		if(kprimes[primedex] > mapSize){
			mapSize = kprimes[primedex];
			return;
		}
	}
}

void hashMap::reHash() {
	int oldSize = mapSize;
	hashNode** oldMap = map;
	getClosestPrime(); // mapSize is now the closest prime to double itself
	map = new hashNode*[mapSize];
	for(int i = 0;i<mapSize;i++){map[i] = NULL;} // Init array to null

	for(int i = 0;i<oldSize;i++){
		if(oldMap[i] != NULL){
			for(int x = 0; x < oldMap[i]->currSize;x++){
				addKeyValue(oldMap[i]->keyword,oldMap[i]->values[x]);
			}
		}
	}
	for(int x = 0;x<oldSize;x++){
		//if(oldMap[x] != NULL){delete oldMap[x];} WORKS IN DEBUG MODE BUT NOT IN RUN MODE???????
	}
	return;
}

int hashMap::coll1(int h, int i, string k) {
	// Linear probing collision algorithm will just keep looking forward for empty spot
	if(i){ // if searching for spot to put
		collisions++;
		while(map[i] != NULL){

			i = (i+1) % mapSize;
		}
		return i;
	}
	if(h){ // if searching for exist
		for(;h<mapSize;h++){
			if(map[h] == NULL){
				return -1;
			}else{
				if(map[h]->keyword.compare(k) == 0){
				return h;
				}
			}
		}
		return -1;
	}
	return -1;

}
int hashMap::coll2(int h, int i, string k) {
	// Algorithm to add ascending primes to index until an empty spot is found
	if(i){ // if searching for spot to put
		h = 0;
		collisions++;
		while(map[i] != NULL){
			i = (i+kprimes[h]) % mapSize;

			h++;
		}
		return i;
	}
	if(h){ // if searching for exist
		i = 0;
		while(map[h] != NULL){
			if(map[h]->keyword.compare(k) == 0){
				return h;
			}
			h = (h+kprimes[i]) % mapSize;
			i++;
		}
		return -1;
	}
	return -1;
}
int hashMap::findKey(string k) {
//NOTE: THIS METHOD CANNOT LOOP from index 0 to end of hash array looking for the key.  That destroys any efficiency in run-time.
	int curIndex;
	if(hashfn){
		curIndex = abs(calcHash1(k)) % mapSize;
	}else{
		curIndex = abs(calcHash2(k)) % mapSize;
	}
	if(map[curIndex] != NULL){
		if(map[curIndex]->keyword.compare(k) == 0){
			return curIndex;
		}
	}
	if(collfn){
		return coll1(curIndex,0,k);
	}else{
		return coll2(curIndex,0,k);
	}

}


void hashMap::printMap() {
	cout << "In printMap()" << endl;
	for (int i = 0; i < mapSize; i++) {
		//cout << "In loop" << endl;
		if (map[i] != NULL) {
			cout << map[i]->keyword << ": ";
			for (int j = 0; j < map[i]->currSize;j++) {
				cout << map[i]->values[j] << ", ";
			}
			cout << endl;
		}
	}
}


