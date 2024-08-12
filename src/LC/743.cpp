#include <cstring>
#include <iostream>
#include <queue>
/*
743. Network Delay Time
从k点出发，到达所有点的最短时间
*/
const int N = 1000;
int INF = 0x3f3f3f3f;
// 邻接表，he[]存储头结点，e[]存储边的终点，ne[]存储下一条边的编号，w[]存储边的权重
int he[N], e[N], ne[N], w[N];
int idx;
void add1(int a, int b, int c)
{
  e[idx] = b;
  ne[idx] = he[a];
  he[a] = idx;
  w[idx] = c;
  idx++;
}
/*
想要遍历所有由 a 点发出的边,可以使用如下代码
for (int i = he[a]; i != -1; i = ne[i]) {
    int b = e[i], c = w[i]; // 存在由 a 指向 b 的边，权重为 c
    }
*/

// 邻接矩阵：
int g[N][N];  // g[i][j]表示i到j的边的权重
void add2(int a, int b, int c) { g[a][b] = c; }

// 1.Floyd,使用邻接矩阵
void floyd(int k)
{
  int ans;
  for (int p = 0; p < N; p++)
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        g[i][j] = std::min(g[i][j], g[i][p] + g[p][j]);  // 求最短路

  // 遍历答案：
  for (int i = 0; i < N; i++) ans = std::max(ans, g[k][i]);
}

// 2.Dijkstra,使用邻接矩阵
void dijkstra(int k)
{
  bool visited[N];  // 记录哪些点被更新过
  int dist[N];      // 记录从k到各个点的最短距离
  for (int i = 1; i < N; i++) dist[i] = INF;
  dist[k] = 0;
  std::fill(visited, visited + N, false);  // 将所有元素设置为 false
  std::memset(visited, false, sizeof visited);  // 将 visited 数组的所有元素设置为 false
  /*    初始化邻接矩阵：
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            g[i][j] = g[j][i] = i == j ? 0 : INF;
  */
  for (int i = 0; i < N; i++) {
    int t = -1;
    for (int j = 0; j < N; j++) {
      if (!visited[j] && (t == -1 || dist[j] < dist[t]))
        t = j;  // 找到距离最小的点i,更新t
    }
    visited[t] = true;
    // 用t更新其他点：
    for (int j = 0; j < N; j++) {
      dist[j] = std::min(dist[j], dist[t] + g[t][j]);
    }
  }

  int ans = 0;
  for (int i = 0; i < N; i++) {
    ans = std::max(ans, dist[i]);
  }
}

// Dijkstra,使用邻接表，使用堆优化
void dijkstra_withadj(int k)
{
  int dist[N];
  bool visited[N];
  std::fill(dist, dist + N, INF);
  std::fill(visited, visited + N, false);
  dist[k] = 0;
  std::fill(he, he + N, -1);
  // 先用add函数把图建立起来
  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                      std::greater<std::pair<int, int>>>
      q;
  q.push({0, k});  // 起点的距离为 0

  while (!q.empty()) {
    auto [step, id] = q.top();
    q.pop();

    if (visited[id]) continue;  // 如果该节点已更新，跳过
    visited[id] = true;         // 标记为已更新

    for (int i = he[id]; i != -1; i = ne[i]) {
      int j = e[i];
      if (dist[j] > dist[id] + w[i]) {
        dist[j] = dist[id] + w[i];
        q.push({dist[j], j});
      }
    }
  }
}
// Bellman Ford
void bf(int k)
{
  int dist[N];
  std::fill(dist, dist + N, INF);
  dist[k] = 0;
  // 迭代n次，每一次都使用上一次的结果
  for (int i = 0; i < N; i++) {
    int prev[N];
    std::copy(dist, dist + N, prev);
    for (int j = 0; j < N; j++) {
      for (int t = he[j]; t != -1; t = ne[t]) {
        int b = e[t];
        // prev[j]+w[t]表示从k到j再到b的距离
        dist[b] = std::min(dist[b], prev[j] + w[t]);
      }
    }
  }
  int ans = 0;
  for (int i = 0; i < N; i++) ans = std::max(ans, dist[i]);
}
// spfa算法，使用邻接表
/*
 初始时将源加入队列。 每次从队列中取出一个元素，
并对所有与他相邻的点进行松弛，若某个相邻的点松弛成功，则将其入队。 直到队列为空时算法结束
*/
void spfa(int k)
{
  std::queue<int> q;
  std::vector<int> dist(N, INF);
  std::vector<bool> inQueue(N, false);

  dist[k] = 0;
  q.push(k);
  inQueue[k] = true;

  while (!q.empty()) {
    int node = q.front();
    q.pop();
    inQueue[node] = false;

    for (int i = he[node]; i != -1; i = ne[i]) {
      int nextNode = e[i];
      int weight = w[i];

      if (dist[nextNode] > dist[node] + weight) {
        dist[nextNode] = dist[node] + weight;

        if (!inQueue[nextNode]) {
          q.push(nextNode);
          inQueue[nextNode] = true;
        }
      }
    }
  }
}