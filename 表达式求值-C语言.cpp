#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#define INITSIZE 100
#define cls system("cls")
#define pause system("pause")

//定义表达式结构体
typedef struct{
    char a[1024];
    int result;
}exp;

//操作数栈
typedef int ElemType;
typedef struct{
    ElemType *top;
    ElemType *base;
    int stacksize;
}numstack;

//运算符栈
typedef struct{
    char *top;
    char *base;
    int stacksize;
}signstack;

//函数声明
int Readfile(exp *bank, int i, int itemnum);
void Checkitem(int i, int allitem);
int Savescore(int *m, int num);
int Saveinput(int *m, int num);

//初始化操作数栈
void Initnum(numstack *S1){
    S1->base= (ElemType *)malloc(INITSIZE*sizeof(ElemType));
    if(!S1->base){
        printf("初始化失败initnum。\n\n");
        pause;
        exit(0);
    }
    S1->top= S1->base;
    S1->stacksize=INITSIZE;
}

//初始化运算符栈
void Initsign(signstack *S2){
    S2->base= (char *)malloc(INITSIZE*sizeof(char));
    if(!S2->base){
        printf("初始化失败initsign。\n\n");
        pause;
        exit(0);
    }
    S2->top= S2->base;
    S2->stacksize=INITSIZE;
}

//取操作数栈顶元素
ElemType Getnum(numstack *S1){
    long e;
    if(S1->top== S1->base){
        printf("栈空getnum。\n\n");
        pause;
        return 1;
    }
    e= *(S1->top-1);
    return e;
}

//取运算符栈顶元素
char Getsign(signstack *S2){
    char c;
    if(S2->top== S2->base){
        printf("栈空getsign。\n\n");
        pause;
        return 1;
    }
    c= *(S2->top-1);
    return c;
}

//操作数入栈
int Pushnum(numstack *S1, ElemType e){
    *S1->top++= e;
    return 0;
}

//运算符入栈
int Pushsign(signstack *S2, char c){
    *S2->top++= c;
    return 0;
}

//操作数出栈
int Popnum(numstack *S1){
    ElemType e;
    if(S1->top== S1->base){
        printf("栈空popnum。\n\n");
        pause;
        return 1;
    }
    e= *--S1->top;
    return e;
}

//运算符出栈
int Popsign(signstack *S2){
    char c;
     if(S2->top== S2->base){
        printf("栈空popsign。\n\n");
        pause;
        return 1;
    }
    c= *--S2->top;
    return c;
}

//判断是否是运算符
int Isoptr(char ch1){
    char ptr[10]= {'+', '-', '*', '/', '(', ')', '#'};
    for(int i=0; i<7; i++){
        if(ch1== ptr[i])
            return 1;
    }
    return 0;
}

//优先级表格
char Precede(char a, char b){
    int i, j;
    char Table[10][10]={
    {' ','+','-','*','/','(',')','#'},
    {'+','>','>','<','<','<','>','>'},
    {'-','>','>','<','<','<','>','>'},
    {'*','>','>','>','>','<','>','>'},
    {'/','>','>','>','>','<','>','>'},
    {'(','<','<','<','<','<','=','0'},
    {')','>','>','>','>','0','>','>'},
    {'#','<','<','<','<','<','0','='}};

    for(i=0; i<8; i++)
        if(Table[0][i]== a) //查找栈顶符号
            break;
    for(j=0; j<8; j++) //查找当前符号
        if(Table[j][0]== b)
            break;
    return Table[j][i];
}

//计算
int Operate(int a, char sign1, int b){
    int c;
    if(sign1== '+') c= a+b;
    else if(sign1== '-') c= a-b;
    else if(sign1== '*') c= a*b;
    else c= a/b;
    return c;
}

