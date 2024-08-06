// imports
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <memory>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <exception>
#include <thread>
#include <future>
#include <chrono>
#include <random>
#include <variant>
#include <queue>
#include <bitset>
#include <iterator>
#include <stack>
#include <utility>
#include <limits>
#include <climits>
#include <bits/stdc++.h>
#include <any>

using namespace std;

/**
 * For `printf`ing to `stderr`.
 */
void eprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
}

/**
 * Prints debug messages when `-DDEBUG` is added.
 */
void _dprintf(const char *format, ...) {
#ifdef DEBUG
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
#endif
}

/**
 * Creates a Node that can point to or be pointed by another Node, and even both.
 *
```cpp
int a = 10;
Node *node_a = new Node(&a);

double b = 5;
std::unique_ptr<Node> node_b = std::make_unique<Node>(&b);

node_a -> next = node_b;
node_b -> prev = node_a;

cout << *(int *)(node_a -> data) << endl;
cout << *(double *)(node_b -> data) << endl;

delete node_a;
node_a = NULL;
node_b = nullptr;
```
 */
struct Node {
  Node *prev;
  any data;
  Node *next;
  Node(any data): prev(nullptr), data(data), next(nullptr) {
    _dprintf("DEBUG: Node created with data %p.\n", data);
  }
};

/**
Rust-inspired structure.
Contains either an okay value or an error value.

```cpp
Result<int, std::string> a = Result::Ok(10); // okay-y
Result<int, std::string> b = Result::Err("uh oh"); // error-y

Result<std::string, std::string> c = Result::Ok("hello"); // okay-y
Result<std::string, std::string> d = Result::Err("hello"); // error-y
```
 */
template <typename T, typename E, typename = std::enable_if_t<!std::is_same_v<T, E>>>
class Result {
public:
  static Result<T, E> Ok(T value) {
    _dprintf("DEBUG: Ok created with value %p.\n", value);
    return Result(std::in_place_index<0>, value);
  }
  static Result<T, E> Err(E error) {
    _dprintf("DEBUG: Err created with error %p.\n", error);
    return Result(std::in_place_index<1>, error);
  }
  bool is_ok() const {
    _dprintf("DEBUG: is_ok called.\n");
    return std::holds_alternative<T>(data);
  }
  bool is_err() const {
    _dprintf("DEBUG: is_err called.\n");
    return std::holds_alternative<E>(data);
  }
  T unwrap() const {
    _dprintf("DEBUG: unwrap called.\n");
    if (is_ok()) {
      _dprintf("DEBUG: unwrap returned %p.\n", std::get<T>(data));
      return std::get<T>(data);
    }
    _dprintf("DEBUG: unwrap threw an error: Err = %p.\n", std::get<E>(data));
    throw std::runtime_error("Called unwrap on an Err value");
  }
  E unwrap_err() const {
    _dprintf("DEBUG: unwrap_err called.\n");
    if (is_err()) {
      _dprintf("DEBUG: unwrap_err returned %p.\n", std::get<E>(data));
      return std::get<E>(data);
    }
    _dprintf("DEBUG: unwrap_err threw an error: Ok = %p.\n", std::get<T>(data));
    throw std::runtime_error("Called unwrap_err on an Ok value");
  }
private:
  std::variant<T, E> data;
  template <typename U>
  Result(U &&value) : data(std::forward<U>(value)) {
    _dprintf("DEBUG: Result created with value %p.\n", value);
  }
};

/**
Rust-inspired structure.
Either contains a value or does not.

```cpp
Option<int> a = Option::Some(10); // some-y
Option<int> b = Option::None(); // none-y
```
 */
template<typename T>
class Option {
public:
  Option() : has_value(false) {}
  Option(T value) : value(value), has_value(true) {}
  Option(const Option &other) : value(other.value), has_value(other.has_value) {}
  Option(Option &&other) : value(std::move(other.value)), has_value(other.has_value) {}
  Option& operator=(const Option &other) {
    value = other.value;
    has_value = other.has_value;
    return *this;
  }
  Option& operator=(Option &&other) {
    value = std::move(other.value);
    has_value = other.has_value;
    return *this;
  }
  static Option<T> Some(T value) {
    return Option(value);
  }
  static Option<T> None() {
    return Option();
  }
  bool is_some() const {
    return has_value;
  }
  bool is_none() const {
    return !has_value;
  }
  T unwrap() const {
    if (has_value) {
      return value;
    }
    throw std::runtime_error("Called unwrap on a None value");
  }
private:
  T value;
  bool has_value;
};

/**
 * Frees a pointer.
```cpp
double *pi = (double *)malloc(sizeof(double));
*pi = 3.14;
printf("PI: %.2f\n", pi);
custom_free((void **)&pi);
```
 */
