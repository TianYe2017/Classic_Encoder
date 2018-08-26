#pragma once
#ifndef P2_1_H
#define P2_1_H
#include "stdafx.h"
#include "io.h"
#include "shannon.h"
#include "data_pair.h"
#include "shannon_node.h"
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 


using namespace std;

void HW1_Problem_two_part_one(void);
double Compute_Entropy(vector<data_pair> input);

#endif 
