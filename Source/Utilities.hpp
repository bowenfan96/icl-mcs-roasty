#include <functional>

template <typename Result> struct RangeGenerator {
  std::function<Result&(size_t)> generator;
  size_t count;
  struct Iterator : public std::iterator<std::forward_iterator_tag, Result> {
    std::function<Result&(size_t)> generator;
    size_t i;
    Iterator(std::function<Result&(size_t)> generator, size_t i) : generator(generator), i(i) {}
    bool operator==(Iterator const& other) const { return i == other.i; }
    bool operator!=(Iterator const& other) const { return i != other.i; }
    Iterator& operator++() {
      ++i;
      return *this;
    }
    Result* operator->() { return &generator(i); }
    Result& operator*() { return generator(i); }
  };
  RangeGenerator(std::function<Result&(size_t)> generator, size_t count)
      : generator(generator), count(count) {}
  Iterator begin() { return Iterator(generator, 0); };
  Iterator end() { return Iterator(generator, count); };
};
