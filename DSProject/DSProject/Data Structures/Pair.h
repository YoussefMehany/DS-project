#pragma once
template<class T1, class T2>
class Pair {
	T1 First;
	T2 Second;
public:
	Pair()
		:First(0), Second(0) {}
	Pair(T1 fi, T2 se)
		:First(fi), Second(se) {}

	T1 getFirst() {
		return First;
	}
	T2 getSecond() {
		return Second;
	}
	void SetFirst(T1 first) {
		First = first;
	}
	void SetSecond(T1 second) {
		Second = second;
	}
};