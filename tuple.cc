// Author: Valeriy Ovchinnikov
// Key string: kremsnx@yandex.ru - 5

#include <iostream>

template<typename ... Types> class Tuple;
template<> class Tuple<> {};
template<typename First, typename ... Rest>
class Tuple<First, Rest...>: private Tuple<Rest...> {
  First Member;
 public:
  Tuple(const First& first, const Rest& ... rest):
      Tuple<Rest...>(rest...), Member(first) {}
  
  const First& Head() const {
    return Member;
  }
  
  const Tuple<Rest...>& Tail() const {
    return *this;
  }
};


// As a return value of Get<>(Tuple) method is not a reference,
// in order to make it available to return POD types.

template<size_t I, class T>
struct TupleElement;

template<size_t I, class Head, class ... Rest>
struct TupleElement<I, Tuple<Head, Rest...> >:
    public TupleElement<I - 1, Tuple<Rest...> > {
  static typename TupleElement<I, Tuple<Head, Rest...> >::Type
      Get(const Tuple<Head, Rest...>& t) {
    return TupleElement<I - 1, Tuple<Rest...> >::Get(t.Tail());
  }
};
 
template<class Head, class ... Rest>
struct TupleElement<0, Tuple<Head, Rest...> > {
  typedef Head Type;
  static typename TupleElement<0, Tuple<Head, Rest...> >::Type
      Get(const Tuple<Head, Rest...>& t) {
    return t.Head();
  }
};

template<size_t Pos, class Head, class ... Rest>
typename TupleElement<Pos, Tuple<Head, Rest...> >::Type
    Get(const Tuple<Head, Rest...>& t) {
  return TupleElement<Pos, Tuple<Head, Rest...> >::Get(t);
}

int main() {
  Tuple<int, double, char> t(42, 3.14, 'a');
  std::cout << Get<0>(t) << std::endl;
  std::cout << Get<1>(t) << std::endl;
  std::cout << Get<2>(t) << std::endl;
  return 0;
}
