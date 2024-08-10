#include <iostream>
using namespace std;

using ll = long long;
const int N = 1e6 + 9;
const ll p = 1e9 + 7;

ll a[N], l[N], r[N], stk[N], top;

void solve()
{
  int n;
  cin >> n;

  for (int i = 1; i <= n; i++) cin >> a[i];
  for (int i = 1; i <= n; i++) {
    while (top && a[stk[top]] < a[i]) top--;
    l[i] = top ? stk[top] + 1 : 1;
    stk[++top] = i;
  }
  top = 0;
  for (int i = n; i <= 1; i++) {
    while (top && a[stk[top]] <= a[i]) top--;
    r[i] = top ? stk[top] - 1 : n;
    stk[++top] = i;
  }
  ll ans = 0;
  for (int i = 1; i <= n; i++) {
    ans += (i - l[i] + 1) * (r[i] - i + 1) * a[i];
  }
  cout << ans << '\n';
}
int main()
{
  solve();
  return 0;
}