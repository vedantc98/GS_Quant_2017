class Tree:

	def dfs_mod(self, start, visited, m, profit, max_p):
		visited[start] = True
		max_p = max(profit, max_p)

		neighbors_list = self.adj[start]
		for neighbor in neighbors_list:
			next = neighbor[0]
			t = neighbor[1]
			p = neighbor[2]

			if(m >= t and not visited[next]):
				temp = self.dfs_mod(next, visited, m - t, profit + p, max_p)
				max_p = max(max_p, temp)

		return max_p

	def __init__(self, n, adj):
		self.n = n
		self.adj = adj

n, m  = map(int, raw_input().strip().split())
structure = []
for i in xrange(n + 1):
	structure.append([])

for a0 in xrange(n - 1):
	u, v, t, p = map(int, raw_input().strip().split())
	structure[u].append([v, t, p])
	structure[v].append([u, t, p])

market = Tree(n, structure)

max_t = 0

for ind in range(1, len(structure)):
	visited = []
	for i in range(n + 1):
		visited.append(False)
	max_t = max(max_t, market.dfs_mod(ind, visited, m, 0, 0))

print max_t