//求值
long Calculate(char *ch, numstack *opnd, signstack *optr){
    char sign;
    int b, d, k=0, i=0, j=0, num= 0;
    Initnum(opnd);
    Initsign(optr);
    Pushsign(optr, '#');
    while(ch[i]!= '='){
        if(!Isoptr(ch[i])){
            k++;
            if(k<= j){
                num= (int(ch[i])-48);
                i++;
            }
            if(k> j){
                num= num*10+(int(ch[i])-48);
                k= j= 0;
                i++;
            }
            if(!Isoptr(ch[i]))
                k++;
            if(k==j)
                Pushnum(opnd, num);
            }
            else if(Isoptr(ch[i])){
                switch(Precede(ch[i], Getsign(optr))){
                    case'<': Pushsign(optr, ch[i]); i++;
                        if(ch[i]!= '(' && ch[i]!= ')')
                            j++;
                        break;
                    case'=': Popsign(optr); i++; break;
                    case'>': sign= Popsign(optr);
                        d= Popnum(opnd);
                        b= Popnum(opnd);
                        Pushnum(opnd, Operate(b, sign, d));
                        break;
                }
            }
    }
    printf("该题正确结果为:");
    printf("%d\n\n", Getnum(opnd));
    return(Getnum(opnd));
}

//(1)建立试题库文件
int Creitembank(void){
    cls;
    int i, itemnum, select;
    exp *bank;
    FILE *fp;
    printf("*******************************************************\n");
    printf("*重新建立试题库请输入1,在原有试题库上增加试题请输入2。\n");
    printf("*******************************************************\n");
    fflush(stdin);
    scanf("%d", &select);
    if(select== 1)
        fp=fopen("D:\\数据结构实训\\试题库.txt","wb");
    if(select== 2)
        fp=fopen("D:\\数据结构实训\\试题库.txt","ab");
    if(fp== NULL){
        printf("还没创建试题库文件D:\\数据结构实训\\试题库.txt\n\n");
        pause;
        return 1;
    }
    printf("请输入试题的数量：\n");
    scanf("%d",&itemnum);
    fflush(stdin);
    bank=(exp *)malloc(itemnum*2*sizeof(exp));   //动态分配内存
    if(bank==NULL){
        printf("存储空间不足。\n\n");
        pause;
        return 1;
    }
    for(i=0; i<itemnum; i++){      //输入试题
        printf("请输入第 %d 道试题:",i+1);
        gets(bank[i].a);
        fputs(bank[i].a, fp);
        fputs("\n",fp);
        fflush(stdin);
        printf("\n");
    }
    fclose(fp);
    printf("成功存入了 %d 道试题到D:\\数据结构实训\\试题库.txt文件中。\n\n", itemnum);
    pause;
    cls;
    Readfile(bank, i, itemnum);
    pause;
    free(bank);  //释放动态分配的内存
    return 0;
}

//(2)查看试题库中试题
void Checkitem(void){
    cls;
    FILE *fp;
    char str[1024];
    fp=fopen("D:\\数据结构实训\\试题库.txt","rb");
    if(fp== NULL){
        printf("找不到D:\\数据结构实训\\试题库.txt\n");
        pause;
        exit(0);
    }
    printf("该试题库中的试题列表:\n\n");
    while(!feof(fp)){
        fgets(str, 1024, fp);
        printf("%s", str);
        strcpy(str, " ");
    }
    fclose(fp);
    printf("\n");
    pause;
}

