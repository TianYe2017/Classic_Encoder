#include "stdafx.h"
#include "p2_3.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 

using namespace std;

int Where_is_symbol(vector<adaptive_haffman_node> tree, unsigned char target) 
{
	int pos = 0;
	bool exist = false;
	for (vector<adaptive_haffman_node>::iterator it = tree.begin(); it != tree.end(); it++)
    {   
		if (it->is_leaf == true && it->symbol == target) 
		{
			exist = true;
			break;
		}
		pos++;
	}
	if (exist == true) 
	{
		return pos;
	}
	return -1;
}

int Where_is_NYT(vector<adaptive_haffman_node> tree) 
{   
	int pos = 0;
	for (vector<adaptive_haffman_node>::iterator it = tree.begin(); it != tree.end(); it++)
	{   
		if (it->is_NYT == true) 
		{
			break;
		}
		pos++;
	}
	return pos;
}

int Where_is_non_used_node(vector<adaptive_haffman_node> tree) 
{
	int pos = 0;
	for (vector<adaptive_haffman_node>::iterator it = tree.begin(); it != tree.end(); it++)
	{
		if (it->used == false)
		{
			break;
		}
		pos++;
	}
	return pos;
}

int Find_Swap_Target(vector<adaptive_haffman_node> tree, int weight, int current_order, adaptive_haffman_node* self)
{   
	int out = -1;
	int order = current_order;
	for (int i = 0; i<tree.size();i++)
	{
		if (tree[i].weight == weight && tree[i].is_root == false && (&tree[i])!=self->father) 
		{
			if (tree[i].order > order) 
			{
				out = i;
				order = tree[i].order;
			}
		}
	}
	return out;
}

void Swap(adaptive_haffman_node* current, adaptive_haffman_node* swap_target) 
{  
	//cout << "swap." << endl;
	//记录待换节点信息
	//adaptive_haffman_node* current_father = current->father;
	bool current_is_leaf = current->is_leaf;
	bool current_is_NYT = current->is_NYT;
	bool current_is_root = current->is_root;
	adaptive_haffman_node* current_left_child = current->left_child;
	//int current_left_or_right = current->left_or_right;
	//int current_order = current->order;
	adaptive_haffman_node* current_right_child = current->right_child;
	unsigned char current_symnol = current->symbol;
	bool current_used = current->used;
	int current_weight = current->weight;
	//把current的全部子树，父节点和权重换成swap_target的
	//current->father = swap_target->father;
	current->is_leaf = swap_target->is_leaf;
	current->is_NYT = swap_target->is_NYT;
	current->is_root = swap_target->is_root;
	current->left_child = swap_target->left_child;
	//current->left_or_right = swap_target->left_or_right;
	//current->order = swap_target->order;
	current->right_child = swap_target->right_child;
	current->symbol = swap_target->symbol;
	current->used = swap_target->used;
	current->weight = swap_target->weight;
	if (current->left_child !=NULL) 
	{
		current->left_child->father = current;
	}
	if(current->right_child!=NULL)
	{
		current->right_child->father = current;
	}
	//把swap_target的全部子树，父节点和权重换成current的
	//swap_target->father = current_father;
	swap_target->is_leaf = current_is_leaf;
	swap_target->is_NYT = current_is_NYT;
	swap_target->is_root = current_is_root;
	swap_target->left_child = current_left_child;
	//swap_target->left_or_right = current_left_or_right;
	//swap_target->order = current_order;
	swap_target->right_child = current_right_child;
	swap_target->symbol = current_symnol;
	swap_target->used = current_used;
	swap_target->weight = current_weight;
	if (swap_target->left_child != NULL) 
	{
		swap_target->left_child ->father = swap_target;
	}
	if (swap_target->right_child != NULL)
	{
		swap_target->right_child->father = swap_target;
	}
	swap_target->weight++;
}



