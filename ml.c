/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);
void printMatrix(double** mat, int row, int col);
void freeMatrix(double** mat, int row);
// main method starts here
int main(int argc, char** argv){

  	//read in files 
	FILE *trainFile;
	trainFile= fopen(argv[1], "r");

	FILE *testFile;
	testFile= fopen(argv[2], "r");

	if( trainFile == NULL || testFile == NULL){
		printf("file error\n");
	}	

	// getting rows and columns for each matrix
	int trainCol;
	int trainRow;

	fscanf(trainFile, "%d", &trainCol);
	fscanf(trainFile, "%d", &trainRow);
	
	int testRow;
	fscanf(testFile, "%d", &testRow);

	//creating the matrices
	
	// training matrix: nx(k+1) and price matrix: nx1

	int i;
	double** trainMat= calloc(trainRow, sizeof(double*));
	double** priceMat= calloc(trainRow, sizeof(double*));

	for(i= 0; i < trainRow; i++){
		priceMat[i]= calloc(1, sizeof(double));
		trainMat[i]= calloc((trainCol +1),  sizeof(double));
	}
	
	int j;

	
	for(i=0; i< trainRow; i++){
			for(j=0; j<(trainCol+2); j++){
				//create column of 1s
				if(j == 0){
					trainMat[i][0]= 1.0;
			 	//make price matrix: nx1
				}else if(j == trainCol+1){
					double num; 
					char holder;
					fscanf(trainFile, "%lf", &num);
					fscanf(trainFile, "%c", &holder);
					priceMat[i][0]= num;
						
				}else{
					double num;
					char holder;
					fscanf(trainFile, "%lf", &num);
					fscanf(trainFile, "%c", &holder);
					trainMat[i][j]= num;
			}
		} 
	}
	 

	// create the test matrix
	double** testMat= calloc(testRow, sizeof(double*));
	for(i=0; i< testRow; i++){
		testMat[i]= calloc(trainCol+1, sizeof(double));
	}	

	
	for(i=0; i<testRow; i++){

		for(j=0; j< trainCol+1; j++){
			if( j==0){
				testMat[i][0]= 1.0;
			}else{
				double temp;			
				fscanf(testFile, "%lf", &temp);
				char c;
				fscanf(testFile, "%c", &c);
				testMat[i][j]= temp;
			}
		}
	}
	
	// *****CALCULATE W MATRIX*******

	//call transpose
	double** transMat= transposeMatrix(trainMat, trainRow, trainCol+1);
	//printf("Transpose \n");
	//printMatrix(transMat, trainCol+1, trainRow);
	//multiply train matrix with its transpose
	double** transbyTrain= multiplyMatrix(transMat, trainMat, trainCol+1, trainRow, trainRow, trainCol+1);
	//printf(" Transpose by Train\n");
	//printMatrix(transbyTrain, trainCol+1, trainCol+1);
	// find inverse of transpose x train
	double** inverse= inverseMatrix(transbyTrain, trainCol+1);
	//printf("Inverse of Transpose by Train\n");
	//printMatrix(inverse, trainCol+1, trainCol+1);
	
	// Multiply inverse by transpose
	double** Ixtrans= multiplyMatrix(inverse, transMat, trainCol+1, trainCol+1, trainCol+1, trainRow);
	
	// Multiply IxTranspose by prices
	double** W= multiplyMatrix(Ixtrans, priceMat, trainCol+1, trainRow, trainRow, 1);
	//printf("W matrix!\n");
	//printMatrix(W, trainCol+1, 1);

	// **** CALCULATE THE NEW PRICES *****
	
	// multiply test by W
	double** testxW= multiplyMatrix(testMat, W, testRow, trainCol+1, trainCol+1, 1);
	//printf("Final Prices\n");
	printMatrix(testxW, testRow, 1);

	
	//Free matrices
	freeMatrix(testxW, testRow);
	freeMatrix(W, trainCol+1);
	freeMatrix(Ixtrans, trainCol+1);
	freeMatrix(inverse, trainCol+1);
	freeMatrix(transbyTrain, trainCol+1);
	freeMatrix(transMat, trainCol+1);
	freeMatrix(trainMat, trainRow);
	freeMatrix(testMat, testRow);
	freeMatrix(priceMat, trainRow);	

	fclose(trainFile);
	fclose(testFile);
	return 0;
}

void freeMatrix(double **mat, int row){
	int i;
	for(i = 0; i< row; i++){
		free(mat[i]);
	}
	free(mat);
}

void printMatrix(double** mat, int row, int col){
	int i, j;
	for(i=0; i<row; i++){
		for(j=0; j< col; j++){
			printf("%0.0lf\n", mat[i][j]);
		}

	}

}

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
	double** result=malloc(r1*sizeof(double*));
	int i, j, k;
	for(i=0; i<r1; i++){
		result[i]= calloc(c2, sizeof(double));
	}
    	double sum;
	// your code goes here
	for(i=0; i< r1; i++){
		for(j=0; j< c2; j++){
			for(k=0; k< r2; k++){
				sum += matA[i][k] * matB[k][j];
			}
			result[i][j]=sum;
			sum=0;
		}
	}
	
	//print matrix
	//printMatrix(result, r1, c2);
    
	return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
  	int transRow= col;
	int transCol= row;
	double** matTran=malloc(col*sizeof(double*));
       	int i;	
    	for(i=0; i<transRow; i++){
		matTran[i]= calloc(transCol, sizeof(double));
	}

    // your code goes here
    int j;

    for(i=0; i< transRow; i++){
    	for(j=0; j< transCol; j++){
		matTran[i][j]= mat[j][i];
	}
    } 
	
	// print result
	//printMatrix(matTran, transRow, transCol);    
    return matTran;        
}


double** inverseMatrix(double **matA, int dimension)
{

	// create identity matrix
	int i, j;
	double** I= calloc(dimension, sizeof(double*));
	for(i=0; i<dimension; i++){
		I[i]= calloc(dimension, sizeof(double));
	}
	for(i=0; i<dimension; i++){
		for(j=0; j <dimension; j++){
			I[i][i]= 1;
		}
	}

	//procedure
	double num;
	int p;
	for(p=0; p<dimension; p++){
		num= matA[p][p];

		//divide Mp and Np by num
		for(j=0; j<dimension; j++){
			matA[p][j]= matA[p][j]/ num;
			I[p][j]= I[p][j]/ num;
		}

		for(i= 0; i <dimension; i++){
			num= matA[i][p];
			//Subtract Mp x num from Mi
			for(j=0; j<dimension; j++){
				if(i==p){
					break;
				}
				matA[i][j]= matA[i][j] -(num*matA[p][j]);
				I[i][j]= I[i][j] - (num*I[p][j]);
			}
		}
	}    	

	return I;
}


