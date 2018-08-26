#pragma once
#ifndef HAFFMAN_NODE_H
#define HAFFMAN_NODE_H
#include "stdafx.h"
#include <vector>
#include <string>

using namespace std;

struct haffman_node
{
	int left_or_right = -1;
	int weight = -1;
	haffman_node *left_child = NULL;
	haffman_node *right_child = NULL;
	haffman_node *father = NULL;
	bool is_leaf = false;
	bool is_root = false;
	bool used = false;
	unsigned char symbol = 0x00;
	bool sorted = false;
	

	//----create a common node----//
	haffman_node() 
	{
		left_or_right = -1;
		weight = -1;
		left_child = NULL;
		right_child = NULL;
		father = NULL;
		is_leaf = false;
		is_root = false;
		used = false;
		symbol = 0x00;
		sorted = false;
	}
};





#endif 