int custom_free(void **pointer) {
  if (pointer == NULL || *pointer == NULL) {
    _dprintf("%p is NULL.\n", pointer);
    eprintf("ERROR:\n\t%p = 0x0;\tpointer is NULL.\n", pointer);
    return -1;
  }
  _dprintf("DEBUG: Pointer %p will be freed...\n", pointer);
  free(*pointer);
  *pointer = NULL;
  _dprintf("DEBUG: Value of %p is freed and is set to NULL.\n", pointer);
  return 0;
}

/**
 * Prints a pointer with its contents.
 * @param ptr Pointer that points to a set of hex values.
 * @param size Number of bytes to print.
 */
void memory_printf(const void *ptr, size_t size) {
  const unsigned char *byte_ptr = static_cast<const unsigned char *>(ptr);
  cout << hex << setfill('0') << setw(8) << static_cast <const void*> (byte_ptr) << ": ";
  for (size_t i = 0; i < size; ++i) {
    if (i % 16 == 0) {
      cout << '\n' << hex << setfill('0') << setw(8) << static_cast <const void*> (byte_ptr + i) << ": ";
    }
    cout << hex << setfill('0') << setw(2) << static_cast <int> (byte_ptr[i]) << " ";
  }
  cout << dec << '\n';
}

/**
 * Generates a random decimal between 0 (inclusive) and 1 (exclusive).
 */
double custom_rand() {
  int rand1 = rand();
  _dprintf("DEBUG: Randomly generated number: %i.\n", rand1);
  double rand2 = static_cast < double > (rand1) / static_cast < double > (RAND_MAX);
  _dprintf("DEBUG: Random number from 0 to 1: %.54lf.\n", rand2);
  return rand2;
}
/**
 * Shorter `getline(cin, <string>)`.
 */
string ask(string question = "") {
  string input;
  cout << question;
  fflush(stdout);
  _dprintf("DEBUG: Asked %s and managed to flush stdout.\n", question.c_str());
  if (getline(cin, input)) {
    _dprintf("DEBUG: Got input: %s.\n", input.c_str());
    return input;
  } else {
    _dprintf("DEBUG: Error getting input.\n");
    throw runtime_error("Error getting input.");
  }
}

/**
 * Convert `string` into different types using `istringstream`.
 */
template <typename T>
T into(const string &input) {
  istringstream stream(input);
  T value;
  _dprintf("DEBUG: Converting %s into %s.\n", input.c_str(), typeid(value).name());
  stream >> value;
  if (stream.fail()) {
    _dprintf("DEBUG: Conversion failed.\n");
    throw runtime_error("Conversion failed.");
  }
  _dprintf("DEBUG: Conversion successful.\n");
  return value;
}

/**
 * Structure for `istringstream`.
 */
typedef struct InputStringStream {
  istringstream iss;
  InputStringStream(const string &input): iss(input) {}
  int set(const string &input) {
    if (input.empty()) {
      cerr << "Input string is empty.\n";
      return -1;
    }
    iss.str(input);
    iss.clear();
    iss.seekg(0, ios::beg);
    return 0;
  }
  bool failed() {
    return iss.fail();
  }
  bool ended() {
    return iss.eof();
  }
  string read(size_t bytes = string::npos) {
    if (bytes == string::npos) {
      bytes = iss.str().size() - iss.tellg();
    }
    unique_ptr<char[]> buffer(new char[bytes]);
    iss.read(buffer.get(), bytes);
    string data = string(buffer.get(), iss.gcount());
    return data;
  }
  template <typename T>
    InputStringStream& operator >> (T &value) {
      if (!(iss >> value)) {
        cerr << "Extraction from InputStringStream failed.\n";
        throw runtime_error("Extraction failed.");
      }
      return *this;
    }
}
ISS;

/**
 * Structure for `ostringstream`.
 */
typedef struct OutputStringStream {
  ostringstream oss;
  OutputStringStream(): oss() {}
  int write(const string &data) {
    if (data.empty()) {
      cerr << "Data string is empty.\n";
      return -1;
    }
    oss << data;
    return 0;
  }
  string get(size_t bytes = string::npos) {
    const size_t size = oss.str().size();
    if (bytes == string::npos || bytes > size) {
      bytes = size;
    }
    return oss.str().substr(0, bytes);
  }
  template <typename T>
    OutputStringStream& operator << (const T &value) {
      oss << value;
      return * this;
    }
}
OSS;

/**
 * Structure for `stringstream` acting as a TransformStream.
 */
typedef struct StringStream {
  InputStringStream iss;
  OutputStringStream oss;
  StringStream(const string &input = ""): iss(input), oss() {}
  int transform(function <string(const string&)> func, size_t bytes = string::npos) {
    while (!iss.ended()) {
      string data = iss.read(bytes);
      if (!data.empty()) {
        oss.write(func(data));
      }
    }
    return 0;
  }
  template <typename T>
    StringStream& operator << (const T &value) {
      oss << value;
      return *this;
    }
  template <typename T>
    StringStream& operator >> (T &value) {
      try {
        iss >> value;
      } catch (const runtime_error & e) {
        cerr << e.what() << '\n';
      }
      return * this;
    }
} SS;

