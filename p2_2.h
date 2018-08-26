#pragma once
#ifndef P2_2_H
#define P2_2_H
#include "stdafx.h"
#include "io.h"
#include "data_pair.h"
#include "haffman_node.h"
#include "p2_1.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 


using namespace std;

void HW1_Problem_two_part_two(void);
void Create_Leaves(vector<haffman_node> &tree, vector<data_pair> original_data);
int Count_number_of_unsorted_node(vector<haffman_node> tree);
int Find_the_start_of_no_used_node(vector<haffman_node> tree);
vector<int> Find_Index_of_Two_Nodes_With_Smallest_Weight(vector<haffman_node> tree_in);
haffman_node* Build_Haffman_Tree(vector<haffman_node> &tree_in, vector<data_pair> pdf);




#endif 
