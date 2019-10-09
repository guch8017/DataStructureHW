//
//  main.c
//  约瑟夫环
//
//  Created by guch8017 on 2019/9/18.
//  Copyright © 2019 guch8017. All rights reserved.
//

#define LINK_NODE_NO_DELETE
//#define LINK_NODE_WITH_DELETE
//#define ADT_LIST

#include <stdio.h>
#include <stdlib.h>

char buff[10001];//保存输出

struct node{
    int password;
    int flag;
    int id;
    struct node * next;
};

typedef struct node node;
typedef struct node * pnode;

int str2int(const char * str){
    int sum=0;
    for(int i=0;str[i] != 0;i++){
        if(str[i]>'9' || str[i]<'0'){
            return -1;
        }
        sum = sum * 10 + str[i] - '0';
    }
    return sum;
}

pnode CreateNode(){
    pnode tmp = (pnode)malloc(sizeof(node));
    if(!tmp){
        printf("[E] 溢出错误:malloc分配内存失败。\n");
        exit(0);
    }
    tmp->next = NULL;
    return tmp;
}

void Insert(pnode head, int password){
    static int i = 1;
    if(!head->next){
        head->next = (pnode)malloc(sizeof(node));
        if(!head->next){
            printf("[E] 溢出错误:malloc分配内存失败。\n");
            exit(0);
        }
        head->next->flag = 0;
        head->next->next = head->next;
        head->next->password = password;
        head->next->id = i++;
        return;
    }
    pnode p = head->next;
    while(p->next != head->next){
        p=p->next;
    }
    p->next = (pnode)malloc(sizeof(node));
    if(!p->next){
        printf("[E] 溢出错误:malloc分配内存失败。\n");
        exit(0);
    }
    p->next->flag = 0;
    p->next->next = head->next;
    p->next->password = password;
    p->next->id = i++;
    return;
}

int main(int argc, const char * argv[]) {
    int m,n;
    FILE *fp;
    fp = fopen("out.txt", "w+");
    if(!fp){
        printf("[E] IO错误:无法写入到文件out.txt\n");
        return 0;
    }
    if(argc < 3){
        printf("[E] 参数错误:参数不足，期望：%d 实际：%d\n",2,argc-1);
        return 0;
    }
    m = str2int(argv[1]);//人数
    n = str2int(argv[2]);//停止报数
    if(m<=0 || n<=0){
        printf("[E] 参数错误:参数输入错误，m与n应为正整数\n");
        return 0;
    }
    if(argc < 3 + n){
        printf("[E] 参数错误:参数不足，期望：%d 实际：%d\n",2+n,argc-1);
        return 0;
    }else if(argc > 3 + n){
        printf("[W] 参数过多，多余参数将被舍弃\n");
    }
    //链表用输入，若采用ADT_LIST实现则跳过此处
#ifndef ADT_LIST
    pnode Head = CreateNode();
    for(int i=3;i<n+3;i++){
        int t = str2int(argv[i]);
        if(t<=0){
            printf("[E] 参数错误:参数输入错误.第%d个：%s\n",i-1,argv[i]);
            return 0;
        }
        Insert(Head, t);
    }
    int count = 0;//报数数字
#endif
    //不删除节点方案
#ifdef LINK_NODE_NO_DELETE
pnode p = Head->next;
while(n){
    if(p->flag){
        p = p->next;
        continue;
    }
    count++;
    if(count == m){
        p->flag = 1;
        m = p->password;
        count=0;
        printf("%d,",p->id);
        fprintf(fp, "%d ",p->id);
        n--;
    }
    p = p->next;
}
#endif
//BEGIN OF 删除节点方案
#ifdef LINK_NODE_WITH_DELETE
pnode p1 = Head, p2 = Head->next;
while(n){
    count++;
    if(count == m){
        m = p2->password;
        count = 0;
        n--;
        printf("%d,",p2->id);
        fprintf(fp, "%d ",p2->id);
        p1->next = p2->next;
        free(p2);
        p2 = p1->next;
    }else{
        p1 = p2;
        p2 = p2->next;
    }
}
#endif
    //END OF 删除节点方案
    //BEGIN OF 顺序表方案
#ifdef ADT_LIST
    int * flag = (int *)malloc(sizeof(int)*n);
    for(int i=0;i<n;i++) flag[i]=0;
    int * secret = (int *)malloc(sizeof(int) * n);
    int count = 0;
    for(int i=0;i<n;i++){
        secret[i] = str2int(argv[i+3]);
        if(secret[i] <= 0){
            printf("[E] 参数错误:参数输入错误.第%d个：%s\n",i+2,argv[i+3]);
            return 0;
        }
    }
    int i=0,n1=n;
    while(n1){
        if(flag[i]){
            i++;
            if(i>=n){
                i=0;
            }
            continue;
        }
        count++;
        if(count == m){
            count = 0;
            m = secret[i];
            flag[i]=1;
            n1--;
            printf("%d ",i+1);
            fprintf(fp, "%d ",i+1);
        }
        i++;
        if(i>=n){
            i=0;
        }
    }
    printf("\n");
#endif
//END OF 顺序表方案
    fclose(fp);
}
