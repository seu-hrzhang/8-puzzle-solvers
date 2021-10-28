#include "util.h"

using namespace std;

int final_state = 123456780;
vector<int> table; // table of 9-digit permutations

// the puzzle class
class AI_Puzzle {
  public:
    AI_Puzzle() { ; };
    AI_Puzzle(const vector<int> &init_state) {
        m_init_state.assign(init_state.begin(), init_state.end());
    };
    ~AI_Puzzle() { ; };

  public:
    // set the init state
    void setInitState(const vector<int> &init_state) {
        m_init_state.assign(init_state.begin(), init_state.end());
    };

    //
    // please fill three types of solver to find the solution
    //
    // return true if the problem has solution
    // return false if the problem does not have solution
    // results: the output sequences of states
    //
    // We want the final state is
    //
    //		1	2	3
    //		4	5	6
    //		7	8	0
    //
    bool solution(vector<vector<int>> &results) {
        if ((get_inv_num(m_init_state) - get_inv_num(int2digit(final_state))) %
                2 !=
            0)
            return false;
        else {
            // return BFS(results);
            return DFS(results);
            // return UCS(results);
        }
    }

  private:
    // bfs to find the solution
    bool BFS(vector<vector<int>> &results) {
        // bfs solver implementation using queue structure
        queue<int> bfs_solver;
        int init_state = digit2int(m_init_state);
        bfs_solver.push(init_state);

        // vectors to record parent nodes and cost
        // inspired by Dijkstra algo.
        vector<int> parents(table.size());
        vector<int> costs(table.size());
        vector<bool> visited(table.size());

        // set param of init state
        int init_idx = binary_search(table, init_state);
        parents[init_idx] = ROOT;
        visited[init_idx] = true;

        int parent, cost; // parent node and cost of current node

        int count = 0; // to count trying times

        while (!bfs_solver.empty()) {
            // visit current node
            int node = bfs_solver.front();
            int idx = binary_search(table, node);

            // pop front node
            bfs_solver.pop();

            // update parent node and costs
            parent = node;
            cost = costs[idx];

            if (node == final_state) {
                // traceback to initial state
                results.push_back(int2digit(node));
                int tb = parents[idx]; // traceback pointer
                while (tb != ROOT) {
                    results.push_back(int2digit(tb));
                    tb = parents[binary_search(table, tb)];
                }
                reverse(results.begin(), results.end());
                break;
            }

            // add fringe nodes to queue
            vector<int> fringes = get_fringe(node);
            for (int &fringe : fringes) {
                int fringe_idx = binary_search(table, fringe);
                if (!visited[fringe_idx]) {
                    bfs_solver.push(fringe);
                    parents[fringe_idx] = parent;
                    costs[fringe_idx] = cost + 1;
                    visited[fringe_idx] = true;
                    // cout << "visiting node: " << fringes[i] << endl;
                    // cout << "parent of " << fringes[i] << " is " << parent
                    // << endl;
                } else {
                    // found shorter path to fringe node, update parent and cost
                    if (costs[fringe_idx] > cost + 1) {
                        parents[fringe_idx] = parent;
                        costs[fringe_idx] = cost + 1;
                        // cout << "redirecting node: " << fringes[i] << endl;
                    } else {
                        // cout << "skipping node: " << fringes[i] << endl;
                    }
                }
            }
            count++;
            if (count % 100 == 0)
                cout << count << " nodes visited" << endl;
        }
        if (count > 0 && !results.empty())
            cout << "BFS succeeded after trying " << count << " times" << endl;
        return (!results.empty());
    }

    // dfs to find the solution
    bool DFS(vector<vector<int>> &results) {
        // dfs solver implementation using stack structure
        stack<int> dfs_solver;
        int init_state = digit2int(m_init_state);
        dfs_solver.push(init_state);

        // vectors to record parent nodes and costs
        // inspired by Dijkstra algo.
        vector<int> parents(table.size());
        vector<int> costs(table.size());
        vector<bool> visited(table.size());

        // set param of init state
        int init_idx = binary_search(table, init_state);
        parents[init_idx] = ROOT;
        visited[init_idx] = true;

        int parent, cost; // parent node and cost of current node

        int count = 0; // to count trying times

        while (!dfs_solver.empty()) {
            // visit current node
            int node = dfs_solver.top();
            int idx = binary_search(table, node);

            // pop front node
            dfs_solver.pop();

            // update parent node and cost
            parent = node;
            cost = costs[idx];
            // cout << "cost: " << cost << endl;

            if (node == final_state) {
                // traceback to initial state
                results.push_back(int2digit(node));
                int tb = parents[idx]; // traceback pointer
                while (tb != ROOT) {
                    results.push_back(int2digit(tb));
                    tb = parents[binary_search(table, tb)];
                }
                reverse(results.begin(), results.end());
                break;
            }

            // add fringe nodes to queue
            vector<int> fringes = get_fringe(node);
            for (int &fringe : fringes) {
                int fringe_idx = binary_search(table, fringe);
                if (!visited[fringe_idx] && cost + 1 < DFS_MAX_DEPTH) {
                    // constraint on maximum searching depth
                    dfs_solver.push(fringe);
                    parents[fringe_idx] = parent;
                    costs[fringe_idx] = cost + 1;
                    visited[fringe_idx] = true;
                    // cout << "visiting node: " << fringe << endl;
                    // cout << "parent of " << fringe << " is " << parent <<
                    // endl;
                } else {
                    // found shorter path to fringe node, update parent and cost
                    if (costs[fringe_idx] > cost + 1) {
                        parents[fringe_idx] = parent;
                        costs[fringe_idx] = cost + 1;
                        // cout << "redirecting node: " << fringes[i] << endl;
                    } else {
                        // cout << "skipping node: " << fringes[i] << endl;
                    }
                }
            }
            count++;
            if (count % 100 == 0)
                cout << count << " nodes visited" << endl;
        }
        if (count > 0 && !results.empty())
            cout << "DFS succeeded after trying " << count << " times" << endl;
        return (!results.empty());
    }

