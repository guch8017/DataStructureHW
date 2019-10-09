//
//  main.c
//  2-2Exp
//
//  Created by guch8017 on 2019/9/25.
//  Copyright ? 2019 guch8017. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stack{
    struct Stack * next;
    int c;
};
typedef struct Stack * pStack;
struct Varables{
    struct Varables * next;
    int num;
    char * name;
};
typedef struct Varables * pVar;
pVar VarList = NULL;

int GetNumByName(char * name){
    pVar p = VarList->next;
    while(p){
        if(!strcmp(p->name, name)){
            return p->num;
        }
        p = p->next;
    }
    printf("变量错误:变量表中无对应变量\n");
    exit(0);
}

void InitVarList(){
    VarList = (pVar)malloc(sizeof(struct Varables));
    VarList->next = NULL;
    VarList->name = NULL;
    VarList->num = 0;
}

void InsertVarList(const char * name, int num){
    pVar tmp = (pVar)malloc(sizeof(struct Varables));
    tmp->next = VarList->next;
    tmp->num = num;
    tmp->name = (char *)malloc(sizeof(char)*(strlen(name) + 1));
    strcpy(tmp->name, name);
    VarList->next = tmp;
    VarList->num++;
}

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

char * stack2str(pStack p){
    if(p->c == 0){
        printf("Error:UNknown Err\n");
    }
    char * str = (char *)malloc(sizeof(char) * (p->c+1));
    str[p->c] = 0;
    while(p->c){
        str[p->c - 1] = StackBack(p);
        StackPop(p);
    }
    
    return str;
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
    char c=0;
    if(!infp){
        printf("I/O:打开输入文件失败\n");
        exit(0);
    }
    if(!outfp){
        printf("I/O:打开输出文件失败\n");
        exit(0);
    }
    InitVarList();
    int status=4;//0:正常表达式 1:接收整形数值 2:接收被赋值变量 3:接收变量 4:初始状态
    int hasVar=0;//是否有变量待赋值
    pStack op = InitStack();
    pStack var = InitStack();
    pStack Variable = InitStack();
    char * varName=NULL;
    while(!feof(infp)){
        DebugPrint(op, var, c);
        c = fgetc(infp);
        //putchar(c);
        fputc(c,outfp);
        if(c == -1) break;
        
        if((('a'<=c && c<='z') || ('A'<=c && c<='Z') || c == '_')){
            if(status == 1){
                printf("变量错误:变量名不合法\n");
                exit(0);
            }
            if(status == 0) status = 3;
            if(status == 4) status = 2;
            StackPush(Variable,c);
            char t = fgetc(infp);
            if(t!=-1) fseek(infp,-1,SEEK_CUR);
            if((('a'<=t && t<='z') || ('A'<=t && t<='Z') || t == '_' || ('0'<=t && t<='9'))){
                continue;
            }
            else{
                if(status == 2 && t=='='){
                    varName = stack2str(Variable);
                    hasVar = 1;
                }else{
                    StackPush(var, GetNumByName(stack2str(Variable)));
                }
                continue;
            }
        }else if(status == 4){
            status = 0;
        }
        
        if('0'<=c && c<='9'){
            if(status == 0 || status == 1){
                int sum = 0;
                if(status == 1){
                    sum = StackBack(var);
                    StackPop(var);
                }
                StackPush(var, sum * 10 + c - '0');
                status = 1;
                continue;
            }else{
                StackPush(Variable,c);
                char t = fgetc(infp);
                if(t!=-1) fseek(infp,-1,SEEK_CUR);
                if((('a'<=t && t<='z') || ('A'<=t && t<='Z') || t == '_' || ('0'<=t && t<='9'))){
                    continue;
                }
                else{
                    if(status == 2){
                        varName = stack2str(Variable);
                    }else if(status == 3){
                        StackPush(var, GetNumByName(stack2str(Variable)));
                    }
                    continue;
                }
            }
        }
        
        
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
        if(c == '='){
            if(hasVar == 1 && status == 2){
                status = 0;
                continue;
            }else{
                printf("格式错误:等号左端无变量\n");
            }
        }
        if(c == ';'){
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
            printf("=%d\n",StackBack(var));
            fprintf(outfp, "=%d\n", StackBack(var));
            if(hasVar){
                printf("Var %s = %d\n",varName,StackBack(var));
                InsertVarList(varName,StackBack(var));
            }
            StackPop(var);
            hasVar = 0;
            status = 4;
            continue;
        }
        printf("未识别的字符\n");
        exit(0);
    }
    if(!op->c){
        return 0;
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
    fprintf(outfp, "=%d\n", StackBack(var));
    fclose(infp);
    fclose(outfp);
}
