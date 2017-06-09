#include<iostream>

using namespace std;

//code does not work for all linear programming problems

const int MAX = 50;		//maximum matrix size
int basic = 2;			//number of basic variable (ex. x1, x2, x3, ....)
int nbasic = 2;			//number of slack variable (ex. s1, s2, s3, ....)
int row, col;
bool unboundSolution = false;

/* matrix format

similar to simplex tablaeu
check the notes
Z or objective function goes last in the matrix
one example is given here

x1 + x2 + s1 = 4
2x1 + x2 + s2 = 5
Z = 3x1 + 4x2 --> (Z) - 3x1 - 4x2 = 0 

*/

float t[MAX][MAX] = {
		{1, 1, 1, 0, 4},
        	{2, 1, 0, 1, 5},
        	{-3, -4, 0, 0, 0}        
		};
int basis[MAX];
int pivotColumn, pivotRow;

void printtable() {

	for(int i=0 ; i<row ; i++) {		
		for (int j=0; j<col ; j++) {
			cout<< t[i][j] << "\t";
		}
		cout << "\n";
	}
	cout << "\n";

}

bool checkOptimal() {
	bool isOptimal = false;
	for (int i=0 ; i<col-1 ; i++) {
		int value = t[row-1][i];
		if(value < 0) {
			return isOptimal;
		}
	}
	isOptimal = true;
	return isOptimal;
}

int enteringColumn() {
	int minimum = 0;
	for (int i=0 ; i<col-1 ; i++) {
		int value = t[row-1][i];
		//cout <<"Val is " << value <<"\n";
		if (value < 0) {
			if (value <= t[row-1][minimum]) {
				minimum = i;
				//cout << "Minimum is " << minimum << "\n";
			}
		}
	}

	return minimum;
}

float* calRatios(int column) {
	float positiveEntries[col];
	static float ratios[MAX];
	int negativecount = 0;
	for (int i=0 ; i<row ; i++) {
		if (t[i][column] > 0) {
			positiveEntries[i] = t[i][column];
		}
		else {
			positiveEntries[i] = 0;
			negativecount += 1;
		}
	}

	if (negativecount == row) {
		cout << "No solution is available \n";
		unboundSolution = true;
	}
	else {
		for (int i=0 ; i<row ; i++) {
			float val = positiveEntries[i];
			if (val > 0) {
				ratios[i] = t[i][col-1]/val;
			}
			else {
				ratios[i] = -1.0f;
			}
		}
	}
	return ratios;
}

int findSmallestRatio(float arr[]) {
	int location = 0;
	float minimum = arr[0];
	for (int j=1 ; j<row ; j++) {
		float val = arr[j];
		if (val > 0) {
			if (minimum == -1 || val < minimum) {
				minimum = val;
				location = j;
			}
		}
		
	}
	return location;
}

void setBasis() {
	basis[pivotRow] = pivotColumn + 1;
}

void formNextTable(int pivotColumn, int pivotRow) {
	float pivotValue = t[pivotRow][pivotColumn];
	float pivotColumnVals[MAX];
	float rowNew[MAX];

	float *pivotRowVals = t[pivotRow];

	for (int i=0 ; i<row ; i++) {
		pivotColumnVals[i] = t[i][pivotColumn];
	}

	for (int i=0 ; i<col ; i++) {
		rowNew[i] = pivotRowVals[i]/pivotValue;
	}

	for (int i=0 ; i<row ; i++) {
		if(i!=pivotRow) {
			for (int j=0 ; j<col ; j++) {
				float val = pivotColumnVals[i];
				t[i][j] = t[i][j] - (val * rowNew[j]);
			}
		}
		else {
			for (int j=0 ; j<col ; j++) {
				t[pivotRow][j] = t[pivotRow][j] / pivotValue;
			}
		}
	}
}

void printSolution() {
	for (int i=0 ; i<row-1 ; i++) {
		int val = basis[i];
		if (val > 0) {
			cout << "x" << val << " : " << t[i][col-1] << "\n";
		}
		else {
			cout << "s" << i + 1 << " : " << t[i][col-1] << "\n";
		}
	}
	cout << "Max is " << t[row-1][col-1] << "\n";
}	


int main() {
	col = basic + nbasic + 1;
	row = nbasic + 1;	
	
	printtable();
	bool optimal = checkOptimal();	
	cout << "\n";

	while (!optimal) {
		pivotColumn = enteringColumn();
		cout << "Pivot Column " << pivotColumn << "\n";
		cout << "\n";
		float *ratio = calRatios(pivotColumn);
		if (unboundSolution == true) {
			return 0;
		}
		else {
			for (int j=0 ; j<row ; j++) {
				cout << ratio[j] << "\t";
			}
			cout<< "\n";
		}
		cout << "\n";
		
		pivotRow = findSmallestRatio(ratio);
		setBasis();
		cout << "Pivot Row " << pivotRow << "\n";
		cout << "\n";

		formNextTable(pivotColumn, pivotRow);
		printtable();
		cout << "\n";

		optimal = checkOptimal();
	}

	printSolution();
	

	return 0;

}