vector<adaptive_haffman_node> Tree_Init(int number_of_symbols) 
{   
	int size = 2 * number_of_symbols;
	vector<adaptive_haffman_node> tree;
	for (int i = 0; i < size; i++)
	{
		tree.push_back(adaptive_haffman_node());
	}
	tree[0].used = true;
	tree[0].is_NYT = true;
	tree[0].is_root = true;
	tree[0].weight = 0;
	tree[0].order = size;
	return tree;
}

string Add_New_Data(vector<adaptive_haffman_node> &tree, unsigned char data) 
{
	string code = "";
	int pos = Where_is_symbol(tree, data);
	if (pos == -1) //说明要建立一个新节点
	{
		int pos_of_old_NYT = Where_is_NYT(tree);
		int pos_of_new_NYT = Where_is_non_used_node(tree);
		int pos_of_new_leaf = pos_of_new_NYT + 1;
		//建造叶子节点
		tree[pos_of_new_leaf].father = &tree[pos_of_old_NYT];
		tree[pos_of_new_leaf].is_leaf = true;
		tree[pos_of_new_leaf].is_NYT = false;
		tree[pos_of_new_leaf].is_root = false;
		tree[pos_of_new_leaf].left_child = NULL;
		tree[pos_of_new_leaf].left_or_right = 1;
		tree[pos_of_new_leaf].order = tree[pos_of_old_NYT].order - 1;
		tree[pos_of_new_leaf].right_child = NULL;
		tree[pos_of_new_leaf].symbol = data;
		tree[pos_of_new_leaf].used = true;
		tree[pos_of_new_leaf].weight = 1;
		//建造新NYT节点
		tree[pos_of_new_NYT].father = &tree[pos_of_old_NYT];
		tree[pos_of_new_NYT].is_leaf = false;
		tree[pos_of_new_NYT].is_NYT = true;
		tree[pos_of_new_NYT].is_root = false;
		tree[pos_of_new_NYT].left_child = NULL;
		tree[pos_of_new_NYT].left_or_right = 0;
		tree[pos_of_new_NYT].order = tree[pos_of_old_NYT].order - 2;
		tree[pos_of_new_NYT].right_child = NULL;
		tree[pos_of_new_NYT].symbol = 0x00;
		tree[pos_of_new_NYT].used = true;
		tree[pos_of_new_NYT].weight = 0;
		//更新旧NYT节点
		tree[pos_of_old_NYT].is_NYT = false;
		tree[pos_of_old_NYT].left_child = &tree[pos_of_new_NYT];
		tree[pos_of_new_NYT].right_child = &tree[pos_of_new_leaf];
		tree[pos_of_new_NYT].weight = 1;
		//根据当前树输出
		int k = Where_is_symbol(tree, data);
		adaptive_haffman_node* write = &tree[k];
		
		while ((write->father) != NULL)
		{
			if ((write->left_or_right) == 0)
			{
				code += "0";
			}
			else
			{
				code += "1";
			}
			write = write->father;
		}
		reverse(code.begin(), code.end());
		//开始从新NYT节点开始看看它爹是不是要换位置
		adaptive_haffman_node* p = &tree[pos_of_old_NYT];
		
		if (p->is_root == true) 
		{
			return "1";
		}
		else 
		{
			p = p->father;
		}
		while (p->is_root!=true) 
		{   
			int swap = Find_Swap_Target(tree, p->weight, p->order,p);
			if (swap == -1) 
			{
				p->weight++;
			}
			else 
			{
				Swap(p, &tree[swap]);
				p = &tree[swap];
			}
			p = p->father;		
		}
		p->weight++;
		
	}//end of 新建节点
	else //说明当前节点已经存在
	{   
		//先输出
		int k = Where_is_symbol(tree, data);
		adaptive_haffman_node* write = &tree[k];
		while ((write->father) != NULL)
		{
			if ((write->left_or_right) == 0)
			{
				code += "0";
			}
			else
			{
				code += "1";
			}
			write = write->father;
		}
		reverse(code.begin(), code.end());
		//再改树
		adaptive_haffman_node* p = &(tree[pos]);
		while (p->is_root==false)
		{
			int swap = Find_Swap_Target(tree, p->weight, p->order,p);
			if (swap == -1)
			{
				p->weight++;
			}
			else
			{
				Swap(p, &tree[swap]);
				p = &tree[swap];
			}
			p = p->father;
		}
		p->weight++;
	}//end of 已有节点更新
	////对当前字符编码输出
	//int k = Where_is_symbol(tree, data);
	//adaptive_haffman_node* write = &tree[k];
	//string code = "";
	//while ((write->father) != NULL)
	//{
	//	if ((write->left_or_right) == 0)
	//	{
	//		code += "0";
	//	}
	//	else
	//	{
	//		code += "1";
	//	}
	//	write = write->father;
	//}
	//reverse(code.begin(), code.end());
	return code;
}



