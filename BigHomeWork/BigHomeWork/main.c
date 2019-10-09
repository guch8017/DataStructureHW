//
//  main.c
//  BigHomeWork
//
//  Created by guch8017 on 2019/10/9.
//  Copyright © 2019 guch8017. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static const int TIME = 600;
static const int TOTAL_MONEY = 10000;
static const int MAX_TIME = 600;
static const int MAX_DURATION_TIME = 50;
static const int MIN_MONEY = -10000;
static const int MAX_MONEY = 10000;

struct HumanData{
    int id;
    int money;
    int arriveTime;
    int durationTime;
};

int Random(const int min, const int max){
    int delta = max - min;
    int res = rand()%delta;
    return res + min;
}

struct HumanData GenerateHumanData(){
    static int id = 0;
    id++;
    struct HumanData tmp;
    tmp.id = id;
    tmp.arriveTime = Random(0,MAX_TIME);
    tmp.durationTime = Random(0,MAX_DURATION_TIME);
    tmp.money = Random(MIN_MONEY,MAX_MONEY);
    return tmp;
}

struct queueNode{
    struct queueNode * next, * prev;
    struct HumanData data;
};

struct queueHead{
    struct queueNode * next, * prev;
};

typedef struct queueNode pQueueNode;
typedef struct queueHead pQueueHead;



int main(int argc, const char * argv[]) {

    return 0;
}
