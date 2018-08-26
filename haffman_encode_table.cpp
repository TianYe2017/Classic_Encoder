#include "stdafx.h"
#include "haffman_encode_table.h"
#include <vector>
#include <string>
#include <iostream>  


using namespace std;


vector<haffman_encode_pair> Create_Haffman_Encode_Table_Without_Using_Recursion(vector<haffman_node> processed_nodes_set)
{   
	cout << "start create table..." << endl;
	vector<haffman_encode_pair> haffman_table;
	for (int i = 0; i < processed_nodes_set.size(); i++)
	{
		if (processed_nodes_set[i].is_leaf == true)
		{
			unsigned char current_symbol;
			string code = "";
			haffman_node* p = &(processed_nodes_set[i]);
			current_symbol = p->symbol;
			while ((p->father) != NULL)
			{
				if ((p->left_or_right) == 0)
				{
					code += "0";
				}
				else
				{
					code += "1";
				}
				p = p->father;
			}
			reverse(code.begin(), code.end());
			haffman_table.push_back(haffman_encode_pair(current_symbol, code));
		}
	}
	return haffman_table;
}


string Look_For_Haffman_Table(unsigned char target, vector<haffman_encode_pair> table)
{
	string output = "";
	bool find = false;
	for (int i = 0; i < table.size(); i++)
	{
		if (target == table[i].symbol)
		{
			find = true;
			output = table[i].code;
			break;
		}
	}
	if (find == false)
	{
		cout << "Fatal Error: Can not find the corresponding code to input symbol." << endl;
		while (1);
	}
	return output;
}



