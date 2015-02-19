#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAX 100
#define MAXLEN 1000

typedef struct matrix{
	char *variable;
	int mat[MAX][MAX];
	int stat;
	int row;
	int column;
	struct matrix *prev;
	struct matrix *next;
}matrix;

matrix *head, *tail;
int totalnum = 0;
int result[MAX][MAX];

void operations (char *filename);
void strassen(int A[MAX][MAX], int B[MAX][MAX], int row, int column);

int main (int argc, char* argv[]){
	if (argc!=2){
		printf("Enter correct number of arguments."); //check arguments number
		exit(-1);
	}
	operations(argv[1]);
	return 0;
}

void operations (char *filename) {
	int i, j = 0, k;
	char *expression = malloc(MAXLEN);
	char *buffer = malloc(MAXLEN);
	FILE *input = fopen(filename, "r+");
	if (!input){
		printf("Input file not found.\n");
		exit(0);
	}
	fgets(expression, MAXLEN-1, input);
	printf("***************************************\n");
	printf("Epression to be calculated : %s\n", expression);
	printf("***************************************\n");
	for (i=0;i<strlen(expression);i++){
		j=0;
		while (expression[i]!='-'&&expression[i]!='+'&&expression[i]!='*'&&expression[i]!='\0'){ //get variable name till a operator
			buffer[j] = expression[i]; //variable name concatenated
			j++;i++;
		}
		buffer[j]='\0';
		if (head==NULL){ //push variable name in linked list (link list push process in following lines)
			matrix *node=malloc(sizeof(matrix));
			node->variable = malloc(strlen(buffer));
			strcpy(node->variable, buffer);
			node->stat = 0;
			head = node;
			head->prev = NULL;
			tail = node;
			tail->next = NULL;
			totalnum++;
		}
		else { //push variable name in linked list (link list push process in following lines)
			matrix *node=malloc(sizeof(matrix));
			node->variable = malloc(strlen(buffer));
			strcpy(node->variable, buffer);
			node->stat = 0;
			node->prev = tail;
			tail->next = node;
			tail = node;
			tail->next = NULL;
			totalnum++;
		}
		if (expression[i]!='\0'){ //push operator in linked list (link list push process in following lines)
			matrix *node=malloc(sizeof(matrix));
			node->stat = expression[i];
			tail->next = node;
			node->prev = tail;
			tail = node;
			tail->next = NULL;
			totalnum++;
		}
	}
	matrix *node= head;j=0;
	while(node!=NULL){
		char *token;
		if (node->stat==0){ //stat set to zero for variables and the operator ascii value for operators
			int row, column;
			fgets(expression, MAXLEN-1, input); //get row of current variable i.e. matrix
			expression[strlen(expression)-1]='\0';
			row = atoi(expression);
			node->row = row;
			fgets(expression, MAXLEN-1, input); //get column of current variable i.e. matrix
			expression[strlen(expression)-1]='\0';
			column = atoi(expression);
			node->column = column;
			fgets(expression, MAXLEN-1, input); //get contents
			expression[strlen(expression)-1]='\0';
			printf("Matrix : %s\n", node->variable);
			token = strtok(expression, " "); //tokenize on basis of spaces
			for (i=0;i<row;i++){
				for (j=0;j<column;j++) {
					node->mat[i][j] = atoi(token); //assign matrix its contents
					printf("%s\t", token);
					token = strtok(NULL, " ");
				}
				printf("\n");
			}
		}
		node=node->next;
	}
	printf("***************************************\n");
	int foundmul=1;
	node=head;
	while(foundmul){
		foundmul=0;
		while(node!=NULL){
			if (node->stat==42){ //if any multiplication operator is found then calculate it first
				foundmul=1;
				break;
			}
			node=node->next;
		}
		if(foundmul){
			matrix *prev = node->prev; //multiplication process below
			node=node->next;
			if (prev->column!=node->row){
				printf("Math Error: Illegal Multiplication in the expression.");
				exit(0);
			}
			if (prev->row==prev->column){
				printf("\nThe matrices to be multiplied are square matrices so strassen algorithm is executed.\n");
				if (prev->row%2){
					strassen(prev->mat, node->mat, prev->row+1, prev->column+1);
				}
				else{
					strassen(prev->mat, node->mat, prev->row, prev->column);
				}
			}
			else {
				for(i=0;i<(prev->row);i++){
					for(j=0;j<(node->column);j++){
						int sum=0;
						for(k=0;k<(prev->column);k++)
							sum+=prev->mat[i][k]*node->mat[k][j];
						result[i][j]=sum;
					}
				}
			}
			for(i=0;i<prev->row;i++){
				for(j=0;j<node->column;j++){
					prev->mat[i][j]=result[i][j];
				}
			}
			prev->column=node->column;
			prev->next=node->next;
			if (node->next!=NULL)
				node->next->prev=prev;
			totalnum-=2;
		}
	}
	node= head;j=0;
	while(node!=NULL){
		if (node->stat!=0){
			if (node->stat==43){ //check for add operator (add operation performed below)
				matrix *prev = node->prev;
				node = node->next;
				if (node->row!=prev->row||node->column!=prev->column){
					printf("Math Error: Order of matrices to be added should be same.");
					exit(0);
				}
				for (i=0;i<node->row;i++){
					for (j=0;j<node->row;j++){
						prev->mat[i][j] = prev->mat[i][j]+node->mat[i][j];
					}
				}
				prev->next=node->next;
				if (node->next!=NULL)
					node->next->prev=prev;
				totalnum-=2;
			}
			else { //check for subtract operator (substract operation performed below)
				matrix *prev = node->prev;
				node = node->next;
				if (node->row!=prev->row||node->column!=prev->column){
					printf("Math Error: Order of matrices to be added should be same.");
					exit(0);
				}
				for (i=0;i<node->row;i++){
					for (j=0;j<node->row;j++){
						prev->mat[i][j] = prev->mat[i][j]-node->mat[i][j];
					}
				}
				prev->next=node->next;
				if (node->next!=NULL)
					node->next->prev=prev;
				totalnum-=2;
			}
		}
		node=node->next;
	}
	printf("\nAnswer : \n");
	for (i=0;i<head->row;i++){
		for (j=0;j<head->column;j++){
			printf("%d\t", head->mat[i][j]);
		}
		printf("\n");
	}
	printf("***************************************\n");
}

void strassen(int A[MAX][MAX], int B[MAX][MAX], int row, int column){
	int i, j, k ,l, m, o;
	int m1, m2, m3, m4, m5, m6, m7;
	for (i=0;i<row;i+=2){
		for(j=0;j<column;j+=2){
			for(m=0;m<row;m+=2){
				k = i+1;l=j+1;o=m+1;
				m1 = (A[i][m]+A[k][o])*(B[m][j]+B[o][l]);
				m2 = (A[k][m]+A[k][o])*B[m][j];
				m3 = A[i][m]*(B[m][l]-B[o][l]);
				m4 = A[k][o]*(B[o][j]-B[m][j]);
				m5 = (A[i][m]+A[i][o])*B[o][l];
				m6 = (A[k][m]-A[i][m])*(B[m][j]+B[m][l]);
				m7 = (A[i][o]-A[k][o])*(B[o][j]+B[o][l]);
				result[i][j] += (m1+m4-m5+m7);
				result[i][l] += (m3+m5);
				result[k][j] += (m2+m4);
				result[k][l] += (m1-m2+m3+m6);
			}
		}
	}
}
