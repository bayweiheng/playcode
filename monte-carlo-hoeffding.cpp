#include <bits/stdc++.h>

using namespace std;

typedef tuple<int,int,int> iii;

static constexpr int numFlips = 10;

class Experiment {
	int N; // N is the number of coins

	// flips a coin numFlips times, reports the number of heads
	int numHeads() {
		int heads = 0;
		for(int i=0;i<numFlips;++i) if(rand()&1) ++heads;
		return heads;
	}

public:
	Experiment(int N): N(N) {}

	iii getResults() {
		int v1 = numHeads(), vrand = v1, vmin = numFlips;
		for(int i=2;i<=N;++i) {
			int h = numHeads();
			vmin = min(vmin, h);
			if(rand()%i==0) vrand = h; // reservoir sampling
		}
		return iii(v1,vrand,vmin);
	}

};

class Engine {
	int T; // T is the number of trials

public:
	Engine(int T): T(T) {}

	void run() {
		srand(time(NULL));
		vector<int> counts1(numFlips + 1,0), countsrand = counts1, countsmin = counts1;
		for(int i=0;i<T;++i) {
			Experiment e(1000);
			iii t = e.getResults();
			++counts1[get<0>(t)];
			++countsrand[get<1>(t)];
			++countsmin[get<2>(t)];
		}
		int totmin = 0;
		for(int i: counts1) printf("%d ",i); printf("\n");
		for(int i: countsrand) printf("%d ",i); printf("\n");
		for(int i: countsmin) printf("%d ",i); printf("\n");
		for(int i=0;i<=numFlips;++i) totmin += i*countsmin[i];
		printf("average value of vmin is: %.4lf\n", ((double) totmin)/T);
	}

};

int main () {
	Engine eng(100000);
	eng.run();

}