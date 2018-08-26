#include "stdafx.h"
#include "p2_2.h"
#include "haffman_encode_table.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 



using namespace std;


void Create_Leaves(vector<haffman_node> &tree, vector<data_pair> original_data) 
{   
	int length = 2 * original_data.size() - 1;
	for (int i = 0; i < length; i++)
	{
		haffman_node node = haffman_node();
		tree.push_back(node);
	}
	for (int i = 0; i < original_data.size(); i++) 
	{
		tree[i].is_leaf = true;
		tree[i].symbol = original_data[i].index;
		tree[i].used = true;
		tree[i].weight = original_data[i].freq;
	}
	cout << "size_of_tree: " << tree.size() << endl;
}

int Count_number_of_unsorted_node(vector<haffman_node> tree) 
{    
	int out = 0;
	for (vector<haffman_node>::iterator it = tree.begin(); it != tree.end(); it++)
	{
		if (it->sorted == false && it->used == true) 
		{
			out++;
		}
	}	
	cout << out << endl;
	return out;
}

int Find_the_start_of_no_used_node(vector<haffman_node> tree) 
{   
	int index = 0;
	for (vector<haffman_node>::iterator it = tree.begin(); it != tree.end(); it++) 
	{
		if (it->used == false) 
		{
			return index;
		}
		index++;
	}
	return -1;
}

vector<int> Find_Index_of_Two_Nodes_With_Smallest_Weight(vector<haffman_node> tree_in)
{
	vector<int> out;
	int left = -1;
	int right = -1;
	int current = 99999999;
	int index = 0;
	for (vector<haffman_node>::iterator it = tree_in.begin(); it != tree_in.end(); it++)
	{  
		if (it->sorted == false && it->used == true)
		{
			if (it->weight <= current)
			{
				left = index;
				current = it->weight;
			}		
		}//ingnore bulit tree	
		index++;
	}//find min
	current = 99999999;
	index = 0;
	for (vector<haffman_node>::iterator it = tree_in.begin(); it != tree_in.end(); it++)
	{   
		if (it->sorted == false && it->used == true)
		{
			if (it->weight <= current && index != left)
			{
				right = index;
				current = it->weight;
			}		
		}//ingnore bulit tree	
		index++;
	}//find second min 
	out.push_back(left);
	out.push_back(right);
	//cout << out[0] << " " << out[1] << endl;
	return out;
}


