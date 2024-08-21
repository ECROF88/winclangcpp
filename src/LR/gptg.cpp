#include <algorithm>  // 用于标准算法函数，如 sort
#include <bitset>     // 用于 bitset 类，位运算
#include <cmath>      // 用于数学函数，如 sqrt
#include <cstdio>     // 用于输入输出函数，如 getchar, printf
#include <cstring>    // 用于内存操作函数，如 memset

// 高效的读入函数，读取一个整数
int read()
{
  int out(0), c(getchar());  // 初始化结果变量 out 和字符 c
  // 跳过所有非数字字符
  for (; c < '0' || c > '9'; c = getchar());
  // 读取数字字符并转换成整数
  for (; c <= '9' && c >= '0'; c = getchar())
    out = (out << 1) + (out << 3) + (c ^ 48);  // 等价于 out = out * 10 + (c - '0')
  return out;                                  // 返回读取的整数
}

const int MAXN = 4e4 + 10;  // 最大点数
const int MAXB = 233;       // 分块大小的上限
const int MAXK = 7;         // 维度的最大数

int nowk;  // 当前处理的维度

// 点的结构体，包含 K 维坐标，定义了一些运算符
struct P {
  int a[MAXK];  // 存储点在每个维度的坐标
  // 重载小于运算符，用于排序
  bool operator<(const P &rv) const
  {
    return a[nowk] < rv.a[nowk];  // 按当前维度 nowk 进行比较
  }
  // 重载下标运算符，用于获取坐标值
  int operator[](const int &x) const { return a[x]; }
  // 重载下标运算符，用于设置坐标值
  int &operator[](const int &x) { return a[x]; }
} p[MAXN];  // 点的数组

int idx[MAXK][MAXN];  // 存储点在每个维度上的索引

int N, K, B;                       // N: 点的数量, K: 维度数量, B: 分块大小
std::bitset<MAXN> bs[MAXK][MAXB];  // 每个维度每个块的 bitset，表示点的索引
std::bitset<MAXN> res, tmp;        // 中间结果和临时变量

// 获取满足条件的点集：第 k 维度上坐标 <= v 的点
inline std::bitset<MAXN> get(int k, int v)
{
  int b = v / B - 1;           // 计算块编号，b 是块的索引
  std::bitset<MAXN> res(0);    // 初始化结果 bitset 为 0
  if (b >= 0) res = bs[k][b];  // 获取前 b 个块的所有点集
  // 合并第 b+1 块内坐标 <= v 的点
  for (int x = v / B * B; x <= v; ++x) res.set(idx[k][x]);  // 设置相应位为 1
  return res;                                               // 返回结果
}

int main()
{
  // 重定向输入输出文件
  freopen("partial_order_plus.in", "r", stdin);
  freopen("partial_order_plus.out", "w", stdout);

  // 读取点的数量 N 和维度数量 K
  N = read(), K = read();
  // 读入每个点在各个维度上的坐标
  for (int k = 1; k <= K; ++k)
    for (int i = 0; i < N; ++i) p[i][k] = read() - 1;  // 坐标从 0 开始计数
  for (int i = 0; i < N; ++i) p[i][0] = i;  // 第 0 维存储点的原始索引

  memset(idx, 0xff, sizeof(idx));  // 初始化 idx 数组为 -1
  B = static_cast<int>(sqrt(N));   // 根据 N 计算分块大小 B

  // 对每个维度进行排序和预处理
  for (nowk = 0; nowk <= K; ++nowk) {
    std::sort(p, p + N);  // 按照当前维度 nowk 对点进行排序
    for (int i = 0; i < N; ++i) {
      // 记录排序后的索引位置
      idx[nowk][p[i][nowk]] =
          p[i][0];  // 保存第 nowk 维上坐标为 p[i][nowk] 的点的原始索引
      bs[nowk][p[i][nowk] / B].set(p[i][0]);  // 将点加入到对应块的 bitset 中
    }
    // 处理每个块的累积 bitset
    for (int b = 1; b * B < N; ++b)
      bs[nowk][b] |= bs[nowk][b - 1];  // 当前块包含前一块的所有点集
  }

  long long ans = 0;  // 最终答案
  // 对每个点计算其部分序关系
  for (int i = 0; i < N; ++i) {
    res.set();  // 初始化为全 1，表示所有点都满足条件
    for (int k = 0; k <= K; ++k) res &= get(k, p[i][k] - 1);  // 与操作，求交集
    ans += res.count();  // 计算满足条件的点的数量，并累加
  }
  // 输出最终结果
  printf("%lld\n", ans);
}
