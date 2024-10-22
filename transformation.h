#ifndef TRANSFORMATION_H_INCLUDED
#define TRANSFORMATION_H_INCLUDED

typedef struct Matrix{
    int row;
    int col;
    int* address;
}Matrix;

void setColor(int color);
int* element(Matrix matrix, int row, int col);
void pri(const Matrix matrix);
void swi(Matrix* matrixAddr, int row1, int row2);
void muladd(Matrix* matrixAddr,int row1,int times1,int row2,int times2);
void mulsub(Matrix* matrixAddr,int row1,int times1,int row2,int times2);

#endif // TRANSFORMATION_H_INCLUDED
