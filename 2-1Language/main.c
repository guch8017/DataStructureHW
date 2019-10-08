//
//  main.c
//  2-2Language
//
//  Created by guch8017 on 2019/9/29.
//  Copyright © 2019 guch8017. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "operators.h"

char * table[26] = {NULL};//查询表
char buff[1001];

void GetTable(const char * filePath){
    
    char character;
    int num;
    FILE * RpTable = fopen(filePath, "r");
    if(!RpTable){
        printf("Unable to open Table file %s",filePath);
        exit(0);
    }
    fscanf(RpTable, "%d\n", &num);
    if(num <= 0 || num > 26){
        printf("Syntax error\n");
        exit(0);
    }
    for(int i=0;i<num && !feof(RpTable);i++){
        fscanf(RpTable,"%c%s\n",&character,buff);
        if(character<'A' || character > 'Z'){
            printf("Index out of range\n");
            exit(0);
        }
        if (table[character-'A']){
            printf("Redefinition of %c",character);
            exit(0);
        }
        table[character-'A'] = (char *)malloc(sizeof(char) * strlen(buff));
        strcpy(table[character-'A'], buff);
    }
    fclose(RpTable);
}


int main(int argc, const char * argv[]) {
    int param[3] = {0};
    for(int i=1;i<argc;i+=2){
        if(!strcmp(argv[i], "-t")){
            if(argc < i + 2){
                printf("Option %s requires an argument\n",argv[i]);
                return 0;
            }
            param[0] = i+1;
        }else if(!strcmp(argv[i], "-i")){
            if(argc < i + 2){
                printf("Option %s requires an argument\n",argv[i]);
                return 0;
            }
            param[1] = i+1;
        }else if(!strcmp(argv[i], "-o")){
            if(argc < i + 2){
                printf("Option %s requires an argument\n",argv[i]);
                return 0;
            }
            param[2] = i+1;
        }else if(!strcmp(argv[i], "-?")){
            printf("decode [-i input file][-o output file][-t self defined table]\n");
            printf("\t-i\t\t#Input file. If not defined will use 'in.txt'\n");
            printf("\t-o\t\t#Output file. If not defined will use 'out.txt'\n");
            printf("\t-t\t\t#Rule file. If not defined will use B->tAdA and A->sae\n");
            return 0;
        }else{
            printf("Unknown option: %s\nUse param -? to get help.\n",argv[i]);
            return 0;
        }
    }
    FILE * infp, *outfp;
    if(param[0]){
        GetTable(argv[param[0]]);
    }else{
        table[0] = "sae";
        table[1] = "tAdA";
    }
    if(param[1]){
        infp = fopen(argv[param[1]], "rb");
    }else{
        infp = fopen("in.txt", "rb");
    }
    if(!infp){
        printf("Can't open input file\n");
        return 0;
    }
    if(param[2]){
        outfp = fopen(argv[param[2]], "w");
    }else{
        outfp = fopen("out.txt", "w");
    }
    if(!outfp){
        printf("Can't open output file\n");
        return 0;
    }
    
    fseek(infp,0,SEEK_END);
    long lsize=ftell(infp);
    rewind(infp);
    char * inFile = (char *) malloc(lsize*sizeof(char)+1);
    memset(inFile,0,lsize*sizeof(char)+1);
    fread(inFile,1,lsize,infp);
    
    pStack sHead = InitStack();
    pQueueHead qHead = QueueInit();
    
    for(long i = lsize - 1; i >= 0; i--){
        StackPush(sHead, inFile[i]);
    }
    while(!StackEmpty(sHead)){
        char c = StackBack(sHead);
        StackPop(sHead);
        if('a' <= c && c <= 'z'){
            printf("%c",c);
            fprintf(outfp, "%c",c);
        }else if('A' <= c && c <= 'Z'){
            if(table[c - 'A']){
                for(int i = (int)strlen(table[c - 'A']) - 1; i>=0;i--){
                    StackPush(sHead, table[c-'A'][i]);
                }
            }else{
                printf("Table doesn't contains %c \n",c);
                return 0;
            }
        }else if(c == '('){
            while(!StackEmpty(sHead) && c != ')'){
                c = StackBack(sHead);
                StackPop(sHead);
                if(c != ')'){
                    QueuePush(qHead, c);
                }
            }
            if(c != ')'){
                printf("Co not match\n");
                return 0;
            }
            if(!QueueEmpty(qHead)){
                char firstChar = QueueEnd(qHead);
                QueuePop(qHead);
                while(!QueueEmpty(qHead)){
                    c = QueueEnd(qHead);
                    QueuePop(qHead);
                    StackPush(sHead,c);
                    StackPush(sHead,firstChar);
                }
            }
        }else{
            printf("Unrecognized Character %c(%d)\n",c,c);
            return 0;
        }
    }
    printf("\n");
    fclose(infp);
    fclose(outfp);
    return 0;
}
