#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "vector.hpp"

using namespace std;

struct sophisticated {
  int* number;
  int index;

  sophisticated() = delete;
  sophisticated(int num, int idx) : number(new int(num)), index(idx) {}
  sophisticated(const sophisticated& other) = delete;
  sophisticated(sophisticated&& other) : number(other.number), index(other.index) {
    other.number = nullptr;
    other.index = 0;
  }

  sophisticated& operator=(const sophisticated& other) = delete;
  sophisticated& operator=(sophisticated&& other) {
    if (&other == this) return *this;
    if (number)
      delete number;
    number = other.number;
    index = other.index;
    other.number = nullptr;
    return *this;
  }

  ~sophisticated() {
    if (number)
      delete number;
  }

  bool operator<(const sophisticated& rhs) const {
    return *number < *rhs.number;
  }

  bool operator==(const sophisticated& rhs) const {
    return *number == *rhs.number && index == rhs.index;
  }
  bool operator!=(const sophisticated& rhs) const {
    return *number != *rhs.number || index != rhs.index;
  }
};

ostream& operator<<(ostream& ostream, const sophisticated& s) {
  ostream << "{" << *s.number << " at " << s.index << "}";
  return ostream; 
}

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
string mismatch_at(size_t i, const T& std_val, const T& my_val) {
  ostringstream oss;
  oss << "mismatch at index " << i << ": ";
  oss << std_val << " in std::vector, " << my_val << " in mine";
  return oss.str();
}

template <typename T>
struct tester {
  Vector<T> my_vec;
  std::vector<T> std_vec;

  void verify() {
    expect(my_vec.size() == std_vec.size(), "should equal");
    for (size_t i = 0; i < my_vec.size(); ++i) {
      expect(my_vec[i] == std_vec[i], mismatch_at(i, std_vec[i], my_vec[i]));
    }
  }

  void push_back(const T& x) {
    my_vec.push_back(x);
    std_vec.push_back(x);
    verify();
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    my_vec.emplace_back(std::forward<Args>(args)...);
    std_vec.emplace_back(std::forward<Args>(args)...);
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
  tester<sophisticated> vec;

  const int n = 5;
  int input[n] = {
    13, -135, 134503, 313, 0
  };
  sophisticated expected[n] = {
    { -135, 1 },
    { 0, 4 },
    { 13, 0 },
    { 313, 3 },
    { 134503, 2 },
  };
  for (int i = 0; i < n; ++i) {
    vec.emplace_back(input[i], i);
  }
  vec.sort();
  for (size_t i = 0; i < n; ++i) {
    expect(vec.my_vec[i] == expected[i], "mismatch");
  }
  cout << "OK! (wait for sanitizers)\n";

  return 0;
}
