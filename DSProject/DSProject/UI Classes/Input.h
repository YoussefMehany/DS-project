#pragma once
#include<iostream>
#include<string>

using namespace std;

class Input {

public:
	Input();
	void GetFileName(string& Filename);
	void GetInput(int& data);
};