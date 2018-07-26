#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const vector< unsigned int > empty_vector(0);

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
	//Subgraph sums for each node
	vector< vector < unsigned long long int > > subsum;
	//testing covered/not for subgraph precomputation
	vector<bool> covered;

	unsigned int find_mod(vector<unsigned int>::iterator beg, vector<unsigned int>::iterator end, unsigned int item) {
		unsigned int count = 0;
		for(vector<unsigned int>::iterator it = beg; it != end; it ++) {
			if(*it == item)
				return count;
			count ++;
		}
		return count;
	}

	//REMEMBER THAT THE SUBSUMS ARE 0-INDEXED!!!!!!!
	void subgraph_precomputation(void) {
		//Find all the leaf nodes
		unsigned int covered_count = 0;
		vector<unsigned int> next_level;
		vector<bool> next_level_covered(n + 1, false);
		for(unsigned int i = 1; i < n + 1; i ++) {
			if(adj[i].size() == 1) {
				covered[i] = true;
				covered_count ++;
				subsum[i].resize(1);
				subsum[i][0] = total_strength - S[i];
				if(!next_level_covered[adj[i][0]]) {
					next_level.push_back(adj[i][0]);
					next_level_covered[adj[i][0]] = true;
				}
			}
		}

		while(covered_count != n) {
			unsigned int len = next_level.size();
			vector<unsigned int> next_level_copy(next_level);
			next_level.resize(0);
			unsigned int node_index;
			for(unsigned int i = 0; i < len; i ++) {
				node_index = next_level_copy[i];
				long int unknown_branch = -1;
				unsigned int len2 = adj[node_index].size();
				subsum[node_index].resize(len2);
				//sum of all other branches
				unsigned long long int run_sum = 0;
				for(unsigned int j = 0; j < len2; j ++) {
					//If the neighbor has been covered
					if(covered[adj[node_index][j]]) {
						subsum[node_index][j] = total_strength - subsum[adj[node_index][j]][find_mod(adj[adj[node_index][j]].begin(),adj[adj[node_index][j]].end(),node_index)];
						run_sum += subsum[node_index][j];
					}
					else {
						if(!next_level_covered[adj[node_index][j]]) {
							next_level_covered[adj[node_index][j]] = true;
							next_level.push_back(adj[node_index][j]);
						}
						unknown_branch = j;
					}
				}
				if(unknown_branch != -1)
					subsum[node_index][unknown_branch] = total_strength - run_sum - S[node_index]; 
				covered[node_index] = true;
				covered_count ++;
			}
		}

		for(unsigned int i = 1; i < n + 1; i ++) {
			for(unsigned int j = 0; j < subsum[i].size(); j ++) {
				printf("The subsum of node %u on branch %u is %llu\n", i, j, subsum[i][j]);
			}
		} 

	}

	//Should return the path inclusive of start and end nodes
	vector< unsigned int > dfs(unsigned int start, unsigned int end, vector<bool> &visited, vector<unsigned int> temp_path) {
		if(start == end){
			temp_path.push_back(start);
			return temp_path;
		}

		visited[start] = true;
		temp_path.push_back(start);

		for(vector< unsigned int >::iterator it = adj[start].begin(); it != adj[start].end(); it ++) {
			if(!visited[*it]){
				vector<unsigned int> result(dfs(*it, end, visited, temp_path));
				if(!result.empty()){
					return result;
				}
			}
		}

		return empty_vector;

	}

	//DFS which returns the sum of nodes
	unsigned long long int dfs_sum(unsigned int start, vector<bool> &visited, unsigned long long int prev_sum) {
		
		visited[start] = true;
		prev_sum += S[start];

		for(vector< unsigned int >::iterator it = adj[start].begin(); it != adj[start].end(); it ++) {
			if(!visited[*it]){
				prev_sum += dfs_sum(*it, visited, 0);
			}
			//printf("PREVSUM OF node %u is %llu at the moment\n", start, prev_sum);
		}

		return prev_sum;
	}

	//Assumes that the path includes the start as well as the end nodes
	pair< unsigned int, unsigned int > get_clashing_nodes(unsigned int start, unsigned int end) {
		vector<bool> visiting(n + 1, false);
		vector< unsigned int > path(dfs(start, end, visiting, empty_vector));
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

		nodes.first = path[first_index];
		nodes.second = path[first_index + 1];

		return nodes;
	}

	//Gets the sum of the subtree starting from the first node (disjoint subgraph seperated by the edge connecting node_a and node_b)
	unsigned long long int get_subgraph_sum(unsigned int node_a, unsigned int node_b) {
		
		return subsum[node_b][find_mod(adj[node_b].begin(), adj[node_b].end(), node_a)];

		/* Getting the subgraph sum using dfs on every iteration
		vector<bool> mod_visited(visited_none);
		mod_visited[node_b] = true;

		unsigned long long int sum = dfs_sum(node_a, mod_visited, 0);
		return sum; */
	}

	pair< unsigned long long int, unsigned long long int > get_scores(unsigned int node_u, unsigned int node_v) {

		pair<unsigned int, unsigned int> clashing_nodes = get_clashing_nodes(node_u, node_v);
		//printf("The clashing nodes are %u and %u\n", clashing_nodes.first, clashing_nodes.second);
		unsigned long long int A_score = get_subgraph_sum(clashing_nodes.first, clashing_nodes.second);
		unsigned long long int B_score = total_strength - A_score;

		return make_pair(A_score, B_score);
	}

	Tree(unsigned int num, vector<unsigned int> weights, vector< vector < unsigned int > > edges) {
		
		n = num;
		adj = edges;
		S = weights;
		visited_none.resize(n + 1, false);
		subsum.resize(n + 1);
		covered.resize(n + 1, false);
	}

};

int main(void){
	unsigned int n;
	cin>>n;

	vector<unsigned int> S;
	S.push_back(0);
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
	stocks.subgraph_precomputation();

	/* TESTING THE DFS 
	for(int i = 0; i < 2; i++){
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

		//cout<<total_A_score<<endl<<total_B_score<<endl;

	}

	printf("%u %llu %u %llu\n", A_wins, total_A_score, B_wins, total_B_score);

	return 0;
}