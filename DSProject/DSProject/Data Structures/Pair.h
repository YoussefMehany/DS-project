#pragma once
template<class T1, class T2>
class Pair {
	T1 first;
	T2 second;
public:
	Pair(T1 fi, T2 se)
		:first(fi), second(se) {}

	T1 getFirst() {
		return first;
	}
	T2 getSecond() {
		return second;
	}
};