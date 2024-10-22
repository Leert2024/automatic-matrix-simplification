#include <stdio.h>
#include <windows.h>
#include "simple.h"
#include "transformation.h"

#define COL matrixAddr->col
#define ROW matrixAddr->row

void setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

//根据元素行、列下标返回元素地址
int* element(Matrix matrix, int row, int col){
    return matrix.address + matrix.col * row + col;
}

/**打印矩阵*/
void pri(const Matrix matrix){
    for(int i=0;i<matrix.row;i++){
        for(int j=0;j<matrix.col;j++){
            if(*element(matrix,i,j)){
                setColor(0x0f);
            }else{
                setColor(0x08);
            }
            printf("%d\t",*element(matrix,i,j));
        }
        printf("\n");
    }
    printf("\n");
    setColor(0x07);
}

/**交换位置*/
void swi(Matrix* matrixAddr, int row1, int row2){
    if(max_num(row1,row2)>ROW)return;
    for(int i=0;i<COL;i++){
        int temp = *element(*matrixAddr,row1,i);
        *element(*matrixAddr,row1,i) = *element(*matrixAddr,row2,i);
        *element(*matrixAddr,row2,i) = temp;
    }
    printf("交换了第%d行与第%d行\n",row1+1,row2+1);
}

/**把第row1行每个数乘times1，再加上第row2行的times2倍*/
//（只改变第row1行，第row2行不变）
void muladd(Matrix* matrixAddr,int row1,int times1,int row2,int times2){
    for(int i=0;i<COL;i++){
        *element(*matrixAddr,row1,i) *= times1;
        *element(*matrixAddr,row1,i) += times2 * (*element(*matrixAddr,row2,i));
    }
}

/**把第row1行每个数乘times1，再减去第row2行的times2倍*/
void mulsub(Matrix* matrixAddr,int row1,int times1,int row2,int times2){
    muladd(matrixAddr,row1,times1,row2,-times2);
    printf("第%d行乘%d，减去%d倍的第%d行\n",row1+1,times1,times2,row2+1);
    pri(*matrixAddr);
}
