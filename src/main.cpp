#include "vector.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

void _expect(bool condition, int line, const char* condition_str, const string& msg) {
  if (!condition) {
    cout << "Err on line " << line << " while checking " << condition_str << endl;
    cout << msg << endl;
    exit(1);
  }
}

constexpr size_t kBufferSize = 1000;
char buffer[kBufferSize];
#define expect(cond, msg) _expect(cond, __LINE__, #cond, msg)

template <typename T>
struct tester {
  Vector<T> my_vec;
  std::vector<T> std_vec;

  void verify() {
    expect(my_vec.size() == std_vec.size(), "should equal");
    for (size_t i = 0; i < my_vec.size(); ++i) {
      expect(my_vec[i] == std_vec[i], "mismatch");
    }
  }

  void push_back(const T& x) {
    my_vec.push_back(x);
    std_vec.push_back(x);
    verify();
  }

  void pop_back() {
    expect(!my_vec.empty() && !std_vec.empty(), "should not call pop back on empty");
    my_vec.pop_back();
    std_vec.pop_back();
    verify();
  }

  void set(size_t i, const T& x) {
    my_vec[i] = x;
    std_vec[i] = x;
    verify();
  }

  void sort() {
    std::sort(my_vec.begin(), my_vec.end());
    std::sort(std_vec.begin(), std_vec.end());
    verify();
  }

  bool empty() {
    verify();
    expect(my_vec.empty() == std_vec.empty(), "huh?");
    return my_vec.empty();
  }
};

int main() {
  tester<pair<int, int>> vec;

  int n;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    int x;
    cin >> x;
    vec.push_back(make_pair(x, i));
  }
  vec.sort();
  for (auto [num, idx] : vec.my_vec) {
    cout << num << " at " << idx + 1 << endl;
  }
  while (!vec.empty()) {
    vec.pop_back();
  }
  return 0;
}
