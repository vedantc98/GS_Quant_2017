#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Tree {
public:
	unsigned int n;
	//A vector of all the nodes. Each node contains a vector representing it's neighbors(through exchanges). Each neighbor has a node number, ti and pi value.
	vector < vector < vector < unsigned long long int > > > adj;

	unsigned long long int dfs_mod(unsigned long long int start, vector<bool> &visited, unsigned long long int m, unsigned long long int profit, unsigned long long int max) {
		visited[start] = true;

		//printf("VISITING NODE %llu\n", start);
		if(profit > max){
			max = profit;
		}

		for(vector< vector <  unsigned long long int > >::iterator it = adj[start].begin(); it != adj[start].end(); it ++) {
			unsigned long long int next = (*it)[0];
			unsigned long long int t = (*it)[1];
			unsigned long long int p = (*it)[2];

			if(m >= t && !visited[next]) {
				unsigned long long int temp = dfs_mod(next, visited, m - t, profit + p, max);
				if (temp > max) {
					max = temp;
				}
			} 
		}

		return max;
	}

	Tree(vector < vector < vector < unsigned long long int > > > str, unsigned int num) {
		adj = str;
		n = num;
	}


};

int main(void){
	unsigned int n;
	unsigned long long int m;

	cin>>n>>m;
	unsigned long long int u, v, t, p;
	vector < vector < vector < unsigned long long int > > > structure(n + 1);	
	vector < unsigned long long int > data(3); 

	for(unsigned int i = 0; i < n - 1; i ++) {
		cin>>u>>v>>t>>p;
		data[0] = v;
		data[1] = t;
		data[2] = p;

		structure[u].push_back(data);
		data[0] = u;
		structure[v].push_back(data);

	}

	Tree market(structure, n);

	unsigned long long int max = 0;
	for(unsigned int i = 1; i < n + 1; i ++){
		vector<bool> visited_none(n + 1, false);
		unsigned long long int temp = market.dfs_mod(i, visited_none, m, 0, 0);
		if(temp > max){
			max = temp;
		}
	}

	cout<<max<<endl;

	return 0;
}