#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <memory>
#include <map>

typedef std::vector<int> SR;
typedef std::pair<std::vector<int>, std::vector<int>> SRpair;

SR fsr(bool b, SR sr)
{
	int s = sr[0];
	int r = sr[1];
	
	if( s == b) r += 2*b-1;
	//s = 6+s*(5+s*s*s*(2+s*(3+s))) + b*(5+s*(5+s*(6+s*s*(3+s*6))));
	if(b == 0) 
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
	return {s,r};
}

struct Tree
{
	std::map<SR, SRpair> mymap;
	std::map<SR, SRpair>::iterator it;
	
	Tree() {}
	Tree(int N, SR sr)
	{
		it = mymap.begin(); 
		
		//insert first node
		mymap.insert( std::pair<SR, SRpair>( sr, SRpair(fsr(0,sr),fsr(1,sr)) )); 	

		for(int depth=0; depth < N; depth++)
		{
			std::vector<SR> SRlist;
			for (it=mymap.begin(); it!=mymap.end(); ++it)
			{
				SRlist.push_back(it->second.first );
				SRlist.push_back(it->second.second);
			}
			for(auto v : SRlist)
			{
				mymap.insert( std::pair<SR, SRpair>( v , SRpair(fsr(0,v),fsr(1,v)) ) );
			}
		}
	}
	
	int search(std::vector<bool> v)
	{
		SR currentSR = {2,0};
		SRpair currentPair = mymap[currentSR];
		
		for(auto b : v)
		{
			if(b) currentSR = currentPair.second;
			else currentSR = currentPair.first;
			
			currentPair = mymap[currentSR];
		}
		
		return currentSR.back();		
	}
	
	void printsize()
	{
		std::cout << "mymap contains " << mymap.size() << " elements.\n";
	}
	void print()
	{
		// showing contents:
		std::cout << "mymap contains " << mymap.size() << " elements:\n";
		for (it=mymap.begin(); it!=mymap.end(); ++it)
		{
			std::cout << "(";
			for(auto v : it->first) std::cout << v << " ";
			std::cout << ")" << " => ";


			std::cout << "( (";
			for(auto v : it->second.first) std::cout << v << " ";
			std::cout << "), (";
			for(auto v : it->second.second) std::cout << v << " ";
			std::cout << ") )" << std::endl;
		}
	}
};

int main()
{
	int N = 3;
	int Ncore = 2;
	
	Tree tree(N,{2,0});
	
	tree.print();
	//tree.printsize();
	
	//std::vector<bool> v = {0,0,0};
	//std::cout << "r is: " << tree.search(v) << std::endl;
/*
	std::vector<std::vector<bool>> v;
	std::vector<bool> v1, v2;
	
	for(int i=0; i<N; i++) 
	{
		v1.push_back(false);
		if(i < N/2) v2.push_back(false);
		else v2.push_back(true);
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
			double r = tree.search(v[j]);
			if(r) count ++;		
			
			//for(auto b : v[j]) std::cout << b;
			//std::cout << "\t" << r << std::endl;
			
		} while(std::next_permutation(v[j].begin(), v[j].end()));
				
		v[j][0] = 1;
		std::sort(v[j].begin(), v[j].end());
	}
	
	//std::cout << "N = " << N << ", count = " << count << std::endl;
	std::cout << "count = " << count << std::endl;
*/
	return 0;
}