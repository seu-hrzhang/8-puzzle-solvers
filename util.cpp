//
// Created by Starry Night on 2021/8/25.
//

#include "util.h"

using namespace std;

// shifting matrix to get an available fringe from a known direction
int shift_mat[4][2] = {1, 0, -1, 0, 0, 1, 0, -1};

int get_inv_num(vector<int> v) {
    int count = 0;

    // erase 0 from v
    for (auto iter = v.begin(); iter != v.end();) {
        if (*iter == 0)
            iter = v.erase(iter);
        else
            iter++;
    }

    for (int i = 0; i < v.size(); ++i) {
        for (int j = i + 1; j < v.size(); ++j) {
            if (v[i] > v[j])
                count++;
        }
    }
    return count;
}

int digit2int(vector<int> digits) {
    int num = 0;
    int len = digits.size();
    for (int i = 0; i < digits.size(); ++i)
        num += digits[i] * pow(10, len - i - 1);

    return num;
}

vector<int> int2digit(int num) {
    vector<int> digits;
    while (num >= 1) {
        digits.push_back(num % 10);
        num /= 10;
    }
    // fill in extra 0 into digit vector
    if (digits.size() < N_NUMBER)
        digits.push_back(0);
    reverse(begin(digits), end(digits));
    return digits;
}

vector<int> get_loc(int idx) {
    vector<int> loc;
    int row, col;

    int grid_idx = idx + 1; // convert index range to 1 ~ 9

    col = grid_idx % 3;
    row = (grid_idx - col) / 3 + 1;

    loc.push_back(row);
    loc.push_back(col);

    return loc;
}

void print_vector(vector<int> v) {
    for (int i : v) {
        printf("%d ", i);
    }
    printf("\n");
}

int binary_search(vector<int> v, int key) {
    int low = 0, high = v.size(), mid = 0;
    while (low < high) {
        mid = (low + high) / 2;
        if (v[mid] == key)
            return mid;
        else if (v[mid] > key)
            high = mid;
        else
            low = mid + 1;
    }
    return -1;
}

int search(vector<int> v, int key) {
    int len = v.size();
    for (int i = 0; i < len; ++i) {
        if (v.at(i) == key)
            return i;
    }
    return -1;
}

int perm_dfs(vector<int> mark, int step, int len, vector<int> path,
             vector<int> &table) {
    if (step == len) {
        int num = digit2int(path);
        table.push_back(num);
        return 1;
    }
    int total = 0;
    int i;
    for (i = 0; i < len; ++i) {
        if (mark[i])
            continue;
        mark[i] = 1;
        path[step] = i;
        total += perm_dfs(mark, step + 1, len, path, table);
        mark[i] = 0;
    }
    return total;
}

int permutations(int len, vector<int> &table) {
    vector<int> mark(len);
    vector<int> path(len);
    int total = 0;
    int i;
    for (i = 0; i < len; ++i)
        mark[i] = 0;
    for (i = 0; i < len; ++i) {
        mark[i] = 1;
        path[0] = i;
        total += perm_dfs(mark, 1, len, path, table);
        mark[i] = 0;
    }
    return total;
}

vector<int> get_fringe(int node) {
    vector<int> fringes;

    // convert to matrix to check availability
    int mat[3][3], t_mat[3][3];
    int row, col;
    for (int i = 2; i >= 0; --i) {
        for (int j = 2; j >= 0; --j) {
            mat[i][j] = node % 10;
            node /= 10;
            // record blank position
            if (mat[i][j] == 0) {
                row = i;
                col = j;
            }
        }
    }

    for (int dir = 0; dir < 4; ++dir) {
        if ((dir == DIR_UP && row == 2) || (dir == DIR_DOWN && row == 0) ||
            (dir == DIR_LEFT && col == 2) || (dir == DIR_RIGHT && col == 0))
            continue;
        else {
            // copy from 'mat' to 't_mat'
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    t_mat[i][j] = mat[i][j];

            t_mat[row][col] =
                mat[row + shift_mat[dir][0]][col + shift_mat[dir][1]];
            t_mat[row + shift_mat[dir][0]][col + shift_mat[dir][1]] = 0;

            int num = 0;
            for (auto &i : t_mat)
                for (int j : i)
                    num = num * 10 + j;
            fringes.push_back(num);
        }
    }

    return fringes;
}

void update_ucs_cost(UCSNode &node, int src) {
    node.cost = 0;
    vector<int> digits = int2digit(node.num);
    vector<int> src_digits = int2digit(src);
    for (int num = 1; num < N_NUMBER; ++num) {
        int idx = search(digits, num);
        int src_idx = search(src_digits, num);
        vector<int> src_loc = get_loc(src_idx);
        vector<int> loc = get_loc(idx);
        node.cost += abs(loc[0] - src_loc[0]) + abs(loc[1] - src_loc[1]);
    }
    // cout << "node: " << node.num << ", \tcost: " << node.cost << endl;
}