haffman_node* Build_Haffman_Tree(vector<haffman_node> &tree_in, vector<data_pair> pdf) 
{   
	cout << "building haffman tree..." << endl;
	int sum = 0;
	for (vector<haffman_node>::iterator it = tree_in.begin(); it != tree_in.end(); it++)
	{
		if(it->used == true)
		{
			sum += it->weight;
		}	
	}
	haffman_node* root_out = NULL;
	while (Find_the_start_of_no_used_node(tree_in)!= -1)
	{   
		int str_no_used = Find_the_start_of_no_used_node(tree_in);
		int count = Count_number_of_unsorted_node(tree_in);
		//cout << tree_in.size() << endl;
		//cout << count << endl;
		vector<int> index = Find_Index_of_Two_Nodes_With_Smallest_Weight(tree_in);
		if (count == 2) 
		{   
			int new_weight = tree_in[index[0]].weight + tree_in[index[1]].weight;
			if (new_weight != sum)
			{
				cout << "Fatal Error: Weight of root does not equal to sum of weights of nodes." << endl;
			}
			tree_in[str_no_used].left_child = &(tree_in[index[0]]);
			tree_in[str_no_used].right_child = &(tree_in[index[1]]);
			tree_in[str_no_used].weight = new_weight;
			tree_in[str_no_used].used = true;
			tree_in[str_no_used].is_root = true;
			tree_in[index[0]].father = &(tree_in[str_no_used]);
			tree_in[index[0]].left_or_right = 0;
			tree_in[index[0]].sorted = true;
			tree_in[index[1]].father = &(tree_in[str_no_used]);
			tree_in[index[1]].left_or_right = 1;
			tree_in[index[1]].sorted = true;
			root_out = &(tree_in[str_no_used]);
		}
		else 
		{
			int new_weight = tree_in[index[0]].weight + tree_in[index[1]].weight;
			tree_in[str_no_used].left_child = &tree_in[index[0]];
			tree_in[str_no_used].right_child = &tree_in[index[1]];
			tree_in[str_no_used].weight = new_weight;
			tree_in[str_no_used].used = true;
			tree_in[index[0]].father = &tree_in[str_no_used];
			tree_in[index[0]].left_or_right = 0;
			tree_in[index[0]].sorted = true;
			tree_in[index[1]].father = &tree_in[str_no_used];
			tree_in[index[1]].left_or_right = 1;
			tree_in[index[1]].sorted = true;
		}	
	}
	//basic check
	bool pass = true;
	for (vector<haffman_node>::iterator it = tree_in.begin(); it != tree_in.end(); it++)
	{
		if (it->is_leaf == true) 
		{
			if ((it->left_child != NULL) | (it->right_child != NULL)) 
			{
				cout << "Fatal Error: leaf node should not has child pointer." << endl;
				pass = false;
				
			}
			if (it->father == NULL) 
			{
				cout << "Fatal Error: Can not find father node for leaf." << endl;
				pass = false;

			}
		}
		else 
		{  
			if (it->is_root != true) 
			{
				if (it->left_child == NULL | it->right_child == NULL)
				{
					cout << "Fatal Error: Can not find child pointer for non-leaf node." << endl;
					pass = false;
				}
				if (it->father == NULL)
				{
					cout << "Fatal Error: Can not find father node for non-root node." << endl;
					pass = false;
				}
			}
			else 
			{
				if (it->left_child == NULL | it->right_child == NULL)
				{
					cout << "Fatal Error: Can not find child pointer for root node." << endl;
					pass = false;
				}
				if (it->father != NULL)
				{
					cout << "Fatal Error: Root node should not has father node." << endl;
					pass = false;
				}
			}		
		}
	}
	cout << "building complete." << endl;
	/*for (int i = 0; i < tree_in.size(); i++) 
	{
		haffman_node* p = &tree_in[i];
		if (p->is_leaf == true) 
		{   
			string temp = "";
			unsigned char c_symbol = p->symbol;
			while (p->father != NULL) 
			{   
				if ((p->left_or_right) == 0)
				{
					temp += "0";
				}
				else
				{
					temp += "1";
				}
				p = p->father;
			}
			cout << (int)c_symbol << " " << temp << endl;
		}
	}
	cout << "number of nodes: " << tree_in.size() << endl;
	for (vector<haffman_node>::iterator it = tree_in.begin(); it != tree_in.end(); it++) 
	{
		cout << (int)it->symbol << " " << it->weight << " " << it->sorted << " " << it->used << endl;
	}*/
	return root_out;
}


