# 8-puzzle Solvers

## Introduction

This project is an implementation of solvers for 8-puzzle. Solver types of this project include BFS, DFS and UCS. This
project was written in C++ with the help of STL. Corresponding theory and tests are listed in following sections.

## Theory

The 8-puzzle is a classic problem in the area of AI. It studies the case of a 9-grid with elements selected
from `{ 1, 2, 3, 4, 5, 6, 7, 8, 0 }`, where digit 0 represents a blank grid. Solvers of this problem tries to find a
moving strategy from an initial state to a final state, in as few steps as possible. Each step allows an exchange of
digit 0 and one of its neighbor.

To find a moving strategy (a path) between 2 states can be regarded as a searching problem, which can be solved using
BFS, DFS and UCS.

### Breadth-first Search (BFS)

Breadth-first search (BFS) is an algorithm for searching a tree data structure for a node that satisfies a given
property. It starts at the tree root and explores all nodes at the present depth prior to moving on to the nodes at the
next depth level. Extra memory, usually a queue, is needed to keep track of the child nodes that were encountered but
not yet explored.

BFS can be expressed in pseudocode as

```
procedure BFS(G, root) is
    let Q be a queue
    label root as explored
    Q.enqueue(root)
    while Q is not empty do
        v := Q.dequeue()
        if v is the goal then
            return v
        for all edges from v to w in G.adjacentEdges(v) do
            if w is not labeled as explored then
                label w as explored
                Q.enqueue(w)
```

### Depth-first Search (DFS)

Depth-first search (DFS) is an algorithm for traversing or searching tree or graph data structures. The algorithm starts
at the root node (selecting some arbitrary node as the root node in the case of a graph) and explores as far as possible
along each branch before backtracking.

DFS can be expressed in pseudocode as

```
procedure DFS_iterative(G, v) is
    let S be a stack
    S.push(v)
    while S is not empty do
        v = S.pop()
        if v is not labeled as discovered then
            label v as discovered
            for all edges from v to w in G.adjacentEdges(v) do 
                S.push(w)
```

### Uniform-cost Search (UCS)

Uniform-cost search (UCS) is a derivation form
of [Dijkstra's algorithm](https://en.wikipedia.org/wiki/Dijkstra's_algorithm), which starts with a priority queue that
contains only one item, and insert new items as they are discovered. UCS always extends the node with the least cost in
the priority queue.

UCS can be expressed in pseudocode as

```
procedure uniform_cost_search(Graph, start, goal) is
    node ← start
    cost ← 0
    frontier ← priority queue containing node only
    explored ← empty set
    do
        if frontier is empty then
            return failure
        node ← frontier.pop()
        if node is goal then
            return solution
        explored.add(node)
        for each of node's neighbors n do
            if n is not in explored then
                frontier.add(n)
```

