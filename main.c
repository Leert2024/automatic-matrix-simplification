#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "simple.h"
#include "transformation.h"

#define COL matrixAddr->col
#define ROW matrixAddr->row

/**将每行按照主元列标进行排序*/
void bubble_sort(Matrix* matrixAddr, int* firstNotZero){
    for(int j=0;j<ROW;j++){
        for(int i=ROW-1;i>j;i--){
            if(firstNotZero[i]<firstNotZero[i-1]){
                swi(matrixAddr,i-1,i);
                int temp = firstNotZero[i];
                firstNotZero[i] = firstNotZero[i-1];
                firstNotZero[i-1] = temp;
            }
        }
    }
}

/**将所有行按主元列标非严格递增排列*/
void allSwi(Matrix* matrixAddr){
    int firstNotZero[ROW];//存储每行先导0的个数

    for(int i=0;i<ROW;i++){
        firstNotZero[i] = COL-1;
        for(int j=0;j<COL;j++){
            if(*element(*matrixAddr,i,j)){
                firstNotZero[i] = j;
                break;
            }
        }
    }

    bubble_sort(matrixAddr,firstNotZero);

    printf("将所有行按主元列标非严格递增排列：\n");
    pri(*matrixAddr);
}

/**检测矩阵是否仍未被化为阶梯形（即是否需要继续化简）*/
int* ifNotOkey(Matrix* matrixAddr){
    int prev = COL;

    for(int i=ROW-1;i>-1;i--){//从下往上，比较相邻行主元列标（全为0则主元列标为COL）
        int recent = COL;
        for(int j=0;j<COL;j++){
            if(*element(*matrixAddr,i,j)){
                recent = j;
                break;
            }
        }
        if(recent==prev && recent < COL){//发现两行主元列标相等
            int* result = malloc(2*sizeof(int));
            result[0] = i+1;
            result[1] = recent;
            return result;
        }
        prev = recent;
    }

    int* result = malloc(2*sizeof(int));
    result[0] = 0;
    result[1] = COL;
    return result;
}

/**将矩阵每一行变为互质组合*/
void divide(Matrix* matrixAddr){
    for(int row = 0;row<ROW;row++){

        //找到当前行第一个不为零的数，获取其下标index与值times
        int index, times = 1;
        for(int i = 0;i < COL;i++){
            if(*element(*matrixAddr,row,i)){
                index = i;
                times = *element(*matrixAddr,row,i);
                break;
            }
        }

        if(index >= COL)continue;//如果该行为空（全为0），结束

        //计算该行所有不为0的数的最大公因数
        for(int i = index+1;i < COL;i++){
            if(*element(*matrixAddr,row,i))times = gcd(times, *element(*matrixAddr,row,i));
        }

        for(int i=0;i<COL;i++){
            *element(*matrixAddr,row,i) /= times;
        }
        if(times!=1)printf("第%d行所有数除以%d\n",row+1,times);
    }
    pri(*matrixAddr);
}

/**将下标为aim的行的列标最小的非零元用其前一行消去*/
void cancelFisrt(Matrix* matrixAddr, int aim, int firstNotZero){
    if(aim<=0 || firstNotZero >= COL)return;
    int gcdOfAimAndBefore = gcd(*element(*matrixAddr,aim-1,firstNotZero),*element(*matrixAddr,aim,firstNotZero));
    int times1 = *element(*matrixAddr,aim-1,firstNotZero)/gcdOfAimAndBefore;
    int times2 = *element(*matrixAddr,aim,firstNotZero)/gcdOfAimAndBefore;
    mulsub(matrixAddr,aim,times1,aim-1,times2);
}

/**获知输入矩阵的大小*/
void getInputSize(Matrix* matrixAddr){
    FILE* fp = fopen("input.txt","r");
    if(!fp){
        printf("找不到文件input.txt，请新建文件并重试！\n");
        system("pause");
        exit(1);
    }

    int rowSize = 1, prevColSize = 0;

    int num;
    char ch = 32;
    int out = 0;

    while(!out){//行级循环
        int colSize = 0;
        while(1){//每一行内的循环
            ch = fgetc(fp);

            if(ch==EOF){
                if(!colSize)rowSize--;
                out = 1;
                break;
            }else if(ch==10){//如果是换行符
                if(colSize != prevColSize && prevColSize && colSize){
                    printf("input.txt中矩阵第%d行元素数（%d）与第%d行元素数（%d）不等！\n",rowSize-1,prevColSize,rowSize,colSize);
                    fclose(fp);
                    system("pause");
                    exit(1);
                }
                if(colSize)prevColSize = colSize;
                rowSize++;
                if(!colSize)rowSize--;
                break;//跳出本行循环进入下一行
            }else if(ch==32 || ch=='\t')continue;//如果是空格或制表符则忽略
            else if(ch == '-' || ch == '+' || ch <= '9' && ch >= '0'){//是数字
                fseek(fp,-1L,SEEK_CUR);
                fscanf(fp,"%d",&num);
                colSize++;
            }else{//非法字符
                printf("文件input.txt含有非法的字符（ascii码为%d）或格式错误，请检查文件！\n",ch);
                fclose(fp);
                system("pause");
                exit(1);
            }
        }
    }
    fclose(fp);

    if(rowSize<2 || prevColSize<2){
        printf("矩阵太小（%d行%d列），程序拒绝化简！\n",rowSize,prevColSize);
        system("pause");
        exit(1);
    }

    COL = prevColSize;
    ROW = rowSize;
}

/**输入*/
void inputMatrix(Matrix* matrixAddr){
    FILE* fp = fopen("input.txt","r");

    for(int i = 0;i<ROW;i++){
        for(int j = 0;j<COL;j++){
            fscanf(fp,"%d",element(*matrixAddr,i,j));
        }
    }

    fclose(fp);
}

int main(){
    //创建矩阵
    Matrix matrix;

    getInputSize(&matrix);
    printf("程序从input.txt检测到了%d行%d列的矩阵：\n",matrix.row,matrix.col);

    matrix.address = (int*)malloc(matrix.row*matrix.col*sizeof(int));

    inputMatrix(&matrix);

    //输出未处理的矩阵
    pri(matrix);

    //将所有行按主元列标非严格递增排列
    allSwi(&matrix);

    while(1){
        int* temp = ifNotOkey(&matrix);
        if(!temp[0])break;//矩阵已化为阶梯型，结束循环
        cancelFisrt(&matrix,temp[0],temp[1]);
    }

    //每行化为互质组合
    divide(&matrix);

    //输出结果
    printf("化简的最终结果为：\n");
    pri(matrix);

    //程序结束
	system("pause");
	return 0;
}