//(3)练习试题
int Practice(void){
    cls;
    FILE *fp, *mp;
    exp *extract;
    char reply, ans;
    char str[1024];
    int i=0, j, n, temp, item[20], result1;
    int score, count= 0, count1= 0, *grade;
    int total, select;
    numstack opnd1,*opnd;
    signstack optr1,*optr;
    opnd= &opnd1; optr= &optr1;
    grade=(int *)malloc(512*sizeof(int));
    extract=(exp *)malloc(512*sizeof(exp));
    do{
        cls;
        printf("请输入要练习的习题数:\n");
        scanf("%d", &n);
        fp=fopen("D:\\数据结构实训\\试题库.txt", "rb");
        if(fp==NULL){
            printf("找不到D:\\数据结构实训\\试题库.txt\n");
            free(extract);
            exit(0);
        }
        while(!feof(fp) && i< n){
            fgets(extract[item[i]].a, 1024, fp);
        i++;
        }
        fclose(fp);
        score= 0;
        srand((unsigned)time(NULL));
        item[i]= rand()% n+1;
        while(i<= n){
            while(1){
                temp = rand()% n+1;
                for(j=0; j<i; j++){
                    if(item[j]== temp)
                        break;
                }
                if(j== i){
                    item[i]=temp;
                break;
                }
            }
            i++;
        }
        cls;
        printf("**************************************\n");
        printf("*随机抽取了 %d 道练习题,请开始答题:   \n", n);
        printf("**************************************\n");
        pause;
        fflush(stdin);
        for(i= 0; i< n; i++){
            cls;
            count++;
            printf("第 %d 道练习题:\n\n",i+1);
            puts(extract[item[i]].a);
            printf("请输入你的答案:\n");
            fflush(stdin);
            scanf("%d", &extract[item[i]].result);
            result1= extract[item[i]].result;
            printf("要保存输入的答案吗?(Y/N)\n");
            fflush(stdin);
            ans= getchar();
            if(ans!= 'N' && ans!= 'n')
                Saveinput(&result1, count);
            if(extract[item[i]].result== Calculate(extract[item[i]].a, opnd, optr)){
                score+=10;
                printf("回答正确。\n\n");
            }
            else{
                printf("回答错误。\n\n");
            }
            pause;
        }
        grade[count1]= score;
        count1++;       //记录做题次数
        cls;
        printf(">>>>>得分情况<<<<<\n\n");
        printf(">>>>>最后的得分为:%d。\n\n", score);
        total= n*10;
        if(score*1.0>= total*0.9)
            printf("成绩很棒。\n\n");
        else if(score*1.0>= total*0.8)
            printf("成绩还好。\n\n");
        else if(score*1.0>= total*0.7)
            printf("成绩一般。\n\n");
        else if(score*1.0>= total*0.6)
            printf("成绩及格。\n\n");
        else
            printf("成绩未及格。要噢力给。\n\n");
        printf("要保存得分吗?(Y/N)\n");
        fflush(stdin);
        ans= getchar();
        if(ans!= 'N' && ans!= 'n')
            Savescore(grade, count1);
        printf("继续练习吗?(Y/N)\n");
        fflush(stdin);
        reply= getchar();
        if(count1>= 20)    //超过最大记录次数清0
            count1= 0;
        }while(reply!='N' && reply!='n');
        free(grade); free(extract);
    pause;
    return count1;
}

//保存用户输入的答案
int Saveinput(int *m, int num){
    FILE *mp;
    int select;
    printf("*******************************************************\n");
    printf("*重新保存输入的答案请输入1,在原有上添加请输入2。\n");
    printf("*******************************************************\n");
    fflush(stdin);
    scanf("%d", &select);
    if(select== 1)
        mp= fopen("D:\\数据结构实训\\输入的答案.txt", "wb");
    else if(select== 2)
        mp= fopen("D:\\数据结构实训\\输入的答案.txt", "ab");
    else if(mp== NULL){
        printf("找不到得分记录文件D:\\数据结构实训\\输入的答案.txt\n\n");
        pause;
        return 1;
    }
    else{
        printf("输入有误Saveinput。\n\n");
        pause;
        return 1;
    }
    for(int i= 0; i< num; i++){
        fprintf(mp, "%d\n", m[i]);
    }
    fclose(mp);
    printf("答案保存成功。\n\n");
    pause;
    printf("\n");
    return 0;
}

//保存得分记录
int Savescore(int *m, int num){
    FILE *mp;
    int select, i;
    printf("*******************************************************\n");
    printf("*重新保存得分记录请输入1,在原有得分记录添加请输入2。 \n");
    printf("*******************************************************\n");
    fflush(stdin);
    scanf("%d", &select);
    if(select== 1)
        mp= fopen("D:\\数据结构实训\\得分记录.txt", "wb");
    else if(select== 2)
        mp= fopen("D:\\数据结构实训\\得分记录.txt", "ab");
    else if(mp== NULL){
        printf("找不到得分记录文件D:\\数据结构实训\\得分记录.txt\n\n");
        pause;
        return 1;
    }
    else{
        printf("输入有误Savescore。\n\n");
        pause;
        return 1;
    }
    for(i= 0; i< num; i++){
        fprintf(mp, "%d\n", m[i]);
    }
    fclose(mp);
    printf("得分保存成功。\n\n");
    pause;
    printf("\n");
    return 0;
}

//(4)查看用户输入的答案
int Checkinput(int num){
    cls;
    char m[50];
    printf("之前输入的答案:\n\n");
    FILE *mp;
    mp=fopen("D:\\数据结构实训\\输入的答案.txt","r");
     if(mp==NULL){
        printf("找不到输入的答案文件D:\\数据结构实训\\输入的答案.txt\n\n");
        pause;
        return 1;
    }
    for(int i= 0; i< num; i++){
        fgets(&m[i], num, mp);
        printf("*第 %d 次输入的答案:\n",i+1);
        printf("%d\n\n", m[i]);
    }
    fclose(mp);
    pause;
    return 0;
}

