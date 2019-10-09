//
//  main.c
//  2-5MMLDeque
//
//  Created by guch8017 on 2019/9/21.
//  Copyright © 2019 guch8017. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Type{
    DICT_INT_INT = 0,
    DICT_INT_STR = 1,
    DICT_STR_INT = 2,
    DICT_STR_STR = 3,
    OPERATOR = 4,
    CHAR = 5,
    PARAM_BLOCK = 6,
    PARAM = 7,
};
//状态
enum Status{
    WAIT_OPERATOR = 0,
    WAIT_OPERATOR_END = 1,
    WAIT_COLON = 2,
    WAIT_L_BRACKET = 3,
    WAIT_PARAM = 4,
    WAIT_D_FIRST = 5,
    WAIT_S_FIRST = 6,
    WAIT_D_QUOTATION = 7,
    WAIT_S_QUOTATION = 8,
    WAIT_INT = 9,
    WAIT_EQUAL = 10,
    WAIT_VALUE = 11,
    WAIT_D_QUOTATION_V = 12,
    WAIT_S_QUOTATION_V = 13,
    WAIT_INT_V = 14,
    WAIT_PARAM_END =  15,
    WAIT_NEXT_OR_END = 16,
};

union Block{
    int Int;
    char * Str;
};

struct stack{
    //作为参数存储
    enum Type type;
    union Block key;
    union Block value;
    //作为操作符存储
    char * operator;
    struct stack * params;
    struct stack * params_block;
    //作为字符串识别
    char c;
    //通用
    int length;
    struct stack * next;
};
typedef struct stack * pStack;
/*---------- BEGIN OF STACK OPERATORS ---------------*/
pStack StackInit(enum Type type){
    pStack tmp = (pStack)malloc(sizeof(struct stack));
    tmp->next = NULL;
    tmp->length = 0;
    tmp->type = type;
    return tmp;
}

void StackPushBack(pStack p, struct stack s){
    pStack tmp = (pStack)malloc(sizeof(struct stack));
    tmp->c = s.c;
    tmp->operator = s.operator;
    tmp->key = s.key;
    tmp->value = s.value;
    tmp->params = s.params;
    tmp->params_block = s.params_block;
    tmp->next = p->next;
    tmp->type = s.type;
    p->next = tmp;
    p->length ++;
}

pStack StackBack(pStack p){
    return p->next;
}

void StackPop(pStack p){
    pStack tmp = p->next;
    p->next = tmp->next;
    free(tmp);
    p->length --;
}

void StackClear(pStack p){
    while(p->length){
        StackPop(p);
    }
}
/*---------- BEGIN OF HELPER FUNCTION ---------------*/
char * stack2str(pStack p){
    char * str = (char *)malloc(sizeof(char) * (p->length+1));
    str[p->length] = 0;
    while(p->length){
        str[p->length - 1] = StackBack(p)->c;
        StackPop(p);
    }
    return str;
}
struct stack newStack(){
    struct stack s;
    s.next = NULL;
    s.length = 0;
    s.params = NULL;
    s.params_block = NULL;
    s.operator = NULL;
    return s;
}

