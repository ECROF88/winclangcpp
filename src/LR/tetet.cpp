#include <algorithm>
#include <bitset>
#include <iostream>
#include <vector>

using namespace std;

const int MAX_N = 1000;  // 假设最多有1000个点
const int MAX_K = 5;     // 假设最多有5个维度

struct Point {
  int index;
  int coords[MAX_K];  // 存储每个点的K维坐标
};

struct Compare {
  int dimension;  // 当前比较的维度

  void set(int k) { dimension = k; }

  bool operator()(const Point &a, const Point &b) const
  {
    return a.coords[dimension] < b.coords[dimension];
  }
} cmp;

bitset<MAX_N> f[MAX_N];  // 每个点的偏序关系集合
bitset<MAX_N> tmp;       // 用于计算偏序关系的临时bitset

int main()
{
  //   int n, K;
  //   cin >> n >> K;  // 输入点的数量和维度数

  //   vector<Point> points(n);
  //   for (int i = 0; i < n; i++) {
  //     points[i].index = i;
  //     for (int j = 0; j < K; j++) {
  //       cin >> points[i].coords[j];  // 输入每个点的K维坐标
  //     }
  //   }

  //   for (int k = 0; k < K; k++) {
  //     cmp.set(k);                               // 设置当前比较维度
  //     sort(points.begin(), points.end(), cmp);  // 按第k维度排序

  //     tmp.reset();  // 清空 tmp
  //     for (int i = 0; i < n; i++) {
  //       if (k == 0) {
  //         f[points[i].index] = tmp;  // 第一个维度时直接赋值
  //       } else {
  //         f[points[i].index] &= tmp;  // 非第一个维度时按位与操作
  //       }
  //       tmp.set(points[i].index);  // 设置当前点为1
  //     }
  //   }

  //   for (int i = 0; i < n; i++) {
  //     cout << f[i].count() << endl;  // 输出每个点的偏序关系个数
  //   }

  //   return 0;
  std::string s1 = "hello";
  std::string s2 = "world";
  s1.append("abcde", 1).insert(1, "12345", 2).insert(0, 5, '5');
  //   s1.append("abcdefg", 3);

  std::cout << s1 << std::endl;
}