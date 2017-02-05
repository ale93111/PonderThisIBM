#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <memory>
#include <map>
#include <omp.h>

enum{ N = 28 };
enum{ Ncore = 2 };

struct Node;

typedef std::vector<int> SR;
typedef std::pair<Node, Node> NodePair;

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

struct Node
{
	SR sr;
	Node *left;
	Node *right;
	
	Node() : left(nullptr), right(nullptr) {}
	Node(const SR & sri) : sr(sri), left(nullptr), right(nullptr) {}

	bool operator == (const Node& other) const
	{
		return (sr == other.sr);
	}

	bool operator < (const Node& other) const
	{
		return (sr < other.sr);
	}

	friend std::ostream& operator << (std::ostream& stream, const Node& node)
	{
		stream << "(" << node.sr[0] << ", " << node.sr[1] << ")";
		return stream;
	}
};

struct Treemap
{
	std::map<Node, NodePair> mymap;
	std::map<Node, NodePair>::iterator it;
	
	int map_search(const std::array<bool,N>& v)
	{
		Node currentNode({2,0});
		NodePair currentPair = mymap[currentNode];
		
		for(auto b : v)
		{
			if(b) currentNode = currentPair.second;
			else currentNode = currentPair.first;
			
			currentPair = mymap[currentNode];
		}
		
		return currentNode.sr.back();		
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
			std::cout << it->first << " => ";
			std::cout << "(" << it->second.first << ", " << it->second.second << ")" << std::endl;
		}
	}

	Treemap() {}
	Treemap(int N, SR sr)
	{
		it = mymap.begin(); 
		
		//insert first node
		Node root(sr);
		Node left_node (fsr(0,sr));
		Node right_node(fsr(1,sr));

		mymap.insert( std::pair<Node, NodePair>(root, NodePair(left_node,right_node)) ); 	

		for(int depth=0; depth<N; depth++)
		{
			std::vector<Node> Nodelist;
			for (it=mymap.begin(); it!=mymap.end(); ++it)
			{
				Nodelist.push_back(it->second.first );
				Nodelist.push_back(it->second.second);
			}
			for(auto& v : Nodelist)
			{
				Node root(v.sr);
				Node left_node (fsr(0,v.sr));
				Node right_node(fsr(1,v.sr));

				mymap.insert( std::pair<Node, NodePair>(root, NodePair(left_node,right_node)) ); 
			}
		}
	}
};

//It is not possible to use treemap as a tree because you can't initialize const key pointers 
//to its own respective elements in the map so we use Tree:
struct Tree
{
	std::vector<Node> Nodelist;
	std::vector<Node>::iterator nit;
	Node* root;

	int node_search(std::array<bool,N>& v)
	{
		Node* currentNode = root;
		for(auto b : v)
		{
			if(b) currentNode = currentNode->right;
			else  currentNode = currentNode->left;
		}

		return currentNode->sr.back();	
	}

	void initialize_pointers(Treemap& tm)
	{
		Node head({2,0});
		root = search(head);
		root->left  = search(tm.mymap[head].first);
		root->right = search(tm.mymap[head].second);

		std::vector<Node>::iterator it;
		for(it=Nodelist.begin(); it!=Nodelist.end(); ++it)
		{
			it->left  = search(tm.mymap[*it].first);
			it->right = search(tm.mymap[*it].second);
		}
	}

	Node* search(const Node& node)
	{
		for(nit=Nodelist.begin(); nit!=Nodelist.end(); ++nit)
		{
			if( *nit == node) return &(*nit);
		}

		return nullptr;
	}

	void printsize()
	{
		std::cout << "Tree contains " << Nodelist.size() << " elements.\n";
	}

	void print()
	{
		std::cout << "My Tree is:\n";
		for(nit=Nodelist.begin(); nit!=Nodelist.end(); ++nit)
		{
			std::cout << *nit << " => ";
			std::cout << "(";
			if(nit->left)  std::cout << *nit->left;
			else std::cout << " None ";
			std::cout << ", ";
			if(nit->right) std::cout << *nit->right;
			else std::cout << " None ";
			std::cout << ")\n";
		}
	}

	Tree() : root(nullptr) {}
	Tree(Treemap& tm)
	{
		for (tm.it=tm.mymap.begin(); tm.it!=tm.mymap.end(); ++tm.it)
		{
			if(!search(tm.it->first))         Nodelist.push_back(tm.it->first );
			if(!search(tm.it->second.first))  Nodelist.push_back(tm.it->second.first );
			if(!search(tm.it->second.second)) Nodelist.push_back(tm.it->second.second);
		}

		std::sort(Nodelist.begin(),Nodelist.end());

		initialize_pointers(tm);
	}
};

int main()
{
	//int N = 28;
	//int Ncore = 2;
	
	Treemap treemap(N,{2,0});
	
	Tree tree(treemap);

	//tree.print();
	//std::cout << std::endl;
	//treemap.printsize();
	//tree.printsize();

	/*
	//test
	std::cout << std::endl;
	std::vector<bool> v = {0,0,1,0};
	std::cout << "r is: " << treemap.map_search(v) << std::endl;
	std::cout << "r is: " << tree.node_search(v) << std::endl;
	*/
	
	std::vector<std::array<bool,N>> v;
	std::array<bool,N> v1, v2;
	
	for(std::size_t i=0; i<N; i++) 
	{
		v1[i] = false;
		if(i < N/2) v2[i] = false;
		else v2[i] = true;
	}
	
	v.push_back(v1);
	v.push_back(v2);
	
	unsigned long long int count = 0;
	
	#pragma omp parallel for reduction(+:count)
	for(std::size_t j=0; j<Ncore; j++)
		for(std::size_t i=0; i<N/Ncore; i++)
		{
			do
			{
				//double r = treemap.map_search(v[j]);
				double r = tree.node_search(v[j]);
				if(r) count ++;		
				
				//for(auto b : v[j]) std::cout << b;
				//std::cout << "\t" << r << std::endl;
				
			} while(std::next_permutation(v[j].begin(), v[j].end()));
					
			v[j][0] = 1;
			std::sort(v[j].begin(), v[j].end());
		}
	
	//std::cout << "N = " << N << ", count = " << count << std::endl;
	std::cout << "count = " << count << std::endl;
	
	return 0;
}