adaptive_haffman_node* Build_Adaptive_Haffman_Tree(vector<adaptive_haffman_node> &tree, vector<unsigned char> data,string &output) 
{  
    cout << "building adaptive haffman tree..." << endl;
	//-----------------------对第一个元素建立节点树-----------------------//
	//NYT变root
	tree[0].father = NULL;
	tree[0].is_leaf = false;
	tree[0].is_NYT = false;
	tree[0].is_root = true;
	tree[0].left_child = &tree[1];//tree【1】也是新的NYT节点
	tree[0].left_or_right = -1;//root节点无左右之分
	tree[0].order = 512;
	tree[0].right_child = &tree[2];//tree[2]是新叶子节点
	tree[0].symbol = 0x00;
	tree[0].used = true;
	tree[0].weight = 1;
	//建立新NYT
	tree[1].father = &tree[0];
	tree[1].is_leaf = false;
	tree[1].is_NYT = true;
	tree[1].is_root = false;
	tree[1].left_child = NULL;
	tree[1].left_or_right = 0;
	tree[1].order = 510;
	tree[1].right_child = NULL;
	tree[1].symbol = 0x00;
	tree[1].used = true;
	tree[1].weight = 0;
	//建立一个叶子
	tree[2].father = &tree[0];
	tree[2].is_leaf = true;
	tree[2].is_NYT = false;
	tree[2].is_root = false;
	tree[2].left_child = NULL;
	tree[2].left_or_right = 1;
	tree[2].order = 511;
	tree[2].right_child = NULL;
	tree[2].symbol = data[0];
	tree[2].used = true;
	tree[2].weight = 1;

	//-----------------------对之后的数据编码-----------------------//
	int i = 0;
	for (vector<unsigned char>::iterator it = data.begin(); it != data.end(); it++)
	{  
		if (i == 0) 
		{
			i++;
		}
		else 
		{   
			cout << "already finish: "<< i << " symbols" << endl;
		    output+= Add_New_Data(tree, *it);//到这里树更新完了，用当前树写string
			i++;
		}	
	}
    //check
	for (int i = 0; i < tree.size(); i++)
	{
	   adaptive_haffman_node* p = &tree[i];
	   if (p->is_leaf == true)
	   {
	     string temp = "";
	     unsigned char c_symbol = p->symbol;
	     while (p->is_root == false)
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
	return NULL;
}










void HW1_Problem_two_part_three(void) 
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
	//-----------------step#2 build haffman tree-----------------//
	vector<adaptive_haffman_node> adaptive_haffman_tree = Tree_Init(256);
	string out = "1";
	adaptive_haffman_node* a = Build_Adaptive_Haffman_Tree(adaptive_haffman_tree, text, out);
	cout << "done building." << endl;
	Write_String_To_Dat(out, "E:\\MY_LOG\\669\\hw1\\output\\new_text_adaptive_haffman.dat");
	



}