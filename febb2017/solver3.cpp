#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <omp.h>

enum{ N = 28 };

int fr(const std::array<bool,N>& v)//, int s = 2)
{
	int s = 2;
	int r = 0;
	for(auto b : v)
	{
		if( s == b) r += 2*b-1;
		//s = 6+s*(5+s*s*s*(2+s*(3+s))) + b*(5+s*(5+s*(6+s*s*(3+s*6))));
		if(!b) 
		{
			//s = (6+s*(5+s*s*s*(2+s*(3+s))));
			//s = s % 7;
			if(s == 0 || s == 4 || s == 7) s = 6;
			else if(s == 1 || s == 3 || s == 5) s = 3;
			else if(s == 6) s = 1;
			else s = 5;
		}
		else
		{
			if(s == 3 || s == 1) s = 0;
			else s = 4;
		}
		//s = 6 + 5*s + 2*s*s*s*s + 3*s*s*s*s*s + s*s*s*s*s*s + b*(5 + 5*s + 6*s*s + 3*s*s*s*s + 6*s*s*s*s*s);
		
	}
	return r;
}

void fc(int Ncore)
{
	//std::vector<bool> v(N);
	std::vector<std::array<bool,N>> v;
	std::array<bool,N> v1, v2;
	
	for(int i=0; i<N; i++) 
	{
		v1[i] = false;
		if(i < N/2) v2[i] = false;
		else v2[i] = true;
	}
	
	v.push_back(v1);
	v.push_back(v2);
	
	unsigned long long int count = 0;
	
	#pragma omp parallel for reduction(+:count)
	for(int j=0; j<Ncore; j++)
	for(int i=0; i<N/Ncore; i++)
	{
		do
		{
			double r = fr(v[j]);
			if(r) count ++;		
			
			//for(auto b : v[j]) std::cout << b;
			//std::cout << "\t" << r << std::endl;
			
		} while(std::next_permutation(v[j].begin(), v[j].end()));
				
		v[j][0] = 1;
		std::sort(v[j].begin(), v[j].end());
	}
	
	//std::cout << "N = " << N << ", count = " << count << std::endl;
	std::cout << "count = " << count << std::endl;
}

int main()
{
	//for(int i = 3; i<25; i++) fc(i);
	fc(2);
	
	return 0;
}