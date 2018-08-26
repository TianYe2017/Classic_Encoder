#include "stdafx.h"
#include "p2_1.h"
#include "shannon_node.h"
#include "shannon_encode_table.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 


using namespace std;


double Compute_Entropy(vector<data_pair> input) 
{
	double entropy = 0.;
	double sum = 0.;
	for (vector<data_pair>::iterator it = input.begin(); it != input.end(); it++)
	{
		sum += it->freq;
	}
	for (vector<data_pair>::iterator it = input.begin(); it != input.end(); it++)
	{
	  entropy += -(it->freq / sum) * log2(it->freq / sum);
	}
	return entropy;
}


int Find_Split_Pos(vector<data_pair> pdf, int st, int end) 
{   
	//cout << st << endl;
	int pos = st;
	int difference = 99999999999;
	for (int i = st; i < end + 1; i++) 
	{
		int sum_left = 0;
		int sum_right = 0;
		for (int j = st; j < i + 1; j++) 
		{
			sum_left += pdf[j].freq;
		}
		for (int j = i + 1; j < end + 1; j++)
		{
			sum_right += pdf[j].freq;
		}
		//printf("sum_left %d \n", sum_left - sum_right);
		if (abs(sum_left - sum_right) < difference) 
		{   
			difference = abs(sum_left - sum_right);
			pos = i;
		}
	}//end of for loop
	//cout << "find pos_split: " << pos << endl;
	return pos;
}

int Find_Split_Pos_2(vector<data_pair> pdf, int st, int end)
{
	//cout << st << endl;
	int pos = st;
	int difference = 99999999999;
	int sum = 0;
	for (int j = st; j < end + 1; j++)
	{
		sum += pdf[j].freq;
	}
	sum = sum / 2;
	for (int i = st; i < end + 1; i++)
	{   
		int sum_left = 0;
		for (int j = st; j < i + 1; j++)
		{
			sum_left += pdf[j].freq;
		}
		//printf("sum_left %d \n", sum_left - sum_right);
		if (abs(sum_left - sum) < difference)
		{
			difference = abs(sum_left - sum);
			pos = i;
		}
	}//end of for loop
	 //cout << "find pos_split: " << pos << endl;
	return pos;
}

void Create_Shannon_Node(vector<shannon_node> &output,vector<data_pair> input, int st, int end, int from_left_or_right)
{    
	// create root node
	if ((end - st + 1) == input.size()) 
	{   	
		int split_pos = Find_Split_Pos(input, 0, input.size() - 1);
		shannon_node root = shannon_node(0, input.size() - 1);
		output.push_back(root);
		Create_Shannon_Node(output,input, 0, split_pos,0);
		Create_Shannon_Node(output,input, split_pos + 1, end,1);
	}
	// create leaf
	else if ((end - st) == 0) 
	{
		shannon_node leaf = shannon_node(input[st].index, st, from_left_or_right);
		output.push_back(leaf);
	}
	// create non-root-non-leaf node
	else 
	{
		shannon_node current_node = shannon_node(st, end, from_left_or_right);
		output.push_back(current_node);
		int split_pos = Find_Split_Pos(input, st, end);
		Create_Shannon_Node(output, input, st, split_pos, 0);
		Create_Shannon_Node(output, input, split_pos + 1, end, 1);
	}
}


vector<shannon_node> Generate_All_The_Nodes(vector<data_pair> data_pdf) 
{
	vector<shannon_node> All_Nodes;
	Create_Shannon_Node(All_Nodes, data_pdf, 0, data_pdf.size() - 1, -1);
	return All_Nodes;
}


shannon_node* Build_Shannon_Tree_From_Nodes(vector<shannon_node> &input) 
{   
	cout << "building shannon tree..." << endl;
	shannon_node* output = NULL;
	for (int i = 0;i < input.size(); i++) 
	{   
		int distance = 999999;
		int index = -1;
		if (input[i].is_root == true) 
		{
			output = &input[i];
		}
		else 
		{
			for (int j = 0; j < input.size(); j++) 
			{
				int band_self_L = input[i].start_and_end[0];
				int band_self_R = input[i].start_and_end[1];
				int band_target_L = input[j].start_and_end[0];
				int band_target_R = input[j].start_and_end[1];
				int band_self = band_self_R - band_self_L;
				int band_target = band_target_R - band_target_L;
				if (band_self_L >= band_target_L && band_self_R <= band_target_R && (band_target != band_self)) 
				{
					if (band_target < distance) 
					{
						distance = band_target;
						index = j;
					}			
				}
			}//index store the father point
		    // build father-child-pointer
			if (index == -1) 
			{
				cout << "Fatal Error: Find a Non-Root Point who doesn't have father points." << endl;
				while (1);
			}
			else 
			{   
				input[i].father = &input[index];
				if (input[i].left_or_right == 0) 
				{
					input[index].left_child = &input[i];
				}
				else 
				{
					input[index].right_child = &input[i];
				}
			}//end of finding father for one node	
		}//end of else-if
	}//find father-node for every nodes 
	if (output == NULL) 
	{   
		cout << "Fatal Error: Can not find the root." << endl;
		while (1);
	}
	for (int i = 0; i < input.size(); i++) 
	{
		if (input[i].is_leaf == false) 
		{
			if (input[i].left_child == NULL | input[i].right_child == NULL) 
			{
				cout << "Fatal Error: Find a Non-Leaf Point who have NULL-child-pointer." << endl;
				cout << "root- " << input[i].is_root << " leaf- " << input[i].is_leaf << " start_index:" << input[i].start_and_end[0] << " end_index:" << input[i].start_and_end[1] << " L or R:" << input[i].left_or_right << " symbol:" << (int)input[i].symbol << endl;
				while (1);
			}
		}
		else 
		{
			if (input[i].left_child != NULL | input[i].right_child != NULL) 
			{
				cout << "Fatal Error: Find a Leaf Point who have valid child-pointer." << endl;
				cout << "root- " << input[i].is_root << " leaf- " << input[i].is_leaf << " start_index:" << input[i].start_and_end[0] << " end_index:" << input[i].start_and_end[1] << " L or R:" << input[i].left_or_right << " symbol:" << (int)input[i].symbol << endl;
				while (1);
			}
		}	
	}
	cout << "shannon tree is correctly bulit. " << endl;
	return output;
}










