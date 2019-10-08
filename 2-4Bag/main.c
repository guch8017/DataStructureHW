//
//  main.c
//  2-4Bag
//
//  Created by guch8017 on 2019/9/22.
//  Copyright © 2019 guch8017. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

struct stack{
    int num;
    int i;
    struct stack * next;
};
typedef struct stack * pStack;
int T,n;
int * data;

pStack StackInit(){
    pStack p = (pStack)malloc(sizeof(struct stack));
    p->i = 0;
    p->num = 0;
    p->next = NULL;
    return p;
}

void StackPushBack(pStack head, int num, int i){
    pStack tmp = (pStack)malloc(sizeof(struct stack));
    tmp->num = num;
    tmp->i = i;
    tmp->next = head->next;
    head->next = tmp;
    head->i++;
    head->num+=num;
}

pStack StackBack(pStack head){
    return head->next;
}

void StackPop(pStack head){
    pStack tmp = head->next;
    head->next = tmp->next;
    head->i--;
    head->num-=tmp->num;
    free(tmp);
}

void StackPrint(pStack head){
    putchar('(');
    pStack p = head;
    for(int i=0;i<head->i;i++){
        p = p->next;
        printf("%d,",p->num);
    }
    printf("\b)\n");
}

void Solution(){
    pStack head = StackInit();
    int i=0;
    while(1){
        if(i>=n){
            if(head->num == 0){
                break;
            }
            else{
                i = StackBack(head)->i;
                StackPop(head);
                i++;
            }
        }
        if(head->num + data[i] < T){
            StackPushBack(head, data[i], i);
            i++;
            continue;
        }
        if(head->num + data[i] == T){
            StackPushBack(head, data[i], i);
            StackPrint(head);
            StackPop(head);
            i++;
            continue;
        }
        if(head->num + data[i] > T){
            i++;
        }
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    T = atoi(argv[1]);
    n = atoi(argv[2]);
    data = (int *)malloc(sizeof(int) * n);
    for(int i=0;i<n;i++){
        data[i] = atoi(argv[i+3]);
    }
    Solution();
    return 0;
}
