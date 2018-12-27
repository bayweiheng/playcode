#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

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

class Perceptron {
	static constexpr int testingPoints = 1000;
	int N,iterations;
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
	// returns the index of the first input point that is misclassified, N if there are none
	int getFirstMisclassified() {
		int ans = 0;
		while(ans < N && predictedOutput(input[ans]) == output[ans]) {
			//printf("%.2lf %.2lf %d\n",input[ans].x,input[ans].y,output[ans]);
			++ans;
		}
		return ans;
	}
	void updateWeights(int idx) {
		w[0] += output[idx] * 0.5;
		w[1] += output[idx] * input[idx].x * 0.5;
		w[2] += output[idx] * input[idx].y * 0.5;
	}
	void printLines() {
		printf("Original Line:%.2lf+%.2lfx+%.2lfy=0\nLearned Line:%.2lf+%.2lfx+%.2lfy=0\nIterations:%d\n\n",
		line[0],line[1],line[2],w[0],w[1],w[2],iterations);
	}
	int run() {
		while(true) {
			int idx = getFirstMisclassified();
			if(idx == N) break;
			iterations++;
			updateWeights(idx);
		}
		//printLines();
		return iterations;
	}
	double test() {
		int numCorrect = 0;
		for(int i=0;i<testingPoints;++i) {
			Pt P = getRandomPoint();
			if(correctOutput(P) == predictedOutput(P)) ++numCorrect;
		}
		return ((double) numCorrect)/testingPoints;
	}
	Perceptron(int N): N(N) {
		iterations = 0;
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
		double totIterations = 0, totPercentageCorrect = 0;
		for(int i=0;i<T;++i) {
			Perceptron perc(N);
			int numIterations = perc.run();
			totIterations += numIterations;
			totPercentageCorrect += perc.test();
		}
		printf("Average iterations: %.2lf\nPercentage correct: %.2lf\n",
			totIterations/T, totPercentageCorrect/T);

	}
};

int main() {
	int T,N;
	scanf("%d %d",&T,&N);
	TrialRunner runner(T,N);
	runner.runTrials();

}

