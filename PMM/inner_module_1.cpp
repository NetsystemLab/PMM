#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <chrono>
#include "opts.hpp"
using namespace std;
const int N = 100010;
struct Module{
  int rule;
  uint32_t key[N], mask[N];
  char c[N][33];
  int pri[N];
  int action[N];
  uint32_t value[N];
}mo[20];

int map_string(string a) {
  int ans = 0;
  if (a.find("accept") != -1) { ans = 1;}
  else if (a.find("count") != -1) { ans = 2; }
  else if (a.find("drop") != -1) { ans = 3; }
  else if (a.find("fwd") != -1) { ans = 4; }
  else if (a.find("modify") != -1) { ans = 5; }
  return ans;
}
char str[100], rule[100];
struct Line{
  int64_t l,r; int pri;
};
int cmp(Line a, Line b) {
  if (a.l != b.l) return a.l<b.l;
  else return a.r < b.r;
}
int get_node(int x, int &num, Line *node) {
  int tot = 0;
  for (int i = 0; i < mo[x].rule; i++) {
    int tx = 0, last = -1;
    int len = strlen(mo[x].c[i]);
    for (int j = len-1; j >= 0; j--) {
      if (mo[x].c[i][j] == '*') tx++;
      else {last = j; break;}
    }
    int nx = 0;
    for (int j = last; j >= 0; j--) {
      if (mo[x].c[i][j] == '*') nx++;
    }
    uint32_t tnx = 1<<nx;
    //printf("tx = %d nx = %d\n", tx, nx);
    for (int j = 0; j < tnx; j++) {
      int ans = 0, idx = 0;
      for (int k = 0; k <= last; k++) {
        if (mo[x].c[i][k] == '*') {
          if((1<<idx)&j) {ans = ans*2+1;}
          else {ans = ans*2;}
          idx++;
        }
        else {
          ans = ans*2+(mo[x].c[i][k] - '0');
        }
      }
      if (last == len) {node[num].l = ans; node[num].r = ans; node[num].pri = mo[x].pri[i]; num++;}
      else {node[num].l = ans<<tx; node[num].r = (ans<<tx)+((1<<tx)-1);  node[num].pri = mo[x].pri[i]; num++;}
    }
  }
}
Line n1[N*100], n2[N*100], nx[N*100];
int num1, num2, numx;
void work() {

  for (int i = 0; i < num1; i++) {
    if (numx == 0) {
      nx[numx].l = n1[i].l; nx[numx].r = n1[i].r; nx[numx].pri = n1[i].pri; numx++;
    }
    else {
      if (n1[i].l <= nx[numx-1].r) {

        if (nx[numx].pri > n1[i].pri) {
          n1[i].l = nx[numx-1].r + 1;
          if (n1[i].r <= nx[numx-1].r) {}
          else {
            i--;
          }
        }
        else {
          nx[numx-1].r = n1[i].l - 1;
          i--;
        }
      }
      else {
        nx[numx].l = n1[i].l; nx[numx].r = n1[i].r; nx[numx].pri = n1[i].pri; numx++;
      }
    }
  }
}
int work2() {
  num2 = 0;
  get_node(1, num2, n2);
  sort(n2, n2+num2, cmp);
//  for (int i = 0; i < num2; i++) {
//    printf("n1= %u %u\n", n2[i].l, n2[i].r);
//  }

  for (int i = 0; i < num2; i++) {
    for (int j = 0; j < numx; j++) {
      if (n2[i].l > nx[j].r) continue;
      if (n2[i].r < nx[j].l) {
        if (j == 0) return 1;
        else break;
      }
      else {
        if (n2[i].l < nx[j].l) return 1;
        else if (n2[i].r > nx[j].r){
          if (nx[j].r+1 == nx[j+1].l) {
            n2[i].l = nx[j+1].l;
          }
          else {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}
void mov(char* str) {
    for (int i = 0; i < 8; i++) {
        swap(str[i], str[16+i]);
    }
}
void get_key(uint32_t &key, uint32_t &mask, char* str){
  int len = strlen(str);
  key = 0; mask = 0;
  mov(str);
  for (int i = 0; i < len; i++) {
    if (str[i] == '*') {mask = mask*2; key = key*2;}
    else {
      mask = mask*2+1;
      key = key*2+(str[i]-'0');
    }
  }
  //printf("%x %x\n", key, mask);
}
int main(int argc, char *argv[])
{
  string rule_num = argv[1];
  string file_name = "../dataset/stanford"+rule_num+".txt";
  // string file_name = "../dataset/real_ASx1.txt";
#if gh54
  if(freopen(file_name.c_str(), "r", stdin) == NULL){
    printf("Error, wrong file name: %s", file_name.c_str());
    exit(0);
  }
#endif // gh546
  std::chrono::high_resolution_clock::time_point m1;
  std::chrono::high_resolution_clock::time_point m2;
  std::chrono::high_resolution_clock::time_point m3;
  std::chrono::high_resolution_clock::time_point m4;
  m1 = std::chrono::high_resolution_clock::now();
  int n; scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    int num; scanf("%d", &num);
    int x, value = -1;

    mo[i].rule = num;
    for (int j = 0; j < num; j++) {
      scanf("%s%s", str, rule);
      //printf("%s %s\n", str, rule);
      x = map_string(rule);
      //printf("module = %d rule=%d x = %d\n", i, j, x);
      if (x >= 4) scanf("%d", &value);
      mo[i].action[j] = x; mo[i].value[j] = value; value = -1;

      get_key(mo[i].key[j], mo[i].mask[j], str);
      int len = strlen(str);
      for (int k = 0; k < len; k++) {
        mo[i].c[j][k] = str[k];
      }
      mo[i].c[j][len] = 0;

      scanf("%d", &mo[i].pri[j]);
      //printf("%s %d %d\n", mo[i].c[j], mo[i].value[j], mo[i].pri[j]);
      //if (j == 2) break;
    }
  }

  num1 = 0; numx = 0;
  get_node(0, num1, n1);
  sort(n1, n1+num1, cmp);
  for (int i = 0; i < num1; i++) {
    //printf("n1= %u %u\n", n1[i].l, n1[i].r);
  }
  m2 = std::chrono::high_resolution_clock::now();
  work();
  m3 = std::chrono::high_resolution_clock::now();
  int flag = work2();
  // printf("flag = %d\n", flag);
  m4 = std::chrono::high_resolution_clock::now();
  double interval_generate_time = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(m2-m1).count()/1000000;
  double min_interval_detection_time = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(m3-m2).count()/1000000;
  double conflict_detection_time = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(m4-m3).count()/1000000;
  double total_time = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(m4-m1).count()/1000000;
  printf("%s %.3f %.3f %.3f %.3f flag=%d\n",rule_num.c_str(), interval_generate_time, min_interval_detection_time, conflict_detection_time, total_time, flag);

  // cout<< (std::chrono::duration_cast<std::chrono::nanoseconds>(m4 - m1)).count() << endl;
  // cout<< std::chrono::duration_cast<std::chrono::nanoseconds>(m3 - m2).count() << endl;
  // cout<< std::chrono::duration_cast<std::chrono::nanoseconds>(m4 - m3).count() << endl;
	return 0;
}
