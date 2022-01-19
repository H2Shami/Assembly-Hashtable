#include "/public/read.h"
#include <random>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <algorithm>
using namespace std;
using namespace chrono;

extern "C" {
//YOU: Write this function, which should insert the first SIZE/2 elements of randos into hash_table
// hash_table is of size SIZE
void print (int a, int b, int c, int d) { cout << "R0: " << a << "\tR1: " << b << "\tR2: " << c << "\tR3: " << d << endl;}
int modulus (int a, int b) { return a % b; }
void asm_hash_insert(int *hash_table, int *randos, int SIZE);
//YOU: Write this function, which returns true or false if the passed in int is in the hash table
bool asm_hash_search(int *hash_table, int key, int SIZE); 
}


//Learning Points:
//1. <random> is a better but WAY MORE VERBOSE way of making random numbers
//2. <chrono> is a better but SIGH MORE VERBOSE way of getting the time
//3. Generate can be used to generate a random array for testing
//4. Iota can be used to generate a bunch of unique ids
//5. You could easily write code to do #3 or #4 but it expresses intent and might open up optimizations

int main() {
	const bool DEBUG = false; //Set to true to print out the arrays
	int seed = read("Please enter a random seed:\n");
	const size_t SIZE = read("Please enter how big the hash table should be:\n");

	//These three lines make a function called roll() that generates random numbers 
	mt19937 generator(seed);
	uniform_int_distribution<int> distribution(1,SIZE << 2); //Make the range 4x that of the size so we get collisions
	auto roll = bind ( distribution, generator ); //Makes a function named roll that returns 1 to 8M

	//Make an array holding all the random numbers so we can feed them to both the C++ and ASM implementations
	vector<int> randos(SIZE);
	auto start = steady_clock::now(); //Get the current time
		generate(randos.begin(),randos.end(),roll); //Generate calls the roll() function on each element of arr
		//iota(arr.begin(),arr.end(),1); //Fills the array with 1 to 1M
		if constexpr (DEBUG) { 
			for (int x : randos) cerr << x << " ";
			cerr << endl;
		}
	auto end = steady_clock::now();
	duration<double> diff = end-start;
	cerr << "Time to generate " << SIZE << " ints: " << diff.count() << "s\n";

	vector<bool> results; //Hold results of the search to compare with your asm code
	results.reserve(SIZE); //Preallocate space for the results
	vector<bool> results2; //Hold results of your search function
	results2.reserve(SIZE); //Preallocate space for the results

	//C++ Reference Implementation with linear probing, 50% load
	{
		vector<int> table(SIZE); //Will half fill the table
		start = steady_clock::now();
		//Insert the first half from rando into table
		//Note: Duplicates are allowed
		for (size_t i = 0; i < SIZE/2; i++) {
			size_t address = randos[i] % SIZE;
			while (table[address] != 0) address = (address+1)%SIZE;
			table[address] = randos[i];
		}
		if constexpr (DEBUG) {
			for (int x : table) cerr << x << " ";
			cerr << endl;
		}
		//Now use the second half of rando to search the table
		for (size_t i = SIZE/2; i < SIZE; i++) {
			size_t address = randos[i] % SIZE;
			bool found = false;
			while (table[address] != 0) {
				if (table[address] == randos[i]) found = true;
				address = (address+1)%SIZE;
			}
			results.push_back(found);
		}
		if constexpr (DEBUG) { 
			for (int x : results) cerr << x << " ";
			cerr << endl;
		}
		end = steady_clock::now();
		diff = end-start;
		cerr << "C++ Run Time: " << diff.count() << "s\n";
	}
	//YOU: Do the above block, except in assembly
	{
		int *table = new int[SIZE];
		fill(table,table+SIZE,0); //Zero fill the hash table, 0 means a bucket is open
		start = steady_clock::now();
		//YOU: DO SIZE/2 inserts here from randos into the hash table
		asm_hash_insert(table, randos.data(), SIZE);
		for (size_t i = SIZE/2; i < SIZE; i++)
			results2.push_back(asm_hash_search(table, randos[i], SIZE)); 
		end = steady_clock::now();
		diff = end-start;
		cerr << "ASM Run Time: " << diff.count() << "s\n";
		delete[] table;
	}
	if (results == results2)
		cout << "Your code is correct.\n";
	else
		cout << "Your code is buggy.\n";
}
