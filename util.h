//
// Created by Starry Night on 2021/8/25.
//

#ifndef INC_8_PUZZLE_SOLVER_UTIL_H
#define INC_8_PUZZLE_SOLVER_UTIL_H

#include <iostream>
#include <map>
#include <math.h>
#include <queue>
#include <stack>
#include <vector>

#define N_NUMBER 9

#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define ROOT -1

#define DFS_MAX_DEPTH 50

using namespace std;

// the node class used in UCS
class UCSNode {
  public:
    int num;
    int cost;
    UCSNode() { this->cost = 0; }
    UCSNode(int num, int cost = 0) {
        this->num = num;
        this->cost = cost;
    }

    friend bool operator<(UCSNode node_1, UCSNode node_2) {
        return node_1.cost > node_2.cost;
    }
};

// get inversion number of a vector
int get_inv_num(vector<int> v);

// convert digit vector to integer
int digit2int(vector<int> digits);

// convert integer to digit vector
vector<int> int2digit(int num);

// get location (row and col) of a given index
// index range: 0 ~ 8
vector<int> get_loc(int idx);

// print a integer vector
void print_vector(vector<int> v);

// binary search in an ascending array
int binary_search(vector<int> v, int key);

// search key element in vector, returns index
int search(vector<int> v, int key);

// DFS for permutation
int perm_dfs(vector<int> mark, int step, int len, vector<int> path,
             vector<int> &table);

// give permutation of 'len' numbers
int permutations(int len, vector<int> &table);

// find fringe states from given node
vector<int> get_fringe(int node);

// update cost of a UCS node
void update_ucs_cost(UCSNode &node, int src);

#endif // INC_8_PUZZLE_SOLVER_UTIL_H