In this project, the stepwise cost is considered to be 1 regardless of the moving direction. Therefore, a customized
cost definition is required to make UCS effective. Let cost between two states in UCS be the sum
of [Chebyshev distance](https://en.wikipedia.org/wiki/Chebyshev_distance)
of each element (except digit 0) within them.

### Judgement of Solution Existence

Following is a simple rule to check if an 8-puzzle is solvable. An 8-puzzle is unsolvable if
the [inversions](https://en.wikipedia.org/wiki/Inversion_(discrete_mathematics)) of the input sequence is odd while that
of the destination is even, and vice versa.

## Implementation Details

From the workflow of BFS, DFS and UCS, it's natural to utilize the Standard Template Library (STL) in C++ as the solver
structures. Queue, stack and priority queue are selected for the three searching methods respectively.

Definitions of solvers are as follows:

```c++
queue<int> bfs_solver;
stack<int> dfs_solver;
priority_queue<UCSNode> ucs_solver;
```

where `UCSNode` is a node class used in coding of UCS.

### Data Encoding

The given code template uses a vector to store a sequence, i.e.

```
{ 1, 0, 2, 3, 4, 5, 6, 7, 8 }
```

represents the state

```
1   0   2
3   4   5
6   7   8
```

This way, a sequence takes up memory of 9 integers. To lower the memory occupation, we use a single integer to represent
a sequence. For instance, the sequence above is encoded to a single integer `102345678`. This saves as much as 8 times
of memory.

### Path Traceback

Inspired from Dijkstra's algorithm, we use a vector to record the parent node of each node, and another to record
whether its visited:

```c++
vector<int> parents(table.size());
vector<bool> visited(table.size());
```

Therefore, after the final state is reached, we traceback to the initial state using the `parents` vector:

```c++
if (node == final_state) {
    // traceback to initial state
    results.push_back(int2digit(node));
    int tb = parents[idx]; // traceback pointer
    while (tb != ROOT) {
        results.push_back(int2digit(tb));
        tb = parents[search(table, tb)];
    }
    reverse(results.begin(), results.end());
    break;
}
```

### Maximum Depth Constraint

While using DFS to search for solution of 8-puzzle, we found that it can easily go too deep into a branch, which may
result in a severe derivation from the solution. Therefore, we set the constraint for the maximum reachable depth in DFS
as `30`.

```c++
#define DFS_MAX_DEPTH 30
```

## Results and Analysis

The given code template offers 5 initial states for testing. All three methods are used to solve the problems.

Ever since inversion parities of initial states 2 and 4 are different from that of the final state, they are unsolvable
and the program sends the following output:

```
> no solution for the initial state: 2 8 3 1 6 4 7 5 0
> no solution for the initial state: 4 3 5 6 0 8 2 1 7
```

Testing results are listed in the following table:

|             |   **BFS**   |  **DFS**   |   **UCS**   |
| :---------: | :---------: | :--------: | :---------: |
| **state 1** |  11 / 1292  | 11 / 6474  | 17 / 70838  |
| **state 3** | 21 / 67846  | 29 / 37631 | 167 / 31881 |
| **state 5** | 23 / 122501 | 49 / 37597 | 49 / 12743  |

*Note: **a / b** where 'a' is total steps of the solution, 'b' is number of nodes visited during searching.*

Complete testing results are listed in Appendices section.

From testing results, it's clear that performance of each method is highly associated with the given initial state.
Generally, BFS has a rather satisfying performance, with solution for each initial state having the fewest steps.

After given a limit on maximum depth, DFS becomes faster and can sometimes find a solution in less time than BFS (see
initial state 3). However, the solution is more likely to be only a local optimum. Also, it should be pointed out that
the actual performance of DFS is highly dependent on the order of fringe nodes. Different arrangements of fringe nodes
could result in very different results.

UCS performs better when initial state is in smaller chessboard distance from initial state. This is actually decided by
the our customized cost definition. To acquire better performance in UCS, an reform in cost definition is necessary.
Also, UCS often provides a solution with much more steps than the other two, since it always tries to find a node with
less cost.

## Appendices

Following is the output of testing programs.

1. **BFS** on initial state 1:

```
BFS succeeded after trying 1292 times
solution for the initial state: 5 1 2 6 3 0 4 7 8
step: 000: 5 1 2 6 3 0 4 7 8
step: 001: 5 1 2 6 0 3 4 7 8
step: 002: 5 1 2 0 6 3 4 7 8
step: 003: 0 1 2 5 6 3 4 7 8
step: 004: 1 0 2 5 6 3 4 7 8
step: 005: 1 2 0 5 6 3 4 7 8
step: 006: 1 2 3 5 6 0 4 7 8
step: 007: 1 2 3 5 0 6 4 7 8
step: 008: 1 2 3 0 5 6 4 7 8
step: 009: 1 2 3 4 5 6 0 7 8
step: 010: 1 2 3 4 5 6 7 0 8
step: 011: 1 2 3 4 5 6 7 8 0
```

2. **BFS** on initial state 3:

```
BFS succeeded after trying 67846 times
solution for the initial state: 1 0 2 3 4 5 6 7 8 
step: 000: 1 0 2 3 4 5 6 7 8
step: 001: 1 4 2 3 0 5 6 7 8
step: 002: 1 4 2 0 3 5 6 7 8
step: 003: 1 4 2 6 3 5 0 7 8
step: 004: 1 4 2 6 3 5 7 0 8
step: 005: 1 4 2 6 3 5 7 8 0
step: 006: 1 4 2 6 3 0 7 8 5
step: 007: 1 4 2 6 0 3 7 8 5
step: 008: 1 4 2 0 6 3 7 8 5
step: 009: 1 4 2 7 6 3 0 8 5
step: 010: 1 4 2 7 6 3 8 0 5
step: 011: 1 4 2 7 0 3 8 6 5
step: 012: 1 0 2 7 4 3 8 6 5
step: 013: 1 2 0 7 4 3 8 6 5
step: 014: 1 2 3 7 4 0 8 6 5
step: 015: 1 2 3 7 4 5 8 6 0
step: 016: 1 2 3 7 4 5 8 0 6
step: 017: 1 2 3 7 4 5 0 8 6
step: 018: 1 2 3 0 4 5 7 8 6
step: 019: 1 2 3 4 0 5 7 8 6
step: 020: 1 2 3 4 5 0 7 8 6
step: 021: 1 2 3 4 5 6 7 8 0
```

3. **BFS** on initial state 5:

```
BFS succeeded after trying 122501 times
solution for the initial state: 1 7 3 0 5 4 6 2 8 
step: 000: 1 7 3 0 5 4 6 2 8
step: 001: 1 7 3 5 0 4 6 2 8
step: 002: 1 7 3 5 4 0 6 2 8
step: 003: 1 7 3 5 4 8 6 2 0
step: 004: 1 7 3 5 4 8 6 0 2
step: 005: 1 7 3 5 4 8 0 6 2
step: 006: 1 7 3 0 4 8 5 6 2
step: 007: 1 7 3 4 0 8 5 6 2
step: 008: 1 7 3 4 6 8 5 0 2
step: 009: 1 7 3 4 6 8 5 2 0
step: 010: 1 7 3 4 6 0 5 2 8
step: 011: 1 7 3 4 0 6 5 2 8
step: 012: 1 0 3 4 7 6 5 2 8
step: 013: 0 1 3 4 7 6 5 2 8
step: 014: 4 1 3 0 7 6 5 2 8
step: 015: 4 1 3 7 0 6 5 2 8
step: 016: 4 1 3 7 2 6 5 0 8
step: 017: 4 1 3 7 2 6 0 5 8
step: 018: 4 1 3 0 2 6 7 5 8
step: 019: 0 1 3 4 2 6 7 5 8
step: 020: 1 0 3 4 2 6 7 5 8
step: 021: 1 2 3 4 0 6 7 5 8
step: 022: 1 2 3 4 5 6 7 0 8
step: 023: 1 2 3 4 5 6 7 8 0
```

4. **DFS** on initial state 1:

```
DFS succeeded after trying 6474 times
solution for the initial state: 5 1 2 6 3 0 4 7 8 
step: 000: 5 1 2 6 3 0 4 7 8
step: 001: 5 1 2 6 0 3 4 7 8
step: 002: 5 1 2 0 6 3 4 7 8
step: 003: 0 1 2 5 6 3 4 7 8
step: 004: 1 0 2 5 6 3 4 7 8
step: 005: 1 2 0 5 6 3 4 7 8
step: 006: 1 2 3 5 6 0 4 7 8
step: 007: 1 2 3 5 0 6 4 7 8
step: 008: 1 2 3 0 5 6 4 7 8
step: 009: 1 2 3 4 5 6 0 7 8
step: 010: 1 2 3 4 5 6 7 0 8
step: 011: 1 2 3 4 5 6 7 8 0
```

5. **DFS** on initial state 3:

```
DFS succeeded after trying 37631 times
solution for the initial state: 1 0 2 3 4 5 6 7 8 
step: 000: 1 0 2 3 4 5 6 7 8
step: 001: 0 1 2 3 4 5 6 7 8
step: 002: 3 1 2 0 4 5 6 7 8
step: 003: 3 1 2 4 0 5 6 7 8
step: 004: 3 1 2 4 7 5 6 0 8
step: 005: 3 1 2 4 7 5 0 6 8
step: 006: 3 1 2 0 7 5 4 6 8
step: 007: 0 1 2 3 7 5 4 6 8
step: 008: 1 0 2 3 7 5 4 6 8
step: 009: 1 2 0 3 7 5 4 6 8
step: 010: 1 2 5 3 7 0 4 6 8
step: 011: 1 2 5 3 0 7 4 6 8
step: 012: 1 2 5 0 3 7 4 6 8
step: 013: 0 2 5 1 3 7 4 6 8
step: 014: 2 0 5 1 3 7 4 6 8
step: 015: 2 3 5 1 0 7 4 6 8
step: 016: 2 3 5 1 7 0 4 6 8
step: 017: 2 3 5 1 7 8 4 6 0
step: 018: 2 3 5 1 7 8 4 0 6
step: 019: 2 3 5 1 0 8 4 7 6
step: 020: 2 3 5 1 8 0 4 7 6
step: 021: 2 3 0 1 8 5 4 7 6
step: 022: 2 0 3 1 8 5 4 7 6
step: 023: 0 2 3 1 8 5 4 7 6
step: 024: 1 2 3 0 8 5 4 7 6
step: 025: 1 2 3 4 8 5 0 7 6
step: 026: 1 2 3 4 8 5 7 0 6
step: 027: 1 2 3 4 0 5 7 8 6
step: 028: 1 2 3 4 5 0 7 8 6
step: 029: 1 2 3 4 5 6 7 8 0
```

6. **DFS** on initial state 5:

```
DFS succeeded after trying 37597 times
solution for the initial state: 1 7 3 0 5 4 6 2 8 
step: 000: 1 7 3 0 5 4 6 2 8
step: 001: 1 7 3 5 0 4 6 2 8
step: 002: 1 7 3 5 4 0 6 2 8
step: 003: 1 7 0 5 4 3 6 2 8
step: 004: 1 0 7 5 4 3 6 2 8
......
step: 046: 1 0 3 4 2 6 7 5 8
step: 047: 1 2 3 4 0 6 7 5 8
step: 048: 1 2 3 4 5 6 7 0 8
step: 049: 1 2 3 4 5 6 7 8 0
```

7. **UCS** on initial state 1:

```
UCS succeeded after trying 70838 times
solution for the initial state: 5 1 2 6 3 0 4 7 8 
step: 000: 5 1 2 6 3 0 4 7 8
step: 001: 5 1 2 6 0 3 4 7 8
step: 002: 5 1 2 0 6 3 4 7 8
step: 003: 0 1 2 5 6 3 4 7 8
step: 004: 1 0 2 5 6 3 4 7 8
step: 005: 1 6 2 5 0 3 4 7 8
step: 006: 1 6 2 5 3 0 4 7 8
step: 007: 1 6 0 5 3 2 4 7 8
step: 008: 1 0 6 5 3 2 4 7 8
step: 009: 1 3 6 5 0 2 4 7 8
step: 010: 1 3 6 5 2 0 4 7 8
step: 011: 1 3 0 5 2 6 4 7 8
step: 012: 1 0 3 5 2 6 4 7 8
step: 013: 1 2 3 5 0 6 4 7 8
step: 014: 1 2 3 0 5 6 4 7 8
step: 015: 1 2 3 4 5 6 0 7 8
step: 016: 1 2 3 4 5 6 7 0 8
step: 017: 1 2 3 4 5 6 7 8 0
```

8. **UCS** on initial state 3:

```
UCS succeeded after trying 31881 times
solution for the initial state: 1 0 2 3 4 5 6 7 8 
step: 000: 1 0 2 3 4 5 6 7 8
step: 001: 0 1 2 3 4 5 6 7 8
step: 002: 3 1 2 0 4 5 6 7 8
step: 003: 3 1 2 4 0 5 6 7 8
......
step: 164: 1 0 3 4 2 6 7 5 8
step: 165: 1 2 3 4 0 6 7 5 8
step: 166: 1 2 3 4 5 6 7 0 8
step: 167: 1 2 3 4 5 6 7 8 0
```

9. **UCS** on initial state 5:

```
UCS succeeded after trying 12743 times
solution for the initial state: 1 7 3 0 5 4 6 2 8 
step: 000: 1 7 3 0 5 4 6 2 8
step: 001: 0 7 3 1 5 4 6 2 8
step: 002: 7 0 3 1 5 4 6 2 8
step: 003: 7 5 3 1 0 4 6 2 8
......
step: 046: 1 2 3 0 5 6 4 7 8
step: 047: 1 2 3 4 5 6 0 7 8
step: 048: 1 2 3 4 5 6 7 0 8
step: 049: 1 2 3 4 5 6 7 8 0
```
