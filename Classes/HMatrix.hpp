

// This is a Matrix class that is needed for the fitting algorithms
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>


#ifndef HMATRIX_H
#define HMATRIX_H



class HMatrix{


	public:

	//I assume:
	int columns;
	int rows;
	double **Mat;
	
	HMatrix(){}
	HMatrix(int a, int b){
		rows = b;
		columns = a;
	}

	int setData(double **dat){
		printf("This doeant work yet\n");
		return 1;
	}

	int FillMatrix(int i, int j, double dat){
		Mat[i][j] = dat;
		return 1;
	}

	int GetNoRows(){return rows;}
	int GetNoColumns(){return columns;}

	int AddColumn(double * column, int row_n){

		for(int i=0; i < columns; i++){
			Mat[row_n][i] = column[i];
		}

		// Add a column 
		// / 0 1 2 3 \   / 2 \    / 0 1 2 3 2 \
		// | 3 2 1 0 | + | 2 | -> | 3 2 1 0 2 |
		// \ 2 8 9 2 /   \ 2 /    \ 2 8 9 2 2 /


		// Mat[0] Mat[1] 
		// 1234   5678      9012 3456

		return 1;
	}


	int GetColumn(double * ret, int i){
		for(int j = 0; j < rows; j++){
			ret[j] = Mat[i][j];
		}
		return 1;
	}


	int Transpose(){

		double newMat[columns][rows];

		for(int i=0; i < rows; i++){
			for(int j=0; j < columns; j++){
				newMat[j][i] = Mat[i][j];	
			}
		}

		//Mat = (double**) newMat;

		// Transpose matrix
		// i->j, j->i
		// (1,1) -> (1,1)
		// (1,2) -> (2,1)
		// etc

		// / 0 1 2 3 \     / 0 3 2 \
		// | 3 2 1 0 |  -> | 1 2 8 |
		// \ 2 8 9 2 /     | 2 1 9 |
		//                 \ 3 0 2 /

		return 1;

	}

	int Print(){

		for(int j = 0; j < rows; j++){
			printf("| ");
			for(int i = 0; i < columns; i++){
				printf("%f, ", Mat[i][j]);
			}
			printf(" |\n");
		}


		return 1;
	}

	private:


};

#endif


