//
//  OPS.h
//  BigHomeWork
//
//  Created by guch8017 on 2019/10/12.
//  Copyright © 2019 guch8017. All rights reserved.
//

#ifndef OPS_h
#define OPS_h

#include <stdio.h>
#include <stdlib.h>

static const int TIME = 600;
static const int TOTAL_MONEY = 10000;
static const int MAX_TIME = 600;
static const int MAX_DURATION_TIME = 10;
static const int MIN_MONEY = -10000;
static const int MAX_MONEY = 10000;
static const int PEOPLE_CNT = 100;

enum EventType{
    EVENTTYPE_ARRIVE = 1,
    EVENTTYPE_LEAVE = 2,
    EVENTTYPE_START = 4,
    EVENTTYPE_FINISH = 8,
    EVENTTYPE_FAIL = 16,
    EVENTTYPE_ALL = 0b11111
};

struct HumanData{
    int id;
    int money;
    int arriveTime;
    int durationTime;
    int helper;//用于辨识
};

struct BankData{
    int money;
};

struct EventData{
    struct HumanData op;
    struct EventData * next, * prev;
    int time;
    enum EventType type;
};

struct queueNode{
    struct queueNode * next, * prev;
    struct HumanData data;
};

struct Node{
    struct Node * next;
    struct HumanData data;
};

struct queueHead{
    struct queueNode * top, * base;
    int length;
};





typedef struct queueNode * pQueueNode;
typedef struct queueHead * pQueueHead;
typedef struct Node * pNode;
typedef struct EventData * pEventList;

int PrintEvent(pEventList head, enum EventType type);
pEventList InitList(void);
void AddEvent(pEventList head, enum EventType type, struct HumanData humanData, int time);
pQueueHead QueueInit(void);
void QueuePush(pQueueHead head, struct HumanData data);
void QueuePop(pQueueHead head);
struct HumanData QueueBack(pQueueHead head);
pNode InitNode(struct HumanData data);
void Insert(pNode head,struct HumanData data);
int Compare(pNode head, int time);
struct HumanData Pop(pNode head);

#endif /* OPS_h */