void HW1_Problem_two_part_one(void) 
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
	/*vector<unsigned char> global;
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
	int sum = global.size();*/
//-----------------step#2 transfer four .dat files to pdf-----------------//
	cout << "start sorting" << endl;
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
	/*for (vector<data_pair>::iterator it = text_pdf.begin(); it != text_pdf.end(); it++)
	{
	cout << (int)it->index << "  " << (double)it->freq << endl;
	}*/
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
//-----------------step#4 Generate all Nodes From Global Pdf-----------------//
	vector<shannon_node> All_The_Nodes = Generate_All_The_Nodes(text_pdf);
	
//-----------------step#5 Use Nodes to build tree-----------------//
	shannon_node* shanno_tree = Build_Shannon_Tree_From_Nodes(All_The_Nodes);
	/*for (vector<shannon_node>::iterator it = All_The_Nodes.begin(); it != All_The_Nodes.end(); it++)
	{
		cout << "root- " << it->is_root << " leaf- " << it->is_leaf << " start_index:" << it->start_and_end[0] << " end_index:" << it->start_and_end[1] << " L or R:" << it->left_or_right << " symbol:" << (int)it->symbol << endl;
	}*/
	cout << "Total_Number_of_Nodes: " << All_The_Nodes.size() << endl;
//-----------------step#6 create encode table-----------------//
	vector<shannon_encode_pair> Table_shannon = Create_Shannon_Encode_Table_Without_Using_Recursion(All_The_Nodes);
	for (vector<shannon_encode_pair>::iterator it = Table_shannon.begin(); it != Table_shannon.end(); it++)
	{
	cout << (int)it->symbol << "  " << it->code << endl;
	}
//-----------------step#7 Use Table To encode data-----------------//
	string outcome_audio = "";
	string outcome_binary = "";
	string outcome_image= "";
	string outcome_text = "";
	/*cout << "rebuild audio..." << endl;
	for (vector<unsigned char>::iterator it = audio.begin(); it != audio.end(); it++)
	{
		outcome_audio += Look_For_Table(*it, Table_shannon);
	}*/
	//cout << outcome_audio << endl;
	/*cout << "rebuild binary..." << endl;
	for (vector<unsigned char>::iterator it = binary.begin(); it != binary.end(); it++)
	{
		outcome_binary += Look_For_Table(*it, Table_shannon);
	}*/
	//cout << outcome_binary << endl;
	/*cout << "rebuild image..." << endl;
	for (vector<unsigned char>::iterator it = image.begin(); it != image.end(); it++)
	{
		outcome_image += Look_For_Table(*it, Table_shannon);
	}*/
	////cout << outcome_image << endl;
	cout << "rebuild text..." << endl;
	for (vector<unsigned char>::iterator it = text.begin(); it != text.end(); it++)
	{
		outcome_text += Look_For_Table(*it, Table_shannon);
	}
	cout << outcome_image << endl;
//-----------------step#8 output new file to OS-----------------//
	//Write_String_To_Dat(outcome_audio, "E:\\MY_LOG\\669\\hw1\\output\\new_audio_shannon.dat");
	//Write_String_To_Dat(outcome_binary, "E:\\MY_LOG\\669\\hw1\\output\\new_binary_shannon.dat");
	//Write_String_To_Dat(outcome_image, "E:\\MY_LOG\\669\\hw1\\output\\new_image_shannon.dat");
	Write_String_To_Dat(outcome_text, "E:\\MY_LOG\\669\\hw1\\output\\new_text_shannon.dat");

	/*for (vector<data_pair>::iterator it = Symbol.begin(); it != Symbol.end(); it++)
	{
    cout << (int)it->index << "  " << it->freq << endl;
	}*/
}