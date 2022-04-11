#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "opts.hpp"
using namespace std;
const int N = 5010;
void map_string(int a) {
  if (a == 1) printf("accept");
  if (a == 2) printf("count");
  if (a == 3) printf("drop");
  if (a == 4) {printf("fwd "); printf("%d", rand()%100+1);}
  if (a == 5) {printf("modify "); printf("%d", rand()%100+1);}
}
int main(int argc, char *argv[])
{
  string rule_num = argv[1];
  string file_name = "../dataset/outk"+rule_num+".txt";
#if gh54
  freopen(file_name.c_str(), "w", stdout);
#endif // gh546
  int n = atoi(rule_num.c_str());
  printf("2\n");
  printf("%d\n", n);
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    int x = rand()%5+1;
    int y = rand()%1+1;
    if (y == 1) {
      for (int j = 0; j < 32-x; j++) printf("%d", rand()%2);
      for (int j = 32-x; j < 32; j++) printf("*");
      printf(" ");
    } else {
      int num = x;
      for (int j = 0; j < 32; j++) {
        if (32-j == num) {
          printf("*"); num--; continue;
        }
        if (num && rand()%2) {
          printf("*"); num--;
        }
        else {
          printf("%d", rand()%2);
        }
      }
      printf(" ");
    }

    int z = rand()%5+1;
    map_string(z);
    printf(" %d\n", n-i);
  }
  printf("1\n");
  for (int i = 0; i < 1; i++) {
    int x = rand()%5+1;
    int y = rand()%1+1;
    if (y == 1) {
      for (int j = 0; j < 32-x; j++) printf("%d", rand()%2);
      for (int j = 32-x; j < 32; j++) printf("*");
      printf(" ");
    }
    else {
      int num = x;
      for (int j = 0; j < 32; j++) {
        if (32-j == num) {
          printf("*"); num--; continue;
        }
        if (num && rand()%2) {
          printf("*"); num--;
        }
        else {
          printf("%d", rand()%2);
        }
      }
      printf(" ");
    }

    int z = rand()%5+1;
    map_string(z);
    printf(" %d\n", 0);
  }
	return 0;
}
