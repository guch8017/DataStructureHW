//
//  operators.c
//  2-2Language
//
//  Created by guch8017 on 2019/9/29.
//  Copyright © 2019 guch8017. All rights reserved.
//

#include "operators.h"


/* --------- START OF STACK OPERATOR ------------- */

pStack InitStack(){
    pStack tmp = (pStack)malloc(sizeof(struct Stack));
    tmp->c = 0;
    tmp->next = NULL;
    return tmp;
}

void StackPush(pStack head, char c){
    pStack tmp = (pStack)malloc(sizeof(struct Stack));
    tmp->c = c;
    tmp->next = head->next;
    head->next = tmp;
}

void StackPop(pStack head){
    if(head->next != NULL){
        pStack tmp = head->next;
        head->next = tmp->next;
        free(tmp);
    }else{
        printf("堆栈错误:尝试在空堆栈中执行pop操作\n");
        exit(0);
    }
}

char StackBack(pStack head){
    if(head->next!=NULL){
        return head->next->c;
    }else{
        printf("堆栈错误:尝试在空堆栈中获取元素\n");
        exit(0);
    }
}

int StackEmpty(pStack head){
    return (head->next == NULL);
}

/* --------- START OF QUEUE OPERATOR ------------- */

pQueueHead QueueInit(){
    pQueueHead head = (pQueueHead)malloc(sizeof(struct QueueHead));
    head->end = NULL;
    head->head = NULL;
    return head;
}

void QueuePush(pQueueHead head, char c){
    pQueue node = (pQueue)malloc(sizeof(struct Queue));
    node->c = c;
    if(head->head == NULL){
        head->head = node;
        head->end = node;
        node->next = NULL;
        node->prev = NULL;
    }else{
        node->next = head->head;
        node->prev = NULL;
        head->head->prev = node;
        head->head = node;
    }
    //printf("队列:已插入%p\n",node->value);
}

void QueuePop(pQueueHead head){
    if(head->end == NULL){
        printf("队列错误:尝试在空队列中执行POP操作");
        return;
    }
    if(head->end == head->head){
        head->head = NULL;
    }
    pQueue tmp = head->end;
    //printf("队列:已弹出%p\n",tmp->value);
    head->end = tmp->prev;
    free(tmp);
}
//返回队列是否为空
int QueueEmpty(pQueueHead head){
    return (head->head == NULL);
}
//返回队列尾部元素
char QueueEnd(pQueueHead head){
    return head->end->c;
}
