//
//  OPS.c
//  BigHomeWork
//
//  Created by guch8017 on 2019/10/12.
//  Copyright © 2019 guch8017. All rights reserved.
//

#include "OPS.h"


pEventList InitList(){
    pEventList list = (pEventList)malloc(sizeof(struct EventData));
    list->next = list->prev = NULL;
    return list;
}
void AddEvent(pEventList head, enum EventType type, struct HumanData humanData, int time){
    pEventList tmp = (pEventList)malloc(sizeof(struct EventData));
    tmp->type = type;
    tmp->op = humanData;
    tmp->time = time;
    tmp->prev = tmp->next = NULL;
    if(!head->next){
        head->next = head->prev = tmp;
    }else{
        tmp->prev = head->prev;
        head->prev->next = tmp;
        head->prev = tmp;
    }
}
//\e[31;1m Hello, world! \e[0m
int PrintEvent(pEventList head, enum EventType type){
    int totalTime = 0;
    int eventId = 0;
    pEventList p = head->next;
    while (p) {
        eventId++;
        if(p->type & type){
            printf("[%2d]\tTime:[%d]\tOpID:[%d]\t",eventId,p->time,p->op.id);
            switch (p->type) {
                case EVENTTYPE_LEAVE:
                    printf("\e[31;1m未完成目的的情况下离开\e[0m\n");
                    totalTime += MAX_TIME - p->op.arriveTime;
                    break;
                case EVENTTYPE_START:
                    printf("开始处理\n");
                    break;
                case EVENTTYPE_FAIL:
                    printf("\e[31;1m处理失败,进入失败重试队列\e[0m\n");
                    break;
                case EVENTTYPE_ARRIVE:
                    printf("进入等待队列\n");
                    break;
                case EVENTTYPE_FINISH:
                    printf("\e[32;1m成功并离开\e[0m\n");
                    totalTime += p->time - p->op.arriveTime;
                    break;
                default:
                    break;
            }
        }
        p=p->next;
    }
    return totalTime;
}

pQueueHead QueueInit(){
    pQueueHead head = (pQueueHead)malloc(sizeof(struct queueHead));
    head->top = NULL;
    head->base = NULL;
    head->length = 0;
    return head;
}

void QueuePush(pQueueHead head, struct HumanData data){
    pQueueNode tmp = (pQueueNode)malloc(sizeof(struct queueNode));
    tmp->data = data;
    if(!head->length){
        head->top = tmp;
        head->base = tmp;
        tmp->next=NULL;
        tmp->prev=NULL;
    }else{
        head->top->next = tmp;
        tmp->next = NULL;
        tmp->prev = head->top;
        head->top = tmp;
    }
    head->length++;
}

void QueuePop(pQueueHead head){
    if(!head->length){
        printf("队列错误:在空队列中执行POP操作");
        exit(0);
    }
    pQueueNode tmp = head->base;
    head->base = tmp->next;
    if(!head->base){
        head->top = NULL;
    }else{
        head->base->prev = NULL;
    }
    free(tmp);
    head->length--;
}

struct HumanData QueueBack(pQueueHead head){
    if(!head->length){
        printf("队列错误:在空队列中执行BACK操作");
        exit(0);
    }
    return head->base->data;
}

pNode InitNode(struct HumanData data){
    pNode tmp = (pNode)malloc(sizeof(struct Node));
    tmp->data = data;
    tmp->next = NULL;
    return tmp;
}
//此链表有序，依据arriveTime排序
void Insert(pNode head,struct HumanData data){
    pNode tmp = InitNode(data);
    pNode cur = head->next, prev = head;
    while(cur && cur->data.arriveTime<data.arriveTime){
        prev = cur;
        cur = cur->next;
    }
    prev->next = tmp;
    tmp->next = cur;
}

int Compare(pNode head, int time){
    if(!head->next) return 0;
    if(head->next->data.arriveTime <= time) return 1;
    return 0;
}

struct HumanData Pop(pNode head){
    struct HumanData data = head->next->data;
    pNode tmp = head->next;
    head->next = tmp->next;
    free(tmp);
    return data;
}

