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

void StackPrint(pStack head, FILE * outfp){
    putchar('(');
    fputc(outfp,'(');
    pStack p = head;
    for(int i=0;i<head->i;i++){
        p = p->next;
        printf("%d,",p->num);
        fprintf(outfp,"%d,",p->num);
    }
    printf("\b)\n");
    fprintf(outfp,"\b)\n");
}

void Solution(FILE * outfp){
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
            StackPrint(head,outfp);
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
    int param[2] = {0};
    for(int i=1;i<argc;i+=2){
        if(!strcmp(argv[i], "-i")){
            if(argc < i + 2){
                printf("Option %s requires an argument\n",argv[i]);
                return 0;
            }
            param[0] = i+1;
        }else if(!strcmp(argv[i], "-o")){
            if(argc < i + 2){
                printf("Option %s requires an argument\n",argv[i]);
                return 0;
            }
            param[1] = i+1;
        }else if(!strcmp(argv[i], "-?")){
            printf("decode [-i input file][-o output file]\n");
            printf("\t-i\t\t#Input file. If not defined will use 'in.txt'\n");
            printf("\t-o\t\t#Output file. If not defined will use 'out.txt'\n");
            return 0;
        }else{
            printf("Unknown option: %s\nUse param -? to get help.\n",argv[i]);
            return 0;
        }
    }
    //char * s;//读入数据
    FILE * infp;
    FILE * outfp;
    if(param[0]){
        infp = fopen(argv[param[0]], "r");
    }else{
        infp = fopen("in.txt", "r");
    }
    if(param[1]){
        outfp = fopen(argv[param[1]], "w");
    }else{
        outfp = fopen("out.txt", "w");
    }
    if(!infp){
        printf("[E] 无法打开输入文件\n");
        return 0;
    }
    if(!outfp){
        printf("[E] 无法打开输出文件\n");
        return 0;
    }
    fscanf(infp,"%d%d",&T,&n);
    data = (int *)malloc(sizeof(int) * n);
    for(int i=0;i<n;i++){
        fscanf(infp,"%d",data + i);
    }
    Solution(outfp);
    return 0;
}
