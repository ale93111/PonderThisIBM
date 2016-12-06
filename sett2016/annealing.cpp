#include <iostream>
#include <string>

#include <algorithm>    // std::random_shuffle
#include <vector> 
#include <random>       // std::default_random_engine
#include <math.h>		// exp
#include <chrono>       // std::chrono::system_clock

enum { N = 24 };

//function to minimize that counts the number of violations of the sequence
int Energy( std::vector<std::string> state)
{
	int count = 0;
	
	for(int i=0; i<N-1; i++)
	{
		for(int j=0; j<4;  j++)
		{
			if( state[i][j] == state[i+1][j] ) count++;
			if(i != N-2) if( state[i][j]  == state[i+2][j] ) count++;
			//if(i != N-2) if( state[i][j+1] == state[i+2][j] ) count++;
		}
	}
	
	return count;
}

//swap to random elements to find a neighbour
std::vector<std::string> Neighbour ( std::vector<std::string> current_state, std::mt19937 & generator, std::uniform_int_distribution<int> & rand_int )
{
	std::vector<std::string> neighbour = current_state;
	int r1, r2;
	r1 = rand_int(generator);
	do 
	{ 
		r2 = rand_int(generator); 
	}
	while ( r2 == r1 );
	
	//swap
	std::swap(neighbour[r1],neighbour[r2]);
	
	return neighbour;
}

double AcceptanceProbability(double e_new, double e, double t)
{
	if( e_new < e) return 1.0;
	else return exp( -(e_new - e)/t );	
}

void print(std::vector<std::string> stringtoprint)
{
	for( const std::string& str : stringtoprint ) std::cout << str << ' ' ;
	std::cout << std::endl;
}

void print2(std::vector<std::string> stringtoprint)
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<N; j++)
			std::cout << stringtoprint[j][i] << ' ';
		
		std::cout << std::endl;
	}
}

int main(void)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> rand_int(0, 23);	
	std::uniform_real_distribution<double> rand_double01(0, 1);
	
	std::vector<std::string> starting = {
		"ACDB", "CBAD", "BACD", "DCBA", 
		"CDAB", "BADC", "ABCD", "DCAB", 
		"DABC", "CBDA", "BDAC", "ACBD", 
		"DBCA", "ADCB", "BCAD", "CABD", 
		"ABDC", "BDCA", "CADB", "ADBC", 
		"DBAC", "BCDA", "CDBA", "DACB" };
	
	//std::shuffle( std::begin(starting), std::end(starting) ,  std::default_random_engine(seed)) ;
	
	std::cout << "starting " << std::endl;
	print(starting);
	int count = Energy(starting);
	std::cout << "starting energy = " << count << std::endl;
	
	std::vector<std::string> current_state = starting;
	std::vector<std::string> neighbour;
	
	
	//simulated annealing
	double temperature, energy_current, energy_new;
	int k_max = 1000000;
	for(int k = 1; k<k_max; k++)
	{
		temperature = 0.3*(1.0 - (k/k_max)*(k/k_max)*(k/k_max)*(k/k_max));
		
		neighbour = Neighbour(current_state, generator, rand_int);
		
		energy_current = Energy(current_state);
		energy_new     = Energy(neighbour);
		
		if( AcceptanceProbability(energy_new, energy_current, temperature) > rand_double01(generator))
		{
			current_state = neighbour;
		}
		
	}
	
	print(current_state);
	int finalcount = Energy(current_state);
	std::cout << "final count = " << finalcount << std::endl;
	print2(current_state);
	return 0;
}