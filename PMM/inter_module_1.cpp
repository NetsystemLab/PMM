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
struct Module{
  int rule;
  uint32_t key[3], mask[3];
  char c[3][32];
  int action[3];
  uint32_t value[3];
}mo[50];

struct Link{
  int L, R, is;  // is == 1(+), 2(>>), 3(&)
}link[20];

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
  uint32_t l,r;
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
      if (last == len) {node[num].l = ans; node[num].r = ans; num++;}
      else {node[num].l = ans<<tx; node[num].r = (ans<<tx)+((1<<tx)-1); num++;}
    }
  }
}

int check1(int l, int r) {
  if (mo[l].rule != mo[r].rule) return 1;
  int flag = 0;
  for (int i = 0; i < mo[l].rule; i++) {
    int f = 0;
    for (int j = 0; j < mo[r].rule; j++) {
      if (mo[l].key[i] == mo[r].key[j] && mo[l].mask[i] == mo[r].mask[j]) {
        f=1; break;
      }
    }
    if (f == 0) {flag = 1; break;}
  }
  return flag;
}
int check2(int l, int r) {
  int flag = 0;
  for (int i = 0; i < mo[l].rule; i++) {
    if (mo[l].action[i] == 3) {
      for (int j = 0; j < mo[r].rule; j++) {
        uint32_t msk= mo[l].mask[i]&mo[r].mask[j];
        if ((mo[l].key[i]&msk) == (mo[r].key[j]&msk)) {
          flag = 1;
        }
      }
    }
    else if (mo[l].action[i] >= 4) {
      uint32_t value = mo[l].value[i];
      for (int j = 0; j < mo[r].rule; j++) {
        uint32_t msk = value&mo[r].mask[j];
        if (mo[l].key[i] == mo[r].key[j] && mo[l].mask[i] == mo[r].mask[j]) {
          if((mo[r].key[j]&msk) == msk);
          else {flag = 1;}
        }
        else {
          msk= mo[l].mask[i]&mo[r].mask[j];
          if ((mo[l].key[i]&msk) == (mo[r].key[j]&msk)) flag = 1;
        }
      }
    }
  }
  return flag;
}
Line n1[200], n2[200];
int check3(int l, int r) {
  int num1 = 0;
  get_node(l, num1, n1);
  sort(n1, n1+num1, cmp);
  int num2 = 0;
  get_node(r, num2, n2);
  sort(n2, n2+num2, cmp);
//  for (int i = 0; i < num1; i++) {
//    printf("n1= %u %u\n", n1[i].l, n1[i].r);
//  }
//  for (int i = 0; i < num2; i++) {
//    printf("n2= %u %u\n", n2[i].l, n2[i].r);
//  }
  int ix1 = 0, ix2 = 0;
  int flag = 0;
  while(ix1 < num1 && ix2 < num2) {
    if (n1[ix1].l > n2[ix2].l) {flag = 1; break;}
    else {
      if (n1[ix1].r >= n2[ix2].r) {ix2++;}
      else if (n1[ix1].r < n2[ix2].l) {ix1++;}
      else {flag = 1; break;}
    }
  }
  return flag;
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
int main()
{
#if gh54
  freopen("../dataset/in1.txt", "r", stdin);
#endif // gh546
  int n; scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    int num; scanf("%d", &num);
    int x, value = -1;

    mo[i].rule = num;
    for (int j = 0; j < num; j++) {
      scanf("%s%s", str, rule);
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
    }
  }
  int m; scanf("%d", &m);
  for (int i = 0; i < m; i++) {
    scanf("%d%d%d", &link[i].L, &link[i].R, &link[i].is);
  }
  clock_t t1 = clock();
  for (int i = 0; i < m; i++) {
    int l = link[i].L, r = link[i].R, is = link[i].is;
    int flag = 0;
    if (is == 1) {
      flag = check1(l, r);
    }
    else if (is == 2) {
      flag = check2(l, r);
    }
    else if (is == 3) {
      flag = check3(l, r);
    }
    if (flag) {
      printf("module = %d %d is = %d\n", l, r, is);
      flag = 0;
    }
  }

  clock_t t2 = clock();
  double t = (double)(t2-t1);
  cout << (double)(t2-t1)*1000  << endl;
	return 0;
}
