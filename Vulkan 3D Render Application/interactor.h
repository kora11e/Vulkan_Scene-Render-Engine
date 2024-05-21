#pragma once
#include "lve_window.h"

class interactor {
public:
	interactor();
	~interactor();
	void printer(int num);
private:
	int number;
	const char* string;
};