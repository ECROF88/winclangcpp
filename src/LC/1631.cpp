#include <algorithm>
#include <iostream>
#include <vector>

int N = 10009;
std::vector<int> p(N);
int find(int x)
{
  if (p[x] != x) p[x] = find(p[x]);
  return p[x];
}
bool query(int a, int b) { return find(a) == find(b); }

void unions(int a, int b)
{
  if (find(a) == find(b)) return;
  p[find(a)] = find(b);
}
struct Edge {
  int x, y, weight;
};
int minimumEffortPath(std::vector<std::vector<int>>& heights)
{
  int row = heights.size();
  int col = heights[0].size();
  std::vector<Edge> edges;
  p.resize(row * col);
  for (int i = 0; i < row * col; i++) p[i] = i;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      int index = i * col + j;
      if (i + 1 < row) {
        int a = index;
        int b = (i + 1) * col + j;
        int weight = std::abs(heights[i][j] - heights[i + 1][j]);
        edges.push_back({a, b, weight});
      }
      if (j + 1 < col) {
        int a = index;
        int b = i * col + j + 1;
        int weight = std::abs(heights[i][j] - heights[i][j + 1]);
        edges.push_back({a, b, weight});
      }
    }
  }
  std::sort(edges.begin(), edges.end(),
            [](const Edge& a, const Edge& b) { return a.weight < b.weight; });

  int start = 0, end = row * col - 1;
  for (auto& e : edges) {
    int w = e.weight;
    int x = e.x;
    int y = e.y;
    unions(x, y);
    if (query(start, end)) return w;
  }
  return 0;
}

auto main() -> int
{
  std::vector<std::vector<int>> heights = {{1, 2, 2}, {3, 8, 2}, {5, 3, 5}};
  std::cout << minimumEffortPath(heights) << std::endl;
  return 0;
}