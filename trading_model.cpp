#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

vector< unsigned int > empty_vector(0);

class Tree {
public:
	//Number of nodes of tree
	unsigned int n;
	//adjacency list
	vector< vector< unsigned int > > adj;
	//weights of nodes(strengths of stocks)
	vector<unsigned int> S;
	//sum of strengths of all the stocks in the tree
	unsigned long long int total_strength;
	//Empty boolean vector for DFS
	vector<bool> visited_none;

	//Should return the path inclusive of start and end nodes
	vector< unsigned int > dfs(unsigned int start, unsigned int end, vector<bool> &visited, vector<unsigned int> temp_path) {
		//TODO
		if(start == end){
			temp_path.push_back(start);
			return temp_path;
		}

		visited[start] = true;
		temp_path.push_back(start);

		for(vector< unsigned int >::iterator it = adj[start].begin(); it != adj[start].end(); it ++) {
			if(!visited[*it]){
				vector<unsigned int> result = dfs(*it, end, visited, temp_path);
				if(!result.empty()){
					return result;
				}
			}
		}

		return empty_vector;

	}

	//Assumes that the path includes the start as well as the end nodes
	pair< unsigned int, unsigned int > get_clashing_nodes(unsigned int start, unsigned int end) {
		vector< unsigned int > path = dfs(start, end, visited_none, empty_vector);
		unsigned int path_length = path.size();

		//if no other nodes are between start and end
		if(path_length == 2){
			return make_pair(start, end);
		}

		pair< unsigned int, unsigned int > nodes;
		unsigned int first_index;
		//if path length is odd
		if(path_length & 1) {
			first_index = path_length / 2; 
		}
		else{
			first_index = path_length / 2 - 1;
		}

		nodes.first = first_index;
		nodes.second = first_index + 1;

		return nodes;
	}

	//Gets the sum of the subtree starting from the first node (disjoint subgraph seperated by the edge connecting node_a and node_b)
	unsigned long long int get_subgraph_sum(unsigned int node_a, unsigned int node_b) {
		//TODO
		return 1;
	}

	pair< unsigned long long int, unsigned long long int > get_scores(unsigned int node_a, unsigned int node_b) {
		unsigned long long int A_score = get_subgraph_sum(node_a, node_b);
		unsigned long long int B_score = total_strength - A_score;

		return make_pair(A_score, B_score);
	}

	Tree(unsigned int num, vector<unsigned int> weights, vector< vector < unsigned int > > edges) {
		
		n = num;
		adj = edges;
		S = weights;
		visited_none.resize(n, false);

	}

};

int main(void){
	unsigned int n;
	cin>>n;

	vector<unsigned int> S;
	unsigned int temp;
	unsigned long long int tree_sum = 0;
	for(unsigned int i = 0; i < n; i++){
		cin>>temp;
		S.push_back(temp);
		tree_sum += temp;
	}

	vector< vector < unsigned int > > adj(n + 1);
	adj[0] = empty_vector;
	unsigned int a, b;
	for(unsigned int i = 0; i < n - 1; i ++){
		cin>>a>>b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	Tree stocks(n, S, adj);
	stocks.total_strength = tree_sum;

	/* TESTING THE DFS
	printf("ENTER THE START NODE\n");
	unsigned int s, e;
	cin>>s;
	printf("ENTER THE END NODE\n");
	cin>>e;
	vector<unsigned int> path_test = stocks.dfs(s, e, stocks.visited_none, empty_vector);

	printf("The sum of nodes is: %llu\n The path from start to end is as follows:\n", stocks.total_strength);
	for(vector<unsigned int>::iterator it = path_test.begin(); it != path_test.end(); it ++){
		cout<<*it<<endl;
	}

	return 1; */

	unsigned int m;
	cin>>m;

	unsigned long long int total_A_score = 0;
	unsigned long long int total_B_score = 0;
	unsigned int A_wins = 0;
	unsigned int B_wins = 0;
	unsigned int u, v;
	pair< unsigned long long int, unsigned long long int > scores;

	//Take the queries as input and simultaneously compute victory/loss for A or B
	for(unsigned int i = 0; i < m; i ++) {
		cin>>u>>v;

		scores = stocks.get_scores(u, v);
		total_A_score += scores.first;
		total_B_score += scores.second;

		scores.first < scores.second? B_wins ++: A_wins ++;

	}

	printf("%u %llu %u %llu\n", A_wins, total_A_score, B_wins, total_B_score);

	return 0;
}