/*---------- END OF HELPER FUNCTION ---------------*/

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
    unsigned int counter=0;
    int sum=0;//Int
    char c;
    if(!infp){
        printf("[E] 无法打开输入文件\n");
        return 0;
    }
    if(!outfp){
        printf("[E] 无法打开输出文件\n");
        return 0;
    }
    struct stack charSaver,paramSaver,paramGroupSaver,operatorSaver;
    enum Status status=WAIT_OPERATOR;
    pStack operator = StackInit(OPERATOR);//操作符栈，存储所有操作符
    pStack str = StackInit(CHAR);//字符串暂存栈
    while(!feof(infp)){
        counter ++;
        c = fgetc(infp);
        //putchar(c);
        if(c == -1 || c=='\n' || c=='\r'){
            continue;
        }
        if(status == WAIT_OPERATOR){
            if(c == ' ') continue;
            if(('a'<=c && c<='z') || ('A'<=c && c<='Z') || c=='_'){
                charSaver = newStack();
                charSaver.c = c;
                StackPushBack(str, charSaver);
                status = WAIT_OPERATOR_END;
                continue;
            }else{
                printf("[E] 在%u(%c)位置解析异常.操作符应该以下划线或字母开头\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_OPERATOR_END){
            if(c == ':' || c == ' '){
                status = (c == ':')?WAIT_L_BRACKET:WAIT_COLON;
                operatorSaver = newStack();
                operatorSaver.operator = stack2str(str);
                operatorSaver.params_block = StackInit(PARAM_BLOCK);
                paramGroupSaver = newStack();
                paramGroupSaver.params = StackInit(PARAM);
                StackPushBack(operator, operatorSaver);
                continue;
            }
            if(('a'<=c && c<='z') || ('A'<=c && c<='Z') || c=='_' || ('0' <= c && c<='9')){
                charSaver = newStack();
                charSaver.c = c;
                StackPushBack(str, charSaver);
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.操作符只应包含下划线,字母或数字\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_COLON){
            if(c == ' '){
                continue;
            }
            if(c == ':'){
                status = WAIT_L_BRACKET;
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.未找到操作符后的':'\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_L_BRACKET){
            if(c == ' '){
                continue;
            }
            if(c == '{'){
                status = WAIT_PARAM;
                paramSaver = newStack();
                paramSaver.params = StackInit(PARAM);
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.未找到操作符后的'{'\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_PARAM){
            if(c == ' '){
                continue;
            }
            if(c == '"'){
                status = WAIT_D_FIRST;
                continue;
            }
            if(c == '\''){
                status = WAIT_S_FIRST;
                continue;
            }
            if('0'<=c && c<='9'){
                sum = c - '0';
                status = WAIT_INT;
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.参数不合法\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_D_FIRST || status == WAIT_S_FIRST){
            paramSaver.type = DICT_STR_INT;
            if(('a'<=c && c<='z') || ('A'<=c && c<='Z') || c=='_'){
                charSaver = newStack();
                charSaver.c = c;
                StackPushBack(str, charSaver);
                status = (status == WAIT_D_FIRST)?WAIT_D_QUOTATION:WAIT_S_QUOTATION;
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.参数名不合法\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_D_QUOTATION || status == WAIT_S_QUOTATION){
            if(c == '"' && status == WAIT_D_QUOTATION){
                paramSaver.key.Str = stack2str(str);
                status = WAIT_EQUAL;
                continue;
            }
            if(c == '\'' && status == WAIT_S_QUOTATION){
                paramSaver.key.Str = stack2str(str);
                status = WAIT_EQUAL;
                continue;
            }
            if(('a'<=c && c<='z') || ('A'<=c && c<='Z') || ('0' <= c && c<='9') || c=='_'){
                charSaver.c = c;
                StackPushBack(str, charSaver);
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.参数名包含非法字符\n",counter,c);
                return 0;
            }
        }
//        if(status == WAIT_S_QUOTATION){
//            if(c == '\''){
//                paramSaver.key.Str = stack2str(str);
//                status = WAIT_EQUAL;
//                continue;
//            }
//            if(('a'<=c && c<='z') || ('A'<=c && c<='Z') || ('0' <= c && c<='9') || c=='_'){
//                charSaver.c = c;
//                StackPushBack(str, charSaver);
//                continue;
//            }
//            else{
//                printf("[E] 在%u(%c)位置解析异常.参数名包含非法字符\n",counter,c);
//                return 0;
//            }
//        }
        if(status == WAIT_INT){
            paramSaver.type = DICT_INT_INT;
            if('0'<=c && c<='9'){
                sum = sum * 10 + c - '0';
                continue;
            }
            if(c == ' '){
                status = WAIT_EQUAL;
                paramSaver.key.Int = sum;
                continue;
            }
            if(c == '='){
                status = WAIT_VALUE;
                paramSaver.key.Int = sum;
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.参数名包含非法字符\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_EQUAL){
            if(c == ' '){
                continue;
            }
            if(c == '='){
                status = WAIT_VALUE;
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.未找到连接参数名与参数值的等号\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_VALUE){
            if(c == ' '){
                continue;
            }
            if(c == '\''){
                status = WAIT_S_QUOTATION_V;
                paramSaver.type = (paramSaver.type == DICT_INT_INT)?DICT_INT_STR:DICT_STR_STR;
                continue;
            }
            if(c == '"'){
                status = WAIT_D_QUOTATION_V;
                paramSaver.type = (paramSaver.type == DICT_INT_INT)?DICT_INT_STR:DICT_STR_STR;
                continue;
            }
            if('0'<=c && c<='9'){
                status = WAIT_INT_V;
                paramSaver.type = (paramSaver.type == DICT_INT_INT)?DICT_INT_INT:DICT_STR_INT;
                sum = c-'0';
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.参数值非法\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_S_QUOTATION_V || status == WAIT_D_QUOTATION_V){
            if((status == WAIT_S_QUOTATION_V && c == '\'') || (status == WAIT_D_QUOTATION_V && c == '"')){
                paramSaver.value.Str = stack2str(str);
                //StackPushBack(StackBack(StackBack(operator)->params_block)->params, tmp);
                status = WAIT_PARAM_END;
                continue;
            }
            else{
                charSaver.c = c;
                StackPushBack(str, charSaver);
            }
        }
        if(status == WAIT_INT_V){
            if('0'<=c && c<='9'){
                status = WAIT_INT_V;
                sum = sum * 10 + c - '0';
                continue;
            }
            if(c == ' '){
                status = WAIT_PARAM_END;
                paramSaver.value.Int = sum;
                continue;
            }
            if(c == ','){
                status = WAIT_PARAM;
                paramSaver.value.Int = sum;
                StackPushBack(paramGroupSaver.params, paramSaver);
                continue;
            }
            if(c == '}'){
                status = WAIT_NEXT_OR_END;
                paramSaver.value.Int = sum;
                StackPushBack(paramGroupSaver.params, paramSaver);
                StackPushBack(StackBack(operator)->params_block, paramGroupSaver);
                paramGroupSaver = newStack();
                paramGroupSaver.params = StackInit(PARAM);
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.参数值非法\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_PARAM_END){
            if(c == ' '){
                continue;
            }
            if(c == ','){
                status = WAIT_PARAM;
                StackPushBack(paramGroupSaver.params, paramSaver);
                continue;
            }
            if(c == '}'){
                status = WAIT_NEXT_OR_END;
                StackPushBack(paramGroupSaver.params, paramSaver);
                StackPushBack(StackBack(operator)->params_block, paramGroupSaver);
                paramGroupSaver = newStack();
                paramGroupSaver.params = StackInit(PARAM);
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.未找到参数分隔符','或参数块结束符'}'\n",counter,c);
                return 0;
            }
        }
        if(status == WAIT_NEXT_OR_END){
            if(c == ' '){
                continue;
            }
            if(c == ','){
                status = WAIT_L_BRACKET;
                continue;
            }
            if(c == ';'){
                status = WAIT_OPERATOR;
                continue;
            }
            else{
                printf("[E] 在%u(%c)位置解析异常.未找到参数块分隔符','或操作符分隔符';'\n",counter,c);
                return 0;
            }
        }
    }
    if(status !=WAIT_OPERATOR && status != WAIT_NEXT_OR_END){
        switch(status){
            case WAIT_OPERATOR_END:
            case WAIT_COLON:
                printf("[E] 文件已达末端.未找到连接操作符与代码块间的':'\n");
                break;
            case WAIT_L_BRACKET:
                printf("[E] 文件已达末端.未找到参数块起始符'{'\n");
                break;
            case WAIT_PARAM:
                printf("[E] 文件已达末端.未找到参数名\n");
                break;
            case WAIT_S_FIRST:
            case WAIT_D_FIRST:
                printf("[E] 文件已达末端.未找到参数名\n");
                break;
            case WAIT_S_QUOTATION:
            case WAIT_D_QUOTATION:
                printf("[E] 文件已达末端.参数名引号未闭合\n");
                break;
            case WAIT_INT:
            case WAIT_EQUAL:
                printf("[E] 文件已达末端.未找到参数名与参数值间的等号\n");
                break;
            case WAIT_VALUE:
                printf("[E] 文件已达末端.未找到参数值\n");
                break;
            case WAIT_D_QUOTATION_V:
            case WAIT_S_QUOTATION_V:
                printf("[E] 文件已达末端.参数值引号未闭合\n");
                break;
            case WAIT_INT_V:
            case WAIT_PARAM_END:
                printf("[E] 文件已达末端.未找到参数块结束符'}'\n");
                break;
            default:
                break;
        }
        return 0;
    }
    
    printf("Operator count: %d \n",operator->length);
    fprintf(outfp, "Operator count: %d \n",operator->length);
    int count = 1;
    while(operator->length){
        pStack pOperator = StackBack(operator);
        printf("Operator No.%d : %s\n",count, pOperator->operator);
        fprintf(outfp,"Operator No.%d : %s\n",count, pOperator->operator);
        pStack pParamGroup = pOperator->params_block;
        int countpb = 1;
        while(pParamGroup->length){
            printf("\tParams Block %d\n",countpb);
            fprintf(outfp,"\tParams Block %d\n",countpb);
            pStack pParams = StackBack(pParamGroup)->params;
            countpb++;
            int countp = 1;
            while(pParams->length){
                printf("\t\t Params %d ",countp);
                pStack pParamsV = StackBack(pParams);
                switch(pParamsV->type){
                    case DICT_INT_INT:
                        printf("(INT:INT) %d: %d",pParamsV->key.Int,pParamsV->value.Int);
                        fprintf(outfp,"(INT:INT) %d: %d",pParamsV->key.Int,pParamsV->value.Int);
                        break;
                    case DICT_INT_STR:
                        printf("(INT:STR) %d: %s",pParamsV->key.Int,pParamsV->value.Str);
                        fprintf(outfp,"(INT:STR) %d: %s",pParamsV->key.Int,pParamsV->value.Str);
                        break;
                    case DICT_STR_INT:
                        printf("(STR:INT) %s: %d",pParamsV->key.Str,pParamsV->value.Int);
                        fprintf(outfp,"(STR:INT) %s: %d",pParamsV->key.Str,pParamsV->value.Int);
                        break;
                    case DICT_STR_STR:
                        printf("(STR:STR) %s: %s",pParamsV->key.Str,pParamsV->value.Str);
                        fprintf(outfp,"(STR:STR) %s: %s",pParamsV->key.Str,pParamsV->value.Str);
                        break;
                    default:
                        break;
                }
                StackPop(pParams);
                putchar('\n');
                fputc('\n', outfp);
                countp++;
            }
            StackPop(pParamGroup);
        }
        count++;
        StackPop(operator);
    }
    return 0;
}
