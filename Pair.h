#ifndef PAIR_H_LAB3_SEM3
#define PAIR_H_LAB3_SEM3
#include <utility>
#include <iostream>

template <typename First, typename Second>
struct Pair {
public:
	First first;
	Second second;
public:
	Pair() = default;
	explicit Pair(First _first, Second _second) {
		first = _first;
		second = _second;
	}
	Pair(const Pair<First, Second>& another) {
		first = another.first;
		second = another.second;
	}
	Pair(const std::pair<First, Second>& another) {
		first = another.first;
		second = another.second;
	}
	~Pair() = default;
	Pair& operator=(Pair pair) {
		std::swap(first, pair.first);
		std::swap(second, pair.second);
		return *this;
	}
};
template <typename First, typename Second>
bool operator==(const Pair<First, Second>& one, const Pair<First, Second>& two) {
	if (one.first == two.first && one.second == two.second) {
		return true;
	}
	return false;
}
template <typename First, typename Second>
bool operator!=(const Pair<First, Second>& one, const Pair<First, Second>& two) {
	return !(one == two);
}
template <typename First, typename Second>
std::ostream& operator<<(std::ostream& out, const Pair<First, Second>& pair) {
	out << "[" << pair.first << ", " << pair.second << "]";
	return out;
}

template <typename T>
struct pair_hash {
	size_t operator()(const Pair<T, T>& pair) const {
		std::hash<T> hasher;
		return hasher(pair.first) + hasher(pair.second);
	}
};
template <typename T>
struct pair_unord_equal {
	bool operator()(const Pair<T, T>& p1, const Pair<T, T>& p2) const {
        if ((p1.first == p2.first && p1.second == p2.second) || (p1.first == p2.second && p1.second == p2.first)) {
			return true;
		}
		return false;
	}
};
template <typename T>
struct pair_ord_equal {
	bool operator()(const Pair<T, T>& p1, const Pair<T, T>& p2) const {
		if (p1.first == p2.first && p1.second == p2.second) {
			return true;
		}
		return false;
	}
};
#endif //PAIR_H_LAB3_SEM3
