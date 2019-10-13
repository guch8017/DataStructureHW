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
#include "OPS.h"




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



int main(int argc, const char * argv[]) {
    //随即种子
    srand((unsigned int)time(NULL));
    pQueueHead waitQueue,unsolveQueue;
    struct HumanData human;//当前正在处理的人
    pNode humanQueue = InitNode(human);
    pEventList eventList = InitList();
    waitQueue = QueueInit(); //队列1排队队列
    unsolveQueue = QueueInit(); //队列2失败队列
    int flag = 0;//是否有人在处理
    int finishTime = 0;//处理当前事件的结束时间
    //Generate List
    for(int i=0;i<PEOPLE_CNT;i++){
        Insert(humanQueue, GenerateHumanData());
    }
    int money = TOTAL_MONEY;
    //时间循环
    for(int time=0;time<TIME;time++){
        //将到达的人插入等待队列
        while (Compare(humanQueue, time)) {
            struct HumanData hmdata = Pop(humanQueue);
            QueuePush(waitQueue, hmdata);
            AddEvent(eventList, EVENTTYPE_ARRIVE, hmdata, time);
        }
        //处理正在进行的事件
        if(flag){
            if(time >= finishTime){
                flag = 0;
                if(human.money <= money){ //成功操作
                    money -= human.money;
                    AddEvent(eventList, EVENTTYPE_FINISH, human, time);
                    if(human.money < 0){ //若为存款
                        if(unsolveQueue->length){
                            while (QueueBack(unsolveQueue).helper < time) {
                                struct HumanData hmd = QueueBack(unsolveQueue);
                                hmd.helper = time;
                                QueuePop(unsolveQueue);
                                if(hmd.money <= money){
                                    AddEvent(eventList, EVENTTYPE_START, hmd, time);
                                    human = hmd;
                                    finishTime = time + hmd.durationTime;
                                    flag = 1;
                                    break;
                                }else{
                                    QueuePush(unsolveQueue, hmd);
                                }
                            }
                        }
                    }
                }else{
                    AddEvent(eventList, EVENTTYPE_FAIL, human, time);
                    human.helper = time;
                    QueuePush(unsolveQueue, human);
                }
            }
        }
        if(!flag){  //没人，添加一个
            if(!waitQueue->length){
                continue;
            }
            flag=1;
            human = QueueBack(waitQueue);
            QueuePop(waitQueue);
            AddEvent(eventList, EVENTTYPE_START, human, time);
            finishTime = time + human.durationTime;
        }
    }
    //时间到，所有人离开
    if(flag){
        AddEvent(eventList, EVENTTYPE_LEAVE, human, MAX_TIME);
    }
    while (waitQueue->length) {
        AddEvent(eventList, EVENTTYPE_LEAVE, QueueBack(waitQueue), MAX_TIME);
        QueuePop(waitQueue);
    }
    while(unsolveQueue->length){
        AddEvent(eventList, EVENTTYPE_LEAVE, QueueBack(unsolveQueue), MAX_TIME);
        QueuePop(unsolveQueue);
    }
    int totalTime = PrintEvent(eventList, EVENTTYPE_ALL);
    printf("Total Wait Time:%d Average Wait Time: %.2lf\n",totalTime,(double)totalTime/PEOPLE_CNT);
    return 0;
}