void HW1_Problem_two_part_two(void)
{
	//-----------------step#1 read four .dat files-----------------//
	string file_audio = "E:\\MY_LOG\\669\\hw1\\input\\EE669_Fall_2017_HW1_Data_Files\\audio.dat";
	string file_binary = "E:\\MY_LOG\\669\\hw1\\input\\EE669_Fall_2017_HW1_Data_Files\\binary.dat";
	string file_image = "E:\\MY_LOG\\669\\hw1\\input\\EE669_Fall_2017_HW1_Data_Files\\image.dat";
	string file_text = "E:\\MY_LOG\\669\\hw1\\input\\EE669_Fall_2017_HW1_Data_Files\\text.dat";
	vector<unsigned char> audio = ReadBinaryFile(file_audio);
	vector<unsigned char> binary = ReadBinaryFile(file_binary);
	vector<unsigned char> image = ReadBinaryFile(file_image);
	vector<unsigned char> text = ReadBinaryFile(file_text);
	vector<unsigned char> global;
	for (vector<unsigned char>::iterator it = audio.begin(); it != audio.end(); it++)
	{
		global.push_back(*it);
	}
	for (vector<unsigned char>::iterator it = binary.begin(); it != binary.end(); it++)
	{
		global.push_back(*it);
	}
	for (vector<unsigned char>::iterator it = image.begin(); it != image.end(); it++)
	{
		global.push_back(*it);
	}
	for (vector<unsigned char>::iterator it = text.begin(); it != text.end(); it++)
	{
		global.push_back(*it);
	}
	int sum = global.size();
	//-----------------step#2 transfer four .dat files to pdf-----------------//
	cout << "start sorting..." << endl;
	vector<data_pair> audio_pdf = Sort_Raw_Data(audio);
	cout << "4 left" << endl;
	vector<data_pair> binary_pdf = Sort_Raw_Data(binary);
	cout << "3 left" << endl;
	vector<data_pair> image_pdf = Sort_Raw_Data(image);
	cout << "2 left" << endl;
	vector<data_pair> text_pdf = Sort_Raw_Data(text);
	cout << "1 left" << endl;
	//vector<data_pair> global_pdf = Sort_Raw_Data(global);
	//cout << "done." << endl;
	//for (vector<data_pair>::iterator it = global_pdf.begin(); it != global_pdf.end(); it++)
	//{
		//cout << (int)it->index << "  " << (double)it->freq / (double)sum << endl;
	//}
	//vector<data_pair> Symbol = Sort_Raw_Data_Symbol(global);
	//-----------------step#3 compute entropy of four .dat files-----------------//
	double entropy_audio = Compute_Entropy(audio_pdf);
	double entropy_binary = Compute_Entropy(binary_pdf);
	double entropy_image = Compute_Entropy(image_pdf);
	double entropy_text = Compute_Entropy(text_pdf);
	cout << "audio: " << entropy_audio << endl;
	cout << "binary: " << entropy_binary << endl;
	cout << "image: " << entropy_image << endl;
	cout << "text: " << entropy_text << endl;
	//-----------------step#4 build haffman tree-----------------//
	vector<haffman_node> tree;
	Create_Leaves(tree, text_pdf);
	//Find_Index_of_Two_Nodes_With_Smallest_Weight(tree);
	haffman_node* haffman_tree_output = Build_Haffman_Tree(tree, text_pdf);
	cout << "size of haffman tree is: " << tree.size() << endl;


	//-----------------step#5 create encode table-----------------//
	vector<haffman_encode_pair> Table_haffman = Create_Haffman_Encode_Table_Without_Using_Recursion(tree);
	for (vector<haffman_encode_pair>::iterator it = Table_haffman.begin(); it != Table_haffman.end(); it++)
	{
		cout << (int)it->symbol << "  " << it->code << endl;
	}
	//-----------------step#7 Use Table To encode data-----------------//
	string outcome_audio = "";
	string outcome_binary = "";
	string outcome_image = "";
	string outcome_text = "";
	/*cout << "rebuild haffman audio..." << endl;
	for (vector<unsigned char>::iterator it = audio.begin(); it != audio.end(); it++)
	{
		outcome_audio += Look_For_Haffman_Table(*it, Table_haffman);
	}*/
	//cout << outcome_audio << endl;
	/*cout << "rebuild haffman binary..." << endl;
	for (vector<unsigned char>::iterator it = binary.begin(); it != binary.end(); it++)
	{
		outcome_binary += Look_For_Haffman_Table(*it, Table_haffman);
	}*/
	////cout << outcome_binary << endl;
	/*cout << "rebuild haffman image..." << endl;
	for (vector<unsigned char>::iterator it = image.begin(); it != image.end(); it++)
	{
		outcome_image += Look_For_Haffman_Table(*it, Table_haffman);
	}*/
	////cout << outcome_image << endl;
	cout << "rebuild haffman text..." << endl;
	for (vector<unsigned char>::iterator it = text.begin(); it != text.end(); it++)
	{
		outcome_text += Look_For_Haffman_Table(*it, Table_haffman);
	}
	//cout << outcome_image << endl;
	//-----------------step#8 output new file to OS-----------------//
	//Write_String_To_Dat(outcome_audio, "E:\\MY_LOG\\669\\hw1\\output\\new_audio_haffman.dat");
	//Write_String_To_Dat(outcome_binary, "E:\\MY_LOG\\669\\hw1\\output\\new_binary_haffman.dat");
	//Write_String_To_Dat(outcome_image, "E:\\MY_LOG\\669\\hw1\\output\\new_image_haffman.dat");
	Write_String_To_Dat(outcome_text, "E:\\MY_LOG\\669\\hw1\\output\\new_text_haffman.dat");

	//*for (vector<data_pair>::iterator it = Symbol.begin(); it != Symbol.end(); it++)
	//{
	//cout << (int)it->index << "  " << it->freq << endl;
	//}*/





	















}

