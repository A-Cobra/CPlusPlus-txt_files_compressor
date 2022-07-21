#pragma once
#include "Node.h"
struct comparison {

	bool operator()(Node* left, Node* right) {
		return left->iterations > right->iterations;
	}

};

