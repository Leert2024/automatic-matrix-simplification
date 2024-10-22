#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "simple.h"
#include "transformation.h"

#define COL matrixAddr->col
#define ROW matrixAddr->row

/**��ÿ�а�����Ԫ�б��������*/
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

/**�������а���Ԫ�б���ϸ��������*/
void allSwi(Matrix* matrixAddr){
    int firstNotZero[ROW];//�洢ÿ���ȵ�0�ĸ���

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

    printf("�������а���Ԫ�б���ϸ�������У�\n");
    pri(*matrixAddr);
}

/**�������Ƿ���δ����Ϊ�����Σ����Ƿ���Ҫ��������*/
int* ifNotOkey(Matrix* matrixAddr){
    int prev = COL;

    for(int i=ROW-1;i>-1;i--){//�������ϣ��Ƚ���������Ԫ�б꣨ȫΪ0����Ԫ�б�ΪCOL��
        int recent = COL;
        for(int j=0;j<COL;j++){
            if(*element(*matrixAddr,i,j)){
                recent = j;
                break;
            }
        }
        if(recent==prev && recent < COL){//����������Ԫ�б����
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

/**������ÿһ�б�Ϊ�������*/
void divide(Matrix* matrixAddr){
    for(int row = 0;row<ROW;row++){

        //�ҵ���ǰ�е�һ����Ϊ���������ȡ���±�index��ֵtimes
        int index, times = 1;
        for(int i = 0;i < COL;i++){
            if(*element(*matrixAddr,row,i)){
                index = i;
                times = *element(*matrixAddr,row,i);
                break;
            }
        }

        if(index >= COL)continue;//�������Ϊ�գ�ȫΪ0��������

        //����������в�Ϊ0�������������
        for(int i = index+1;i < COL;i++){
            if(*element(*matrixAddr,row,i))times = gcd(times, *element(*matrixAddr,row,i));
        }

        for(int i=0;i<COL;i++){
            *element(*matrixAddr,row,i) /= times;
        }
        if(times!=1)printf("��%d������������%d\n",row+1,times);
    }
    pri(*matrixAddr);
}

/**���±�Ϊaim���е��б���С�ķ���Ԫ����ǰһ����ȥ*/
void cancelFisrt(Matrix* matrixAddr, int aim, int firstNotZero){
    if(aim<=0 || firstNotZero >= COL)return;
    int gcdOfAimAndBefore = gcd(*element(*matrixAddr,aim-1,firstNotZero),*element(*matrixAddr,aim,firstNotZero));
    int times1 = *element(*matrixAddr,aim-1,firstNotZero)/gcdOfAimAndBefore;
    int times2 = *element(*matrixAddr,aim,firstNotZero)/gcdOfAimAndBefore;
    mulsub(matrixAddr,aim,times1,aim-1,times2);
}

/**��֪�������Ĵ�С*/
void getInputSize(Matrix* matrixAddr){
    FILE* fp = fopen("input.txt","r");
    if(!fp){
        printf("�Ҳ����ļ�input.txt�����½��ļ������ԣ�\n");
        system("pause");
        exit(1);
    }

    int rowSize = 1, prevColSize = 0;

    int num;
    char ch = 32;
    int out = 0;

    while(!out){//�м�ѭ��
        int colSize = 0;
        while(1){//ÿһ���ڵ�ѭ��
            ch = fgetc(fp);

            if(ch==EOF){
                if(!colSize)rowSize--;
                out = 1;
                break;
            }else if(ch==10){//����ǻ��з�
                if(colSize != prevColSize && prevColSize && colSize){
                    printf("input.txt�о����%d��Ԫ������%d�����%d��Ԫ������%d�����ȣ�\n",rowSize-1,prevColSize,rowSize,colSize);
                    fclose(fp);
                    system("pause");
                    exit(1);
                }
                if(colSize)prevColSize = colSize;
                rowSize++;
                if(!colSize)rowSize--;
                break;//��������ѭ��������һ��
            }else if(ch==32 || ch=='\t')continue;//����ǿո���Ʊ�������
            else if(ch == '-' || ch == '+' || ch <= '9' && ch >= '0'){//������
                fseek(fp,-1L,SEEK_CUR);
                fscanf(fp,"%d",&num);
                colSize++;
            }else{//�Ƿ��ַ�
                printf("�ļ�input.txt���зǷ����ַ���ascii��Ϊ%d�����ʽ���������ļ���\n",ch);
                fclose(fp);
                system("pause");
                exit(1);
            }
        }
    }
    fclose(fp);

    if(rowSize<2 || prevColSize<2){
        printf("����̫С��%d��%d�У�������ܾ�����\n",rowSize,prevColSize);
        system("pause");
        exit(1);
    }

    COL = prevColSize;
    ROW = rowSize;
}

/**����*/
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
    //��������
    Matrix matrix;

    getInputSize(&matrix);
    printf("�����input.txt��⵽��%d��%d�еľ���\n",matrix.row,matrix.col);

    matrix.address = (int*)malloc(matrix.row*matrix.col*sizeof(int));

    inputMatrix(&matrix);

    //���δ����ľ���
    pri(matrix);

    //�������а���Ԫ�б���ϸ��������
    allSwi(&matrix);

    while(1){
        int* temp = ifNotOkey(&matrix);
        if(!temp[0])break;//�����ѻ�Ϊ�����ͣ�����ѭ��
        cancelFisrt(&matrix,temp[0],temp[1]);
    }

    //ÿ�л�Ϊ�������
    divide(&matrix);

    //������
    printf("��������ս��Ϊ��\n");
    pri(matrix);

    //�������
	system("pause");
	return 0;
}
