//
//  main.c
//  3Tree
//
//  Created by guch8017 on 2019/9/24.
//  Copyright © 2019 guch8017. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char buff[1001];
char buff2[1001];
/* --------- START OF STRUCT DEF ------------- */
enum TrvOrder{
    PreOrder = 0,
    MidOrder = 1,
    PostOrder = 2,
    LevelOrder = 3
};
struct BTree{
    char c;
    struct BTree * left, * right;
};
struct BTreeHead{
    struct BTree * next;
};
struct Queue{
    struct Queue * next, * prev;
    struct BTree * value;
};
struct QueueHead{
    struct Queue * head;
    struct Queue * end;
};
typedef struct BTree * pBTree;
typedef struct BTreeHead * pBTreeHead;
typedef struct Queue * pQueue;
typedef struct QueueHead * pQueueHead;
/* --------- START OF QUEUE OPERATOR ------------- */
pQueueHead QueueInit(){
    pQueueHead head = (pQueueHead)malloc(sizeof(struct QueueHead));
    head->end = NULL;
    head->head = NULL;
    return head;
}

void QueuePush(pQueueHead head, pBTree tree){
    pQueue node = (pQueue)malloc(sizeof(struct Queue));
    node->value = tree;
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
pBTree QueueEnd(pQueueHead head){
    return head->end->value;
}
/* --------- START OF BTREE OPERATOR ------------- */
//初始化二叉树头指针
pBTreeHead BTreeInit(){
    pBTreeHead tmp = (pBTreeHead)malloc(sizeof(struct BTreeHead));
    tmp->next = NULL;
    return tmp;
}
//辅助函数：创建一个新的二叉树节点
pBTree __BTreeNodeInit(char c){
    pBTree tmp = (pBTree)malloc(sizeof(struct BTree));
    tmp->left = NULL;
    tmp->right = NULL;
    tmp->c = c;
    return tmp;
}
//由层次便利创建二叉树 %代表空节点
void BTreeCreateByLevelTrv(pBTreeHead head, char c[], unsigned int length){
    if(length <= 0) return;
    if(c <= 0) return;
    pQueueHead q = QueueInit();
    pBTree p = __BTreeNodeInit(c[0]);
    head->next = p;
    QueuePush(q, p);
    int i=1;
    while(i<length){
        pBTree tmp = QueueEnd(q);
        if(tmp!=NULL){
            if(c[i] != '%'){
                pBTree p1 = __BTreeNodeInit(c[i]);
                tmp->left = p1;
                QueuePush(q, p1);
            }else{
                QueuePush(q, NULL);
            }
            i++;
            if(i<length){
                if(c[i] != '%'){
                    pBTree p1 = __BTreeNodeInit(c[i]);
                    tmp->right = p1;
                    QueuePush(q, p1);
                }else{
                    QueuePush(q, NULL);
                }
            }
        }else{
            if(c[i]!='%' || (++i<length && c[i]!='%')){
                printf("输入有误!\n");
                exit(0);
            }
        }
        i++;
        QueuePop(q);
    }
}

pBTree __BTreeCreateMPT(char mid[], char post[], const unsigned long length){
    if (length == 0) return NULL;
    if (length == 1) return __BTreeNodeInit(post[length-1]);
    char root = post[length-1];
    int i;
    for(i=0;i<length;i++){
        if(mid[i] == root){
            break;
        }
    }
    if(i==length && mid[i-1]!=root){
        printf("二叉树:由中后序遍历队列创建失败\n");
        exit(0);
    }
    pBTree rootp = __BTreeNodeInit(root);
    rootp->left = __BTreeCreateMPT(mid, post, i);
    rootp->right = __BTreeCreateMPT(mid+i+1, post+i, length-1-i);
    return rootp;
};

pBTree __BTreeCreateMPRT(char mid[],char pre[], unsigned int length){
    if(length == 0) return NULL;
    if(length == 1) return __BTreeNodeInit(pre[0]);
    char root = pre[0];
    int i;
    for(i=0;i<length;i++){
        if(mid[i] == root){
            break;
        }
    }
    if(i==length && mid[i-1]!=root){
        printf("二叉树:由中先序遍历队列创建失败\n");
        exit(0);
    }
    pBTree rootp = __BTreeNodeInit(root);
    rootp->left = __BTreeCreateMPRT(mid, pre+1, i);
    rootp->right = __BTreeCreateMPRT(mid+1+i, pre+1+i, length-1-i);
    return rootp;
}

//由中后序遍历队列创建二叉树
void BTreeCreateByMidAndPostTrv(pBTreeHead head, char mid[], char post[], unsigned int length){
    head->next = __BTreeCreateMPT(mid, post, length);
}
void BTreeCreateByMidAndPreTrv(pBTreeHead head, char mid[], char pre[], unsigned int length){
    head->next = __BTreeCreateMPRT(mid, pre, length);
}
//遍历二叉树
void __BTreePreOrderTrv(pBTree p){
    printf("%c",p->c);
    if(p->left) __BTreePreOrderTrv(p->left);
    if(p->right) __BTreePreOrderTrv(p->right);
}
void __BTreeMidOrderTrv(pBTree p){
    if(p->left) __BTreeMidOrderTrv(p->left);
    printf("%c",p->c);
    if(p->right) __BTreeMidOrderTrv(p->right);
}
void __BTreePostOrderTrv(pBTree p){
    if(p->left) __BTreePostOrderTrv(p->left);
    if(p->right) __BTreePostOrderTrv(p->right);
    printf("%c",p->c);
}
void __BTreeLevelOrderTrv(pBTree p){
    pQueueHead queue = QueueInit();
    pBTree tmp;
    QueuePush(queue, p);
    while(!QueueEmpty(queue)){
        tmp = QueueEnd(queue);
        printf("%c",tmp->c);
        if(tmp->left) QueuePush(queue, tmp->left);
        if(tmp->right) QueuePush(queue, tmp->right);
        QueuePop(queue);
    }
}

void BTreeTrv(pBTreeHead head, enum TrvOrder order){
    if(!head->next){
        printf("二叉树遍历错误:空的头节点\n");
        exit(0);
    }
    if(order == PreOrder) __BTreePreOrderTrv(head->next);
    else if(order == MidOrder) __BTreeMidOrderTrv(head->next);
    else if(order == PostOrder) __BTreePostOrderTrv(head->next);
    else if(order == LevelOrder) __BTreeLevelOrderTrv(head->next);
}

//测试
int main(int argc, const char * argv[]) {
    scanf("%s",buff);
    scanf("%s",buff2);
    pBTreeHead tree = BTreeInit();
    //BTreeCreateByLevelTrv(tree, buff, (unsigned int)strlen(buff));
    BTreeCreateByMidAndPreTrv(tree, buff, buff2, (unsigned int)strlen(buff));
    BTreeTrv(tree, MidOrder);
    putchar('\n');
    BTreeTrv(tree, PreOrder);
    putchar('\n');
    BTreeTrv(tree, PostOrder);
    putchar('\n');
    //printf("Hello, World!\n");
    return 0;
    
}
