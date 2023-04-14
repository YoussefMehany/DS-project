#pragma once
template<class T>
class Pair {
	T first, second;
public:
	Pair(T fi, T se)
		:first(fi), second(se) {}

	T getFirst() {
		return first;
	}
	T getSecond() {
		return second;
	}
};