    // ucs to find the solution
    bool UCS(vector<vector<int>> &results) {
        // ucs solver implementation using priority queue structure
        priority_queue<UCSNode> ucs_solver;
        int init_state = digit2int(m_init_state);
        UCSNode init_node(init_state);
        update_ucs_cost(init_node, init_state);
        ucs_solver.push(UCSNode(init_state));

        // vectors to record parent nodes and steps
        // inspired by Dijkstra algo.
        vector<int> parents(table.size());
        vector<int> steps(table.size());
        vector<bool> visited(table.size());

        // set param of init state
        int init_idx = binary_search(table, init_state);
        parents[init_idx] = ROOT;
        visited[init_idx] = true;

        int parent, step; // parent node and step of current node

        int count = 0; // to count trying times

        while (!ucs_solver.empty()) {
            // visit current node
            UCSNode node = ucs_solver.top();
            int idx = binary_search(table, node.num);

            // pop front node
            ucs_solver.pop();

            // update parent node and cost
            parent = node.num;
            step = steps[idx];

            if (node.num == final_state) {
                // traceback to initial state
                results.push_back(int2digit(node.num));
                int tb = parents[idx]; // traceback pointer
                while (tb != ROOT) {
                    results.push_back(int2digit(tb));
                    tb = parents[binary_search(table, tb)];
                }
                reverse(results.begin(), results.end());
                break;
            }

            // add fringe nodes to queue
            vector<int> fringes = get_fringe(node.num);
            for (int fringe : fringes) {
                int fringe_idx = binary_search(table, fringe);
                if (!visited[fringe_idx]) {
                    UCSNode fringe_node(fringe);
                    update_ucs_cost(fringe_node, init_state);
                    ucs_solver.push(fringe_node);
                    parents[fringe_idx] = parent;
                    steps[fringe_idx] = step + 1;
                    visited[fringe_idx] = true;
                    // cout << "visiting node: " << fringe << endl;
                    // cout << "parent of " << fringe << " is " << parent <<
                    // endl;
                } else {
                    // found shorter path to fringe node, update parent and cost
                    if (steps[fringe_idx] > step + 1) {
                        parents[fringe_idx] = parent;
                        steps[fringe_idx] = step + 1;
                        // cout << "redirecting node: " << fringe << endl;
                    } else {
                        // cout << "skipping node: " << fringe << endl;
                    }
                }
            }
            count++;
            if (count % 100 == 0)
                cout << count << " nodes visited" << endl;
        }
        if (count > 0 && !results.empty())
            cout << "UCS succeeded after trying " << count << " times" << endl;
        return (!results.empty());
    }

  private:
    vector<int> m_init_state;
};

// test function for 8-puzzle problem
void test(const vector<int> &init_state) {
    AI_Puzzle puzzle;
    puzzle.setInitState(init_state);
    vector<vector<int>> results;
    if (puzzle.solution(results)) {
        printf("solution for the initial state: ");
        for (int i : init_state)
            printf("%d ", i);
        printf("\n");
        for (int i = 0; i < results.size(); i++) {
            printf("step: %03d:", i);
            for (int j : results[i]) {
                printf(" %d", j);
            }
            printf("\n");
        }
    } else {
        printf("no solution for the initial state: ");
        for (int i : init_state)
            printf("%d ", i);
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    // This is the code for testing the 8-puzzle problem in
    //
    //  'The first project homework for Artifical Intelligence'
    //
    // Please fill in the code of class 'AI_Puzzle'
    // and write your design, running tests, analysis, etc. into your report
    // Be careful: in each step, we can ONLY exchange with ONE neighbor of digit
    // 0, horizontally or vertically
    //
    // Good Luck!
    //
    // Yangang Wang, 2021/8/15, @SEU

    permutations(N_NUMBER, table);

    // Test 1
    //	5   1   2
    //	6   3   0
    //	4   7   8
    vector<int> init_state_1 = {5, 1, 2, 6, 3, 0, 4, 7, 8};
    // test(init_state_1);

    // Test 2
    //
    //	2   8   3
    //	1   6   4
    //	7   5   0
    vector<int> init_state_2 = {2, 8, 3, 1, 6, 4, 7, 5, 0};
    // test(init_state_2);

    // Test 3
    //
    //	1   0   2
    //	3   4   5
    //	6   7   8
    vector<int> init_state_3 = {1, 0, 2, 3, 4, 5, 6, 7, 8};
    // test(init_state_3);

    // Test 4
    //
    //	4   3   5
    //	6   0   8
    //	2   1   7
    vector<int> init_state_4 = {4, 3, 5, 6, 0, 8, 2, 1, 7};
    // test(init_state_4);

    // Test 5
    //
    //	1   7   3
    //	0   5   4
    //	6   2   8
    vector<int> init_state_5 = {1, 7, 3, 0, 5, 4, 6, 2, 8};
    test(init_state_5);

    // vector<int> test_state = {1, 2, 3, 4, 5, 6, 0, 7, 8};
    // test(test_state);

    return 0;
}