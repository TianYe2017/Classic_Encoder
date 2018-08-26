#pragma once
#ifndef SHANNON_NODE_H
#define SHANNON_NODE_H
#include "stdafx.h"
#include <vector>
#include <string>

using namespace std;

struct shannon_node 
{
	int left_or_right = -1;
	int start_and_end[2] = { -1,-1 };
	shannon_node *left_child = NULL;
	shannon_node *right_child = NULL;
	shannon_node *father = NULL;
	bool is_leaf = false;
	bool is_root = false;
	unsigned char symbol = 0x00;

	//----create a leaf node----//
	shannon_node(unsigned char symbol_in, int index_in, int left_or_right_in) 
	{
		left_or_right = left_or_right_in;
		start_and_end[0] = index_in;
		start_and_end[1] = index_in;
		left_child = NULL;
		right_child = NULL;
		father = NULL;
		is_leaf = true;
		is_root = false;
		symbol = symbol_in;
	}
	//----create a non-leaf-non-root node----//
	shannon_node(int index_left, int index_right, int left_or_right_in)
	{
		left_or_right = left_or_right_in;
		start_and_end[0] = index_left;
		start_and_end[1] = index_right;
		left_child = NULL;
		right_child = NULL;
		father = NULL;
		is_leaf = false;
		is_root = false;
		symbol = 0x00;	
	}
	//----create a root node----//
	shannon_node(int index_left, int index_right) 
	{
		left_or_right = -1;
		start_and_end[0] = index_left;
		start_and_end[1] = index_right;
		left_child = NULL;
		right_child = NULL;
		father = NULL;
		is_leaf = false;
		is_root = true;
		symbol = 0x00;	
	}
};





#endif 
