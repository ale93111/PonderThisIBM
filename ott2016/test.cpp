#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


bool Check_t(int Ndigits, int pos, const std::vector<char> & e, const std::vector<char> & t, int & pos_t)
{
	bool check = false;
	
	int i;
	for(i=0; i<e.size(); i++)
	{
		check = true;
		
		for(int k=0; k<Ndigits; k++) if( e[pos+k] != t[i+k] ) check = false;
		
		if(check) break;
	}
	
	pos_t = i;
	
	return check;
}

void swap ( std::vector<char> & one, std::vector<char> & two ) 
{
	std::vector<char> temp = one;
	
	one = two;
	two = temp;
}

int main(void)
{
	std::vector<char> e, p, t;
	
	char e_digit, p_digit, t_digit;
	
	std::ifstream e_file_in("e.txt");
	while( e_file_in >> e_digit ) {
		e.push_back( e_digit );
	}
	e_file_in.close();
	
	std::ifstream p_file_in("p.txt");
	while( p_file_in >> p_digit ) {
		p.push_back( p_digit );
	}
	p_file_in.close();
	
	std::ifstream t_file_in("t.txt");
	while( t_file_in >> t_digit ) {
		t.push_back( t_digit );
	}
	t_file_in.close();
	
	//for(const char idigit : e) std::cout << idigit;
	std::cout << std::endl;
	
	//for(const char idigit : p) std::cout << idigit;
	std::cout << std::endl;
	
	//for(const char idigit : t) std::cout << idigit;
	std::cout << std::endl;
	
	//swap(e,p);
	
	if( e.size() == p.size() && e.size() == t.size() ) std::cout << "Size check OK!" << std::endl;
	else std::cout << "Size check FAILED!" << std::endl;
	
	int Ndigits = 5;
	int Nsize = e.size();
	
	bool check = false;
	
	std::vector<int> i_store, j_store, k_store;
	
	int i,j,it;
	for(i=0; i<Nsize; i++)
	{
		for(j=0; j<Nsize; j++)
		{
			
			check = true;
			
			for(int k=0; k<Ndigits; k++) if( e[i+k] != p[j+k] ) check = false;
			
			if(check) check = Check_t(Ndigits, i, e, t, it);
			
			if(check) 
			{
				//goto label;
				i_store.push_back( i );
				j_store.push_back( j );
				k_store.push_back( it);
			}
		}
	}
	
	if(!i_store.size()) std::cout << "Nothing found!" << std::endl;
	
	//label: 
	
	std::vector<int> total_sum;
	
	for(int i=0; i<i_store.size(); i++) total_sum.push_back( i_store[i] + j_store[i] + k_store[i] );
	
 	auto minelement = std::min_element(std::begin(total_sum), std::end(total_sum));
	int index = std::distance(std::begin(total_sum), minelement);
	
	//for(int i=0; i<total_sum.size(); i++) std::cout << total_sum[i] << ' ';
	std::cout << std::endl;
	
	int time_i = i_store[index]+Ndigits+1;
	int time_j = j_store[index]+Ndigits+1;
	int time_k = k_store[index]+Ndigits+1;
	
	int time_tot = time_i + time_j + time_k;
	
	std::cout << "i = " << time_i << " and  j = " << time_j <<  " and k = " << time_k << std::endl;
	std::cout << "Total time (in seconds)  = " << time_tot << std::endl;
	std::cout << "Total time (in hh:mm:ss) = " << time_tot/3600 << ":" << time_tot/60 << ":" << time_tot%60 << std::endl;
	
	for(int k=0; k<Ndigits; k++) std::cout << e[i_store[index]+k];
	std::cout << std::endl;
	
	for(int k=0; k<Ndigits; k++) std::cout << p[j_store[index]+k];
	std::cout << std::endl;
	
	for(int k=0; k<Ndigits; k++) std::cout << t[k_store[index]+k];
	std::cout << std::endl;
	
	return 0;
}