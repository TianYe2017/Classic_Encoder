#pragma once

#ifndef HAFFMAN_ENCODE_TABLE_H
#define HAFFMAN_ENCODE_TABLE_H
#include "stdafx.h"
#include <vector>
#include <string>
#include "haffman_node.h"


using namespace std;

struct haffman_encode_pair
{
	string code = "";
	unsigned char symbol = 0x00;

	haffman_encode_pair(unsigned char symbol_in, string code_in)
	{
		code = code_in;
		symbol = symbol_in;
	}
};

vector<haffman_encode_pair> Create_Haffman_Encode_Table_Without_Using_Recursion(vector<haffman_node> processed_nodes_set);
string Look_For_Haffman_Table(unsigned char target, vector<haffman_encode_pair> table);


#endif 