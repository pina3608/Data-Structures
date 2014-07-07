#include <iostream>
#include <vector>

using namespace std;

class SegmentTree{

public:
	SegmentTree(std::vector<std::vector <int> > *matrix, int rows, int columns);
	int sum(int i1, int i2, int j1, int j2);
	void update ( int i , int j, int val);

private:
	std::vector<std::vector <int> > *treeMatrix;
	int n;
	int m;

	void create2DTree(std::vector<std::vector <int> > *matrix, int start, int end, int currentIndex);
	int create1DTree(std::vector<int> *v, int start, int end, int staticIndex, int currentIndex);

	void sumRows(int sourceA, int sourceB, int dest);

	int sum2D(int i1, int i2, int j1, int j2, int start, int end, int index);
	int sum1D(int j1, int j2, int start, int end, int staticIndex, int index);

	void update2D(int i, int j, int val, int start, int end, int index);
	void update1D(int j, int val, int start, int end, int staticIndex, int index);

};


SegmentTree::SegmentTree(std::vector<std::vector <int> > *matrix, int rows, int columns){
	if(matrix->size() == rows && (*matrix)[0].size()== rows ){
		n = rows;
		m = columns;
		treeMatrix = new std::vector<std::vector <int> > (matrix->size() * 3,
														std::vector <int> ((*matrix)[0].size() * 3));
		create2DTree(matrix, 0, n-1, 0);
	}
}

int SegmentTree::sum (int i1, int i2, int j1, int j2){
	return sum2D(i1,i2,j1,j2, 0, n-1, 0);
}

void SegmentTree::update(int i, int j, int val){
	update2D(i, j, val, 0, n-1, 0);
}


void SegmentTree::update2D(int i, int j, int val, int start, int end, int index){
	if(start == end) update1D(j, val, 0, m-1, index, 0);
	else{
		int mid = (start+end) /2;
		if(i <= mid){
			update2D(i, j, val, start, mid,  index*2 +1);
		}
		else{
			update2D(i, j, val, mid+1, end, index*2 +2);
		}
		sumRows(index*2 +1, index*2 +2, index);

	}
}

void SegmentTree::update1D(int j, int val, int start, int end, int staticIndex, int index){
	if(start == end) (*treeMatrix)[staticIndex][index] = val;
	else{
		int mid = (start+end) /2;
		if(j <= mid){
			update1D(j, val, start, mid, staticIndex, index*2 +1);
		}
		else{
			update1D(j, val, mid+1, end, staticIndex, index*2 +2);
		}
		(*treeMatrix)[staticIndex][index] = (*treeMatrix)[staticIndex][index*2+1]
				+(*treeMatrix)[staticIndex][index*2+2];
	}
}

int SegmentTree::sum2D (int i1, int i2, int j1, int j2, int start, int end, int index){
	if(start >= i1 && end <= i2) return sum1D(j1, j2, 0, m-1, index, 0);
	if(i2 < start) return 0;
	if(i1 > end) return 0;

	int mid = (start + end) /2;

	int sum = 0;
	sum += sum2D(i1,i2,j1,j2, start, mid, index*2 +1);
	sum += sum2D(i1,i2,j1,j2, mid+1, end, index*2 +2);

	return sum;

}

int SegmentTree::sum1D(int j1, int j2, int start, int end, int staticIndex, int index){

	if(start >= j1 && end <= j2) return (*treeMatrix)[staticIndex][index];
	if(j2 < start) return 0;
	if(j1 > end) return 0;

	int mid = (start + end) /2;

	int sum = 0;
	sum += sum1D(j1,j2, start, mid, staticIndex, index*2 +1);
	sum += sum1D(j1,j2, mid+1, end, staticIndex, index*2 +2);

	return sum;


}



void SegmentTree::create2DTree(std::vector<std::vector <int> > *matrix, int start, int end,
																		int currentIndex){

	if(start == end){
		create1DTree(&((*matrix)[start]), 0, m-1, currentIndex, 0);
	}
	else{
		int mid = (start + end) /2;
		create2DTree(matrix, start, mid, currentIndex*2 +1);
		create2DTree(matrix, mid+1, end, currentIndex*2 +2);
		sumRows(currentIndex*2 +1, currentIndex*2 +2, currentIndex);
	}

}

void SegmentTree::sumRows(int sourceA, int sourceB, int dest){

	if((*treeMatrix)[sourceA].size() != (*treeMatrix)[sourceB].size() ){
		return;
	}

	for(int i = 0; i < (*treeMatrix)[sourceA].size(); i++){
		(*treeMatrix)[dest][i] = (*treeMatrix)[sourceA][i] + (*treeMatrix)[sourceB][i];
	}

}
int SegmentTree::create1DTree(std::vector <int> *matrix, int start, int end,
																	int staticIndex, int currentIndex){


	if(start == end){
		(*treeMatrix)[staticIndex][currentIndex] = (*matrix)[start];
		return (*matrix)[start];
	}
	else{
		int mid = (start + end) /2;
		int sol = 
			create1DTree(matrix, start, mid, staticIndex, currentIndex*2 +1) 
			+ create1DTree(matrix, mid+1, end, staticIndex, currentIndex*2 +2);

		(*treeMatrix)[staticIndex][currentIndex] = sol;
		return sol;
	}

}

int main(){

	std::vector<std::vector <int> > t (10,std::vector<int> (10));
	t[0][1] = 10;
	t[0][2] = 20;
	t[1][2] = 50;

	SegmentTree *tree = new SegmentTree(&t, 10, 10);


	cout << tree->sum(0,0,0,1) << endl;
	cout << tree->sum(0,0,0,2) << endl;
	cout << tree->sum(0,1,0,1) << endl;
	cout << tree->sum(0,1,0,2) << endl;

	cout << endl;
	tree->update(1,2, 100);


	cout << tree->sum(0,0,0,1) << endl;
	cout << tree->sum(0,0,0,2) << endl;
	cout << tree->sum(0,1,0,1) << endl;
	cout << tree->sum(0,1,0,2) << endl;

	cout << endl;
	tree->update(0,2, 0);


	cout << tree->sum(0,0,0,1) << endl;
	cout << tree->sum(0,0,0,2) << endl;
	cout << tree->sum(0,1,0,1) << endl;
	cout << tree->sum(0,1,0,2) << endl;

}
