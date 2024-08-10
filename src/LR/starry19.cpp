#include <algorithm>
#include <iostream>
#include <vector>

using ll = long long;

int main()
{
  int n, m;
  std::cin >> n >> m;
  std::vector<ll> a(n);
  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  std::sort(a.begin(), a.end());

  int ans = 0;

  for (int i = 0; i < n; i++) {
    int l = i + 1;
    int r = n - 1;
    int pos = n;
    if (l >= r) break;
    while (l < r) {
      int mid = (l + r) / 2;
      if (a[mid] + a[i] > m) {
        pos = mid;
        r = mid - 1;
      } else {
        l = mid + 1;
      }
    }
    ans += n - pos;
  }

  std::cout << ans << '\n';
  return 0;
}