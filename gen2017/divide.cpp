#include <boost/multiprecision/gmp.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>

void findy(mpf_t y, long unsigned int nn, int precision)
{
	mpz_t n,n1,y1,y2;
	mpz_inits(n,n1,y1,y2,NULL);

	mpz_set_ui( n,  nn);
	mpz_set_ui(n1,nn+1);

	mpz_pow_ui(y1,n1,nn+1);
	mpz_pow_ui(y2,n ,nn+1);
	
	mpf_t y3,y4;
	mpf_init2(y3,precision);
	mpf_init2(y4,precision);
	mpf_set_z(y3,y1);
	mpf_set_z(y4,y2);
	
	mpf_div(y,y3,y4);	
	
	mpz_clear(n);
	mpz_clear(n1);
	mpz_clear(y1);
	mpz_clear(y2);
	mpf_clear(y3);
	mpf_clear(y4);
}


void findx(mpf_t x, long unsigned int nn, int precision)
{
	mpz_t n,n1,x1,x2;
	mpz_inits(n,n1,x1,x2,NULL);

	mpz_set_ui( n,  nn);
	mpz_set_ui(n1,nn+1);

	mpz_pow_ui(x1,n1,nn);
	mpz_pow_ui(x2,n ,nn);
	
	/*
	std::cout << "X1 = "; 
	mpz_out_str(stdout, 10, x1);
	std::cout << std::endl;
	
	std::cout << "X2 = ";
	mpz_out_str(stdout, 10, x2);
	std::cout << std::endl;
	*/
	mpf_t x3,x4;
	mpf_init2(x3,precision);
	mpf_init2(x4,precision);
	mpf_set_z(x3,x1);
	mpf_set_z(x4,x2);
	
	mpf_div(x,x3,x4);	
	
	mpz_clear(n);
	mpz_clear(n1);
	mpz_clear(x1);
	mpz_clear(x2);
	mpf_clear(x3);
	mpf_clear(x4);
}

int getsizef(mpf_t x)
{
	mp_exp_t exponent; 
	char* c = mpf_get_str(NULL,&exponent,10,0,x);
	
	return strlen(c);
}

int main()
{
	using namespace boost::multiprecision;
	
	int precision = 262144;
	char* c;

	mpf_t x,y;
	mpf_init2(x,precision);
	mpf_init2(y,precision);
	mp_exp_t exponent;
	
	for(long unsigned int nn = 1; nn<=5000; nn++)
	{
		findx(x,nn,precision);
		findy(y,nn,precision);
		
		//std::cout << "X = ";
		//mpf_out_str(stdout, 10, 10, x);
		//std::cout << std::endl;
		//int sz = getsizef(x);
		c = mpf_get_str(NULL,&exponent,10,0,x);
		int sz = strlen(c); 
		if(sz < 78911)//315651)	//<precision/3.33
		{
			std::cout << "N = " << nn << "\t" << "# of digits of X = " << sz << " : ";// << std::endl;
			if(sz > 7) for(int i=0; i<10; i++) std::cout << c[sz-10+i];
			std::cout << "\t";
			
			bool ok = true;
			for (int i=sz-8; i<sz; i++) 
				for (int j= i+1; j<sz; j++) 
					if(c[i] == c[j]) ok = false;

			if(ok) std::cout << "OK!!!!!";
			else std::cout << "NO";
			std::cout << std::endl;
		}
		
		c = mpf_get_str(NULL,&exponent,10,0,y);
		sz = strlen(c); 
		if(sz < 78911)
		{
			std::cout << "N = " << nn << "\t" << "# of digits of Y = " << sz << " : ";// << std::endl;
			if(sz > 7) for(int i=0; i<10; i++) std::cout << c[sz-10+i];
			std::cout << "\t";
			
			bool ok = true;
			for (int i=sz-8; i<sz; i++) 
				for (int j= i+1; j<sz; j++) 
					if(c[i] == c[j]) ok = false;

			if(ok) std::cout << "OK!!!!!";
			else std::cout << "NO";
			std::cout << std::endl;
		}

	}
	
	
	mpf_clear(x);
	
	return 0;
}
