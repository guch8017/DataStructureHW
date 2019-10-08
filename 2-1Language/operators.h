//
//  operators.h
//  2-2Language
//
//  Created by guch8017 on 2019/9/29.
//  Copyright © 2019 guch8017. All rights reserved.
//

#ifndef operators_h
#define operators_h

#include <stdio.h>
#include <stdlib.h>

struct Stack{
    struct Stack * next;
    char c;
};
typedef struct Stack * pStack;

pStack InitStack(void);
void StackPush(pStack head, char c);
void StackPop(pStack head);
char StackBack(pStack head);
int StackEmpty(pStack head);


struct Queue{
    struct Queue * next, * prev;
    char c;
};
struct QueueHead{
    struct Queue * head;
    struct Queue * end;
};
typedef struct Queue * pQueue;
typedef struct QueueHead * pQueueHead;

pQueueHead QueueInit(void);
void QueuePush(pQueueHead head, char c);
void QueuePop(pQueueHead head);
int QueueEmpty(pQueueHead head);
char QueueEnd(pQueueHead head);

#endif /* operators_h */
