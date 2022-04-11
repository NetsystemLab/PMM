#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <time.h>
using namespace std;
const int N = 5010;

int main()
{

  freopen("../dataset/tmp2.txt", "w", stdout);

  int n = 60;
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    int x = rand()%1000+1;
    double y = (double)x/1000;
    int z = rand()%3+1;
    printf("%.3f\n", 2+y);
  }
	return 0;
}
