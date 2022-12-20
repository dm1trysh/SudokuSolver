#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>

bool CheckSquares(std::vector<std::vector<int>>& ans, int n) {
  const int sq_size = 3;
  for (int hor = 0; hor < 3; hor++) {
    for (int vert = 0; vert < 3; vert++) {
      int min_hor_idx = 3 * hor;
      int max_hor_idx = min_hor_idx + sq_size - 1;
      int min_vert_idx = 3 * vert;
      int max_vert_idx = min_vert_idx + sq_size - 1;
      std::set<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      for (int pos_hor_sq = min_hor_idx; pos_hor_sq <= max_hor_idx; pos_hor_sq++) {
        for (int pos_vert_sq = min_vert_idx; pos_vert_sq <= max_vert_idx; pos_vert_sq++) {
          if (nums.find(ans[pos_hor_sq][pos_vert_sq]) != nums.end()) {
            nums.erase(ans[pos_hor_sq][pos_vert_sq]);
          } else {
            return false;
          }
        }
      }
    }
  }
  return true;
}

bool CheckRight(std::vector<std::vector<int>>& ans, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (ans[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

void PrintRight(std::vector<std::vector<int>>& ans, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cout << ans[i][j] << " ";
    }
    std::cout << "\n";
  }
}

void SearchSolution(std::vector<std::vector<int>>& ans, std::vector<std::set<int>>& poss_vert,
                    std::vector<std::set<int>>& poss_hor, int pos_hor, int pos_vert, int n,
                    bool& found, std::vector<std::vector<std::set<int>>>& squares) {
  if (found) {
    return;
  }
  if (pos_hor == n || pos_vert == n) {
      PrintRight(ans, n);
      found = true;
      return;
  }
  if (ans[pos_hor][pos_vert] == 0) {
    int sq_hor_idx = floor(pos_hor / 3);
    int sq_vert_idx = floor(pos_vert / 3);
    std::set<int> intersection;
    std::set_intersection(poss_vert[pos_vert].begin(), poss_vert[pos_vert].end(),
                          poss_hor[pos_hor].begin(), poss_hor[pos_hor].end(),
                          std::inserter(intersection, intersection.begin()));
    std::set<int> final_intersection;
    std::set_intersection(intersection.begin(), intersection.end(),
                          squares[sq_hor_idx][sq_vert_idx].begin(), squares[sq_hor_idx][sq_vert_idx].end(),
                          std::inserter(final_intersection, final_intersection.begin()));
    for (int poss_value: final_intersection) {
      ans[pos_hor][pos_vert] = poss_value;
      poss_vert[pos_vert].erase(poss_value);
      poss_hor[pos_hor].erase(poss_value);
      squares[sq_hor_idx][sq_vert_idx].erase(poss_value);
      int pos_hor_add = 0;
      int pos_vert_add = 0;
      if (pos_vert == n - 1) {
        pos_vert_add = 1 - n;
        pos_hor_add = 1;
      } else {
        pos_vert_add = 1;
      }
      SearchSolution(ans, poss_vert, poss_hor, pos_hor + pos_hor_add, pos_vert + pos_vert_add,
                     n, found, squares);
      if(found)
        return;
      poss_vert[pos_vert].insert(poss_value);
      poss_hor[pos_hor].insert(poss_value);
      squares[sq_hor_idx][sq_vert_idx].insert(poss_value);
      ans[pos_hor][pos_vert] = 0;
    }
  } else {
    int pos_hor_add = 0;
    int pos_vert_add = 0;
    if (pos_vert == n - 1) {
      pos_vert_add = 1 - n;
      pos_hor_add = 1;
    } else {
      pos_vert_add = 1;
    }
    SearchSolution(ans, poss_vert, poss_hor, pos_hor + pos_hor_add, pos_vert + pos_vert_add,
                   n, found, squares);
  }
}

int main() {
  int n;
  std::cin >> n;
  std::vector<std::vector<int>> sudoku(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cin >> sudoku[i][j];
    }
  }
  std::vector<std::set<int>> appropriate_vertical(n);
  std::vector<std::set<int>> appropriate_horizontal(n);
  std::vector<std::vector<std::set<int>>> squares(3,std::vector<std::set<int>>(
    3, {1, 2, 3, 4, 5, 6, 7, 8 , 9}));
  for (int i = 0; i < n; i++) {
    for (int j = 1; j <= n; j++) {
      appropriate_horizontal[i].insert(j);
      appropriate_vertical[i].insert(j);
    }
  }
  for (int vert = 0; vert < n; vert++) {
    for (int hor = 0; hor < n; hor++) {
      if (sudoku[hor][vert] != 0) {
        int sq_hor_idx = floor(hor / 3);
        int sq_vert_idx = floor(vert / 3);
        squares[sq_hor_idx][sq_vert_idx].erase(sudoku[hor][vert]);
        appropriate_vertical[vert].erase(sudoku[hor][vert]);
        appropriate_horizontal[hor].erase(sudoku[hor][vert]);
      }
    }
  }
  bool has_found = false;
  SearchSolution(sudoku, appropriate_vertical, appropriate_horizontal, 0, 0, n, has_found, squares);
  return 0;
}