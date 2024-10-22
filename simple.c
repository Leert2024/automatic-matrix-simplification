#include "simple.h"

int max_num(int a,int b){
    if(a<b)return b;
    return a;
}

int min_num(int a,int b){
    if(a>b)return b;
    return a;
}

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}
