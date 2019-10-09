# 约瑟夫环实验报告
顾超 PB18030825
## 题目描述
* 约瑟夫问题的一种描述：编号为1～n的n个人围成一圈，每人持有一个密码（正整数），一开始任选一正整数作为报数上限值m，从第一个人按顺时针方向自1开始报数，报到m时停止报数，报m的人出列，将其密码作为新m值，下一个人重新从1开始报数，直到所有人出列为止。
* 编写一个程序，该程序根据输入的命令行参数创建一个单循环链表表示的约瑟夫环，然后输出约瑟夫环出列的顺序。

## 题目要求
1. 创建一个单循环链表表示的约瑟夫环，然后输出约瑟夫环出列的顺序。
2. 程序有对命令行参数不全或不正确的处理(如提示输入、报错等).
3. 将约瑟夫环用顺序表实现。

## 实现方式
分别采用循环链表（不删除节点），循环链表（删除节点），线性表三种方式进行实现。
### 1.循环链表（不删除节点）
根据输入创建一个循环单向链表，链表中包含ID（输出时使用），Secret（作为下一个m的值）以及flag（标记节点是否已被踢出）。指针p在循环链表中移动，当节点flag为0时计数器加1，直至计数器等于m时将该节点flag设为1，m设为该节点的secret，计数器重置，直至所有节点被踢出。
### 2.循环链表（删除节点）
实现类似1，但不包含flag项，当踢出节点时直接将该节点从链表中移除，其余操作与1相同。
### 3.线性表
创建两线性表，分别作为踢出标记与secret的存储，i在0～n间循环，遇到未被踢出的节点时计数器加一，当计数器等于m时踢出节点（将提出标记设为1），根据secret设置m，重置计数器，后继续循环直至踢出所有节点。


## 程序逻辑

```mermaid
graph TD
开始 --> 判断参数个数
判断参数个数 --大于3个--> 从argv获取m,n
判断参数个数 --小于等于3个--> 错误提示
从argv获取m,n --失败--> 错误提示
错误提示 --> 结束
从argv获取m,n --> 根据n判断参数个数
根据n判断参数个数 --"小于n+3"--> 错误提示
根据n判断参数个数 --"等于n+3"--> 从argv获取secret
根据n判断参数个数 --"大于n+3"--> 警告提示
警告提示 --> 从argv获取secret
从argv获取secret --> p指向head节点
p指向head节点 --> p=p-next
p=p-next --> p是否已经被踢出?
p是否已经被踢出? --是--> p=p-next
p是否已经被踢出? --否--> 计数器++
计数器++ --> 计数器是否等于m
计数器是否等于m --否--> p=p-next
计数器是否等于m --是--> n-=1,m=secret,重置计数器,踢出节点
n-=1,m=secret,重置计数器,踢出节点 --> n是否为0?
n是否为0? --是--> 结束
n是否为0? --否--> p=p-next
```

## 函数解释
字符串转整形，默认输入均为正整数，若遇到非数字字符直接返回-1，否则返回数值。

```c
int str2int(const char * str);
```
初始化链表，返回链表头节点

```c
pnode CreateNode();
```
向链表插入新节点
 
```c
void Insert(pnode head, int password);
```
链表读入部分代码
```c
pnode Head = CreateNode();
for(int i=3;i<n+3;i++){
    int t = str2int(argv[i]);
    if(t<=0){
        printf("[E] 参数错误:参数输入错误.第%d个：%s\n",i-1,argv[i]);
        return 0;
    }
    Insert(Head, t);
}
```

不删除节点的循环链表
```c
pnode p = Head->next;
while(n){
    if(p->flag){//遇到删除标记直接跳过
        p = p->next;
        continue;
    }
    count++;
    if(count == m){
        p->flag = 1;//设置删除标记
        m = p->password;
        count=0;
        printf("%d,",p->id);
        fprintf(fp, "%d ",p->id);
        n--;
    }
    p = p->next;
}
```
删除节点的循环链表
```c
pnode p1 = Head, p2 = Head->next;
while(n){
    count++;
    if(count == m){
        m = p2->password;
        count = 0;
        n--;
        printf("%d,",p2->id);
        fprintf(fp, "%d ",p2->id);
        //将节点删除
        p1->next = p2->next;
        free(p2);
        p2 = p1->next;
    }else{
        p1 = p2;
        p2 = p2->next;
    }
}
```
顺序表存储方案
```c
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
            //大于n直接归0实现循环效果
            if(i>=n){
                i=0;
            }
            continue;
        }
        count++;
        if(count == m){
            count = 0;
            m = secret[i];
            //设置踢出标记
            flag[i]=1;
            n1--;
            printf("%d ",i+1);
            fprintf(fp, "%d ",i+1);
        }
        i++;
        //大于n直接归0实现循环效果
        if(i>=n){
            i=0;
        }
    }
```
## 宏解释
程序共定义了三个宏，分别对应三种实现方式。
```c
#define LINK_NODE_NO_DELETE
//#define LINK_NODE_WITH_DELETE
//#define ADT_LIST
```

## 错误处理
当输入有误时程序将发出错误提示并停止运行。
* 错误样例

|错误原因|输入|输出|
|---|---|---|
|参数不足（m与n）|bash-3.2$ ./app|[E] 参数错误:参数不足，期望：2 实际：0|
|参数不足（password)|bash-3.2$ ./app 10 10 2|[E] 参数错误:参数不足，期望：12 实际：3|
|参数过多|bash-3.2$ ./app 2 2 1 2 3 4 5|[W] 参数过多，多余参数将被舍弃|
|参数错误|bash-3.2$ ./app 2 -2 1 2 3 4|[E] 参数错误:参数输入错误，m与n应为正整数|
|内存分配失败|/|[E] 溢出错误:malloc分配内存失败。|
|文件写失败|/|[E] IO错误:无法写入到文件out.txt|


## 结果演示
![](/Users/guch8017/Documents/GitHub/DataStructureHW/约瑟夫环/img.png)
测试样例：对于三种模式均有
```bash
bash-3.2$ ./app 20 7 3 1 7 2 4 8 4
6 1 4 7 2 3 5
```
同时包含结果的文件out.txt被输出到运行目录下。与书本中提供的结果相符。
