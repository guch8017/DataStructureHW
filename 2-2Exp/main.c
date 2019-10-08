//
//  main.c
//  2-2Exp
//
//  Created by guch8017 on 2019/9/25.
//  Copyright © 2019 guch8017. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Stack{
    struct Stack * next;
    int c;
};
typedef struct Stack * pStack;

pStack InitStack(){
    pStack tmp = (pStack)malloc(sizeof(struct Stack));
    tmp->c = 0;
    tmp->next = NULL;
    return tmp;
}
void StackPrint(pStack head, char * notice, int type){
    if(head->next == NULL){
        printf("%s:栈空 ",notice);
    }
    else{
        pStack p = head->next;
        printf("%s:",notice);
        while(p){
            if(type){
                printf("%d ",p->c);
            }else{
                printf("%c ",p->c);
            }
            p=p->next;
        }
    }
}

void DebugPrint(pStack op, pStack var, char c){
    printf("Char:%c ",c);
    StackPrint(var, "Var", 1);
    StackPrint(op, "Operator", 0);
    putchar('\n');
}

void StackPush(pStack head, int c){
    pStack tmp = (pStack)malloc(sizeof(struct Stack));
    tmp->c = c;
    tmp->next = head->next;
    head->next = tmp;
    head->c++;
}

void StackPop(pStack head){
    if(head->next != NULL){
        pStack tmp = head->next;
        head->next = tmp->next;
        free(tmp);
        head->c--;
    }else{
        printf("堆栈错误:尝试在空堆栈中执行pop操作\n");
        exit(0);
    }
}

int StackBack(pStack head){
    if(head->next!=NULL){
        return head->next->c;
    }else{
        printf("堆栈错误:尝试在空堆栈中获取元素\n");
        exit(0);
    }
}

//a^b
int power(int a, int b){
    int sum = 1;
    for(int i=0;i<b;i++){
        sum *= a;
    }
    return sum;
}


int main(int argc, const char * argv[]) {
    FILE * fp;
    char * inFIle;
    if(argc == 1){
        inFIle = "in.txt";
    }else if(argc == 2){
        inFIle = argv[1];
    }else{
        printf("命令行:参数错误\n");
        return 0;
    }
    fp = fopen(inFIle, "r");
    char c=0;
    if(!fp){
        printf("I/O:打开文件%s失败","in.txt");
        exit(0);
    }
    int status=0,minus = 0;
    pStack op = InitStack();
    pStack var = InitStack();
    while(!feof(fp)){
        DebugPrint(op, var, c);
        c = fgetc(fp);
        if(c == -1) break;
        if(c == '('){
            status = 0;
            StackPush(op, '(');
            continue;
        }
        if(c == ')'){
            status = 0;
            while(StackBack(op) != '('){
                int a = StackBack(var);
                StackPop(var);
                int b = StackBack(var);
                StackPop(var);
                int c;
                switch(StackBack(op)){
                    case '+':
                        c = a+b;
                        break;
                    case '-':
                        c = b-a;
                        break;
                    case '*':
                        c = a*b;
                        break;
                    case '/':
                        if(a == 0){
                            printf("数学错误:除0错误\n");
                            exit(0);
                        }
                        c = b/a;
                        break;
                    case '^':
                        c = power(b, a);
                        break;
                    default:
                        c=-1;
                        break;
                }
                StackPush(var, c);
                StackPop(op);
            }
            StackPop(op);
            continue;
        }
        if(c == '+' || c == '-'){
            status = 0;
            char opc;
            if(op->c == 0){
                opc = -1;
            }else{
                opc = StackBack(op);
            }
            while(opc == '*' || opc == '/'){
                int a = StackBack(var);
                StackPop(var);
                int b = StackBack(var);
                StackPop(var);
                switch (opc) {
                    case '^':
                        StackPush(var, power(b, a));
                        break;
                    case '*':
                        StackPush(var, a*b);
                        break;
                    case '/':
                        StackPush(var, b/a);
                        break;
                    default:
                        break;
                }
                StackPop(op);
                if(op->c == 0){
                    opc = -1;
                }else{
                    opc = StackBack(op);
                }
            }
            StackPush(op, c);
            continue;
        }
        if(c == '*' || c == '/'){
            status = 0;
            char opc;
            if(op->c == 0){
                opc = -1;
            }else{
                opc = StackBack(op);
            }
            while(opc == '^'){
                int a = StackBack(var);
                StackPop(var);
                int b = StackBack(var);
                StackPop(var);
                StackPush(var, power(b, a));
                StackPop(op);
                if(op->c == 0){
                    opc = -1;
                }else{
                    opc = StackBack(op);
                }
            }
            StackPush(op, c);
            continue;
        }
        if(c == '^'){
            status = 0;
            StackPush(op, c);
            continue;
        }
        if('0'<=c && c<='9'){
            int sum = 0;
            if(status == 1){
                sum = StackBack(var);
                StackPop(var);
            }
            StackPush(var, sum * 10 + c - '0');
            status = 1;
            continue;
        }
    }
    while(op->c){
        int a = StackBack(var);
        StackPop(var);
        int b = StackBack(var);
        StackPop(var);
        int c;
        switch(StackBack(op)){
            case '+':
                c = a+b;
                break;
            case '-':
                c = b-a;
                break;
            case '*':
                c = a*b;
                break;
            case '/':
                if(a == 0){
                    printf("数学错误:除0错误\n");
                    exit(0);
                }
                c = b/a;
                break;
            case '^':
                c = power(b, a);
                break;
            default:
                c=-1;
                break;
        }
        StackPush(var, c);
        StackPop(op);
    }
    printf("Result:%d\n",StackBack(var));
}