//(5)查看得分记录
int Checkscore(int num){
    cls;
    FILE *mp;
    char m[50];
    mp=fopen("D:\\数据结构实训\\得分记录.txt","r");
    if(mp==NULL){
        printf("找不到D:\\数据结构实训\\得分记录.txt\n\n");
        pause;
        return 1;
    }
    printf(">>>>>得分情况<<<<<：\n\n");
    fgets(m, num, mp);
    for(int i= 0; i< num; i++){
        printf("第 %d 次得分:\n",i+1);
        printf("%d\n\n", m[i]);
    }
    fclose(mp);
    pause;
    return 0;
}

//(6)依照得分给出评价
void Evaluate(int count1){
    cls;
    int *grade;
    int i, total;
    total= count1*10;
    grade=(int *)malloc(sizeof(int));
    Checkscore(count1);
    printf("\n");
    cls;
    printf(">>>>>小小评价<<<<<\n\n");
    for(i=0; i< count1; i++)
        printf("第 %d 次得分 %d。\n", i+1, grade[i]);
    if(i> 1){
        if(grade[i-1]> total*0.6 || grade[i-2]> total*0.6){
            if(grade[i-1]> grade[i-2])
                printf("\n有进步，也要噢力给。\n\n");
            else if(grade[i-1]== grade[i-2])
                printf("\n跟上次差不多，要争取进步。噢力给。\n\n");
            else
                printf("\n退步了阿，赶快补回来。噢力给。\n\n");
        }
        else
            printf("\n阿这，退后了很多。噢力给。\n\n");
    }
    else{
        if(grade[0]*1.0>= total*0.9)
            printf("成绩很棒。\n\n");
        else if(grade[0]*1.0>= total*0.8)
            printf("成绩还好。\n\n");
        else if(grade[0]*1.0>= total*0.7)
            printf("成绩一般。\n\n");
        else if(grade[0]*1.0>= total*0.6)
            printf("成绩及格。\n\n");
        else
            printf("成绩未及格。要噢力给。\n\n");
    }
    free(grade);
    pause;
}

//读取试题库中试题
int Readfile(exp *bank, int i, int itemnum){
    FILE *fp;
    fp=fopen("D:\\数据结构实训\\试题库.txt","rb");
    if(fp==NULL){
        printf("找不到D:\\数据结构实训\\试题库.txt\n\n");
        pause;
        exit(0);
    }
    fgets(bank[i].a, itemnum, fp);
    printf("刚刚建立的试题列表：\n\n");
    for(i=0; i<itemnum;i++){     //输出试题
        printf("*第 %d 道试题:",i+1);
        printf("%s",bank[i].a);
        printf("\n\n");
    }
    fclose(fp);
    return 0;
}

void Menu(){
    system("cls");
	printf("****************************************************************\n");
	printf("           表达式求值程序，并能给出分数，可供小学生作业练习   \n");
	printf("****************************************************************\n\n");
	printf("           1.写入试题。\n\n");
	printf("           2.查看试题。\n\n");
	printf("           3.练习试题。\n\n");
	printf("           4.查看输入答案。\n\n");
	printf("           5.查看得分情况。\n\n");
	printf("           6.评价得分情况。\n\n");
	printf("           7.退出程序。\n\n");
	printf("****************************************************************\n");
}

int main(){
    int m, select, ret, num;
    do{
        Menu();
        printf("请选择并输入序号进行相应操作:\n");
        ret=scanf("%d", &select);
        switch(select){
            case 1: Creitembank(); break;
            case 2: Checkitem(); break;
            case 3: m=Practice(); break;
            case 4: cls; printf("输入要查看的记录个数(答案):\n"); scanf("%d", &num); Checkinput(num); break;
            case 5: cls; printf("输入要查看的记录个数(分数):\n"); scanf("%d", &num); Checkscore(num); break;
            case 6: cls; printf("输入要查看的记录个数(评价):\n"); scanf("%d", &num); Evaluate(num); break;
            case 7: cls; printf("*程序结束。*\n"); exit(0);
            default:
                printf("*************************************************\n");
                printf("* *输入有误，请正确选择序号（1-7）进行相应操作* *。\n");
                printf("*************************************************\n");
                pause;
                break;
        }
    }while(ret==1);
    return 0;
}
