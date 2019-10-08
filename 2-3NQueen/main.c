//
//  main.c
//  2-3N皇后问题
//
//  Created by guch8017 on 2019/9/20.
//  Copyright © 2019 guch8017. All rights reserved.
//
//输出全部调用情况
//#define U_DEBUG

#include <stdio.h>
#include <stdlib.h>

struct call_stack{
    int depth;//定位,stack长
    int num;//数值
    struct call_stack * next;
};

typedef  struct call_stack * pStack;
int * nList;
int n=0;
int cnt=0;

pStack init(){
    pStack tmp = (pStack)malloc(sizeof(struct call_stack));
    tmp->next = NULL;
    tmp->depth=0;
    return tmp;
}

void push_back(pStack p, int depth, int i){
    pStack tmp = (pStack)malloc(sizeof(struct call_stack));
    p->depth++;
    tmp->depth = depth;
    tmp->num = i;
    tmp->next = p->next;
    p->next = tmp;
}

pStack back(pStack p){
    return p->next;
}

void pop(pStack p){
    if(p->next){
        p->depth--;
        pStack tmp = p->next;
        p->next = tmp->next;
        free(tmp);
    }else{
        printf("[E] 溢出错误：在栈为空时执行pop指令");
    }
}

char check(int depth, int posy){
    for(int i=1; i<depth; i++){
        if(nList[i] == posy) return 0;
        if(nList[depth - i] == posy - i) return 0;
        if(nList[depth - i] == posy + i) return 0;
    }
    return 1;
}

int Solution(int n){
    nList = (int *)malloc(sizeof(int) * (n+1));
    for(int i=1; i<=n; i++){
        nList[i] = 0;
    }
    pStack p = init();
    int i=1;//相当于i
    int depth=1;//相当于depth
    int flag=2;//0：需要入栈 1:需要出栈 2:正常执行
    int forStatus = 0;//0:递归前 1:递归后
    while (1) {
        //模拟for循环
        if(i>n){
            if(depth==1){
                break;
            }
            i = back(p)->num;
            depth = back(p)->depth;
            pop(p);
            i++;
        }
        //递归前循环执行的语句
        if(flag == 2 && !forStatus){
            forStatus = 1;
            if(depth > n){
                cnt++;
#ifndef U_DEBUG
                printf("%d:",cnt);
                for(int j=1;j<=n;j++){
                    printf(" %d",nList[j]);
                }
                putchar('\n');
#endif
                flag = 1;
                continue;
            }else if(check(depth, i)){
                nList[depth] = i;
                flag = 0;
                continue;
            }else{
                i++;
                continue;
            }
        }
        //递归后循环执行的语句
        if(flag == 2 && forStatus){
            forStatus = 0;
            nList[depth] = 0;
            continue;
        }
        //进栈操作
        if(flag == 0){
            push_back(p, depth, i);
            depth++;
            flag = 2;
#ifdef U_DEBUG
            printf("Calling depth:%d with i=%d",depth,i);
            for(int j=1;j<=n;j++){
                printf(" %d",nList[j]);
            }
            putchar('\n');
#endif
            i=1;
            continue;
        }
        //出栈操作
        if(flag == 1){
            i = back(p)->num;
            depth = back(p)->depth;
            pop(p);
            flag = 2;
            i++;
            continue;
        }
        
    }
    return cnt;
}


//递归版本求解器
/*
void solver(int depth){
    if(depth>n){
        cnt++;
        return;
    }
    
    for(int i=1; i<=n; i++){
        if(check(depth, i)){
            nList[depth] = i;
            solver(depth+1);
            nList[depth] = 0;
        }
    }
}
*/

int main(int argc, const char * argv[]) {
    int n;
    n=8;
    //n = atoi(argv[1]);
    printf("%d\n",Solution(n));
}
