#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef vector<double> row;
typedef vector<row> matrix;

constexpr double eps = 1e-10;

// assumes proper dimensions
matrix matrix_multiply(matrix &A, matrix &B) {
	int p = A.size(), q = A[0].size(), r = B[0].size();
	//printf("%d %d %d %d\n", p,q,(int) B.size(),r);
	matrix ans(p, row(r, 0));
	for(int i=0;i<p;++i) {
		for(int j=0;j<r;++j) {
			for(int k=0;k<q;++k) ans[i][j]+=A[i][k]*B[k][j];
		}
	}
	return ans;
}

// inverts the matrix A via Gauss-Jordan elimination
void invert(matrix &A) {
  const int n = A.size();
  vector<int> irow(n), icol(n), ipiv(n);

  for (int i = 0; i < n; i++) {
    int pj = -1, pk = -1;
    for (int j = 0; j < n; j++) if (!ipiv[j])
      for (int k = 0; k < n; k++) if (!ipiv[k])
        if (pj == -1 || fabs(A[j][k]) > fabs(A[pj][pk])) { pj = j; pk = k; }
    if (fabs(A[pj][pk]) < eps) return;
    ipiv[pk]++;
    swap(A[pj], A[pk]);
    irow[i] = pj;
    icol[i] = pk;

    double c = 1.0 / A[pk][pk];
    A[pk][pk] = 1.0;
    for (int p = 0; p < n; p++) A[pk][p] *= c;
    for (int p = 0; p < n; p++) if (p != pk) {
      c = A[p][pk];
      A[p][pk] = 0;
      for (int q = 0; q < n; q++) A[p][q] -= A[pk][q] * c;     
    }
  }

  for (int p = n-1; p >= 0; p--) if (irow[p] != icol[p]) {
    for (int k = 0; k < n; k++) swap(A[k][irow[p]], A[k][icol[p]]);
  }
}

struct Pt {
	double x,y;
	Pt(double x,double y):x(x),y(y) {}
	Pt() { x=0; y=0; }
	Pt(const Pt &p) { x=p.x; y=p.y; }
};

struct Vec {
	double x,y;
	Vec(Pt a, Pt b) { x=b.x-a.x; y=a.y-b.y;}
	Vec() { x=0; y=0; }
};

// returns z-coordinate of cross product, others are trivially zero
int cross(Vec v,Vec w) {
	return v.x*w.y - v.y*w.x;
}

// returns whether the point P lies on the left of (or on) the ray AB
bool onLeftSide(Pt A,Pt B,Pt P) {
	return cross(Vec(A,B),Vec(A,P)) >= 0;
}

double getRand() {
	return ((double) rand())/RAND_MAX*2-1;
}

class LinearRegression {
	static constexpr int testingPoints = 1000;
	int N;
	Pt A,B;
	vector<Pt> input;
	vector<int> output;
	vector<double> w,line;
	Pt getRandomPoint() {
		return Pt(getRand(), getRand());
	}
	public:
	int predictedOutput(Pt P) {
		double dotProduct = w[0] + w[1] * P.x + w[2] * P.y;
		if(dotProduct >= 0) return 1;
		else return -1;
	}
	int correctOutput(Pt P) {
		double dotProduct = line[0] + line[1] * P.x + line[2] * P.y;
		if(dotProduct >= 0) return 1;
		else return -1;
	}
	void printLines() {
		printf("Original Line:%.2lf+%.2lfx+%.2lfy=0\nLearned Line:%.2lf+%.2lfx+%.2lfy=0\n\n",
		line[0],line[1],line[2],w[0],w[1],w[2]);
	}
	double run() {
		matrix X(N,row(3)), XT(3,row(N)), y(N,row(1));
		for(int i=0;i<N;++i) {
			X[i][0] = XT[0][i] = 1;
			X[i][1] = XT[1][i] = input[i].x;
			X[i][2] = XT[2][i] = input[i].y;
			y[i][0] = output[i];
		}
		matrix Xdagger = matrix_multiply(XT, X), XTy = matrix_multiply(XT, y);
		invert(Xdagger);
		matrix weights = matrix_multiply(Xdagger, XTy);
		for(int i=0;i<3;++i) w[i] = weights[i][0];
		int numCorrect = 0;
		for(int i=0;i<N;++i) {
			if(correctOutput(input[i]) == predictedOutput(input[i])) ++numCorrect;
		}
		double p = ((double) numCorrect)/N;
		return max(p,1-p);
		//printLines();
	}
	double test() {
		int numCorrect = 0;
		for(int i=0;i<testingPoints;++i) {
			Pt P = getRandomPoint();
			if(correctOutput(P) == predictedOutput(P)) ++numCorrect;
		}
		double p = ((double) numCorrect)/testingPoints;
		return max(p,1-p);
	}
	LinearRegression(int N): N(N) {
		A = getRandomPoint();
		B = getRandomPoint();
		line = w = vector<double>(3,0);
		line[1] = A.y-B.y;
		line[2] = B.x-A.x;
		line[0] = -line[1] * A.x -line[2] * A.y;
		for(int i=0;i<N;++i) {
			Pt P = getRandomPoint();
			input.push_back(P);
			output.push_back(correctOutput(P));
		}

	}

};

class TrialRunner {
	int T,N;
	public:
	TrialRunner(int T,int N):T(T),N(N) {}

	// runs T trials, each with N points, and prints statistics
	void runTrials() {
		srand(time(NULL));
		double totPercentageCorrectInSample = 0, totPercentageCorrect = 0;
		for(int i=0;i<T;++i) {
			LinearRegression linreg(N);
			totPercentageCorrectInSample += linreg.run();
			totPercentageCorrect += linreg.test();
		}
		printf("Percentage correct in sample: %.2lf\nPercentage correct out of sample: %.2lf\n",
			totPercentageCorrectInSample/T, totPercentageCorrect/T);

	}
};

int main() {
	int T,N;
	scanf("%d %d",&T,&N);
	TrialRunner runner(T,N);
	runner.runTrials();

}

