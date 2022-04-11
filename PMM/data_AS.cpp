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
char str[1010];
char ans[1010], bns[1010];

void process(int num, int id) {

    for (int i = id*8; i < (id+1)*8; i++) {
        int len = 7-(i-id*8);
        if (num&(1<<len)) {
            ans[i] = '1';
        }
        else {
            ans[i] = '0';
        }
    }
    //printf("num= %d id = %d %s\n", num, id, ans);
}
void process2(int num, int id) {
    for (int i = id*8; i < (id+1)*8; i++) {
        int len = 7-(i-id*8);
        if (num&(1<<len)) {
            bns[i] = '1';
        }
        else {
            bns[i] = '0';
        }
    }
}
int main()
{
#if gh546
    freopen("../dataset/real_AS1.txt", "r", stdin);
    freopen("../dataset/real_ASx1.txt", "w", stdout);
#endif // gh546
    int tx = 4000, pri = 0;
    while(gets(str)) {
        if(pri == 4000) break;
        int len = strlen(str);
        int st  = 0;
        while(str[st]<'0' || str[st]>'9') st++;
        //printf("str= %s st=%d\n", str, st);
        memset(ans,0,sizeof(ans));
        memset(bns,0,sizeof(bns));
        int num = 0, cnt = 0, x = 0;
        for (int i = st; i < len; i++) {
            if (str[i]==' ') {
                st=i; break;
            }
            if (str[i]=='/') {
                process(num, cnt);
                num = 0; cnt++;
                i++;
                while(str[i]>='0'&&str[i]<='9'){
                    x=x*10+str[i]-'0'; i++;
                }
                i--; continue;
            }
            if (str[i]=='.') {
                process(num, cnt);
                num = 0; cnt++;
            }
            if (str[i] >= '0' && str[i] <= '9') {
                //printf("c=%c\n",str[i]);
                num = num*10+(str[i]-'0');
            }
        }
        for (int i = 31; i > 31-x; i--) ans[i]='*';
        for (int i = 31-x; i >= 0; i--) {
            int t = rand()%2;
            if (t == 0) ans[i]='0';
            else ans[i] = '1';
        }
        st++;
        //printf("%d\n",st);
        num = 0; cnt = 0;
        for (int i = st; i < len; i++) {
                if (str[i]=='.'||str[i]==' ') {
                    process2(num, cnt);
                    num = 0; cnt++;
                    if (str[i] == ' ') break;
                }
                if (str[i] >= '0' && str[i] <= '9') {
                    num = num*10+(str[i]-'0');
                }
        }
        //printf("%s ", ans, bns);
        printf("%s ", ans);
        int z = rand()%5+1;
        map_string(z);
        printf(" %d\n", tx-pri); pri++;
    }
    return 0;
}
