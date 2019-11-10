struct NullS {};

template <class Head, class Tail>
struct TypeList {
    typedef Head head;
    typedef Tail tail;
};

template <class T, class TList, int N>
struct IndexOf {
	static const int ind = IndexOf<T, class TList::tail, N + 1>::ind;
};

template <class T, int N>
struct IndexOf <T, TypeList <T, NullS>, N > {
	static const int ind = N;
};

template <class T, class Tail, int N>
struct IndexOf <T, TypeList<T, Tail>, N> {
	static const int ind = N;
};

template <class T, class Head, int N>
struct IndexOf <T, TypeList<Head, NullS>, N> {
	static const int ind = -1;
};

template <class T, class TList>
struct getIndexOf {
    static const int res = IndexOf<T, TList, 0>::ind;
};