template <typename T>
  /**
   * Slicing for arrays.
   */
  vector<T> slice(const T *arr, size_t start, size_t end) {
    if (end > size(arr) || start > end) {
      throw out_of_range("Invalid slice indices");
    }
    _dprintf("DEBUG: Slicing array from %i to %i.\n", start, end);
    return vector<T>(arr + start, arr + end);
  }

template <typename T>
  /**
   * Slicing for vectors.
   */
  vector<T> slice(const vector<T> &vec, size_t start, size_t end) {
    if (end > vec.size() || start > end) {
      throw out_of_range("Invalid slice indices");
    }
    _dprintf("DEBUG: Slicing vector from %i to %i.\n", start, end);
    return vector <T>(vec.begin() + start, vec.begin() + end);
  }

/**
 * Constructs a new `File` object used for 
 */
class File {
  private: string filename;
  ifstream read_stream;
  ofstream write_stream;

  public: File(const char *name): filename(name) {
      write_stream.open(filename);
      read_stream.open(filename);
      _dprintf("DEBUG: File %s created.\n", filename.c_str());
    }
    ~File() {
      if (read_stream.is_open()) {
        read_stream.close();
      }
      if (write_stream.is_open()) {
        write_stream.close();
      }
      _dprintf("DEBUG: File %s closed.\n", filename.c_str());
    }
  friend ofstream & operator << (File &file,
    const string & content) {
    if (!file.write_stream.is_open()) {
      throw runtime_error("Could not open file for writing.");
    }
    file.write_stream << content;
    _dprintf("DEBUG: Wrote %s to file %s.\n", content.c_str(), file.filename.c_str());
    return file.write_stream;
  }
  friend ifstream & operator >> (File &file, string &value) {
    if (!file.read_stream.is_open()) {
      throw runtime_error("Could not open file for reading.");
    }
    file.read_stream >> value;
    _dprintf("DEBUG: Read %s from file %s.\n", value.c_str(), file.filename.c_str());
    return file.read_stream;
  }
};

template <typename T>
  /**
   * Creates an instance with methods like `jest` to create tests.
   */
  class Expectation {
    private: T value;
    public: Expectation(const T &val): value(val) {
      _dprintf("DEBUG: Expectation created with value %p.\n", value);
    }
    bool eq(const T & expected) const {
      return value == expected;
    }
    bool nully() {
      return value == (T)NULL;
    }
    bool gt(const T & val) {
      return value > val;
    }
    bool lt(const T & val) {
      return value < val;
    }
    bool ge(const T & val) {
      return value >= val;
    }
    bool le(const T & val) {
      return value <= val;
    }
    bool has(const T & val) {
      for (T item: value) {
        if (item == val) {
          return true;
        }
      }
      return false;
    }
    T get() const {
      return value;
    }
  };

template <typename T>
  /**
   * Shorthand way of creating an `Expectation` object, like in `jest`.
   */
  Expectation <T> expect(const T & value) {
    Expectation <T> expectation(value);
    return expectation;
  }

/**
 * This function calculates the hash value of a given string.
 * The hash value is calculated using the formula: `hash = (hash * prime + char) % modulo` where hash is the current hash value, prime is a large prime number,
 * char is the current character of the string, and modulo is a large number.
 * The default values for prime and modulo are 1000000007 and 1000000000000000007 respectively.
 *
 * @param str The string to hash.
 * @param prime The prime number to use. Default value is 1000000007.
 * @param modulo The modulo number to use. Default value is 1000000000000000007.
 * @returns The hash of the string.
 */
unsigned long long _hash(const char *str, unsigned long long prime = 1000000007, unsigned long long modulo = 1000000000000000007) {
  unsigned long long _hash = 0;
  for (; *str; str++) {
    _dprintf("Old hash: %llu\n", _hash);
    _dprintf("Character: %c, Code point: %d\n", *str, *str);
    _hash = (_hash * prime + *str) % modulo;
    _dprintf("New hash: %llu\n", _hash);
  }
  return _hash;
}

/**
 * The main function.
 */
int _main(int & argc, char * argv[]) {
  char a[1 << 20];
  scanf("%s", a);
  cout << _hash(a) << endl;

  return 0;
}
int main(int argc, char * argv[]) {
  const clock_t start = clock();

  srand(time(0));

  // main
  const int code = _main(argc, argv);

  cout << endl;
  const clock_t end = clock();
  double performance = double(end - start) / CLOCKS_PER_SEC;
  printf("Code: %.6lfs", performance);
  cout << endl;
  return code;
}
