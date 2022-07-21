#pragma once
#include<iostream>
struct Node {
	char character;
	int iterations;
	Node* left, * right;
};

Node* setAndGetNode(char character, int iterations, Node* left, Node* right) {
	Node* node = new Node();

	node->character = character;
	node->iterations = iterations;
	node->left = left;
	node->right = right;

	return node;
}
