#include <stdio.h>
#include <windows.h>
#include "simple.h"
#include "transformation.h"

#define COL matrixAddr->col
#define ROW matrixAddr->row

void setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

//����Ԫ���С����±귵��Ԫ�ص�ַ
int* element(Matrix matrix, int row, int col){
    return matrix.address + matrix.col * row + col;
}

/**��ӡ����*/
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

/**����λ��*/
void swi(Matrix* matrixAddr, int row1, int row2){
    if(max_num(row1,row2)>ROW)return;
    for(int i=0;i<COL;i++){
        int temp = *element(*matrixAddr,row1,i);
        *element(*matrixAddr,row1,i) = *element(*matrixAddr,row2,i);
        *element(*matrixAddr,row2,i) = temp;
    }
    printf("�����˵�%d�����%d��\n",row1+1,row2+1);
}

/**�ѵ�row1��ÿ������times1���ټ��ϵ�row2�е�times2��*/
//��ֻ�ı��row1�У���row2�в��䣩
void muladd(Matrix* matrixAddr,int row1,int times1,int row2,int times2){
    for(int i=0;i<COL;i++){
        *element(*matrixAddr,row1,i) *= times1;
        *element(*matrixAddr,row1,i) += times2 * (*element(*matrixAddr,row2,i));
    }
}

/**�ѵ�row1��ÿ������times1���ټ�ȥ��row2�е�times2��*/
void mulsub(Matrix* matrixAddr,int row1,int times1,int row2,int times2){
    muladd(matrixAddr,row1,times1,row2,-times2);
    printf("��%d�г�%d����ȥ%d���ĵ�%d��\n",row1+1,times1,times2,row2+1);
    pri(*matrixAddr);
}
