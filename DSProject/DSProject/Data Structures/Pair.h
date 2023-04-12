#pragma once

class Pair {
	int first, second;
public:
	Pair(int fi, int se)
		:first(fi), second(se) {}

	int getFirst() {
		return first;
	}
	int getSecond() {
		return second;
	}
};