/**
 * Sereja and Bubble Sort
 * TOPIC: dynamic programming, expectation, strategy
 * status:
 */
#include <cassert>
#include <iomanip>

#include <algorithm>
#include <functional>
#include <numeric>

#include <iostream>
#include <cstdio>

#include <cstring>
#include <string>
#include <memory>

#include <cctype>
#include <cmath>

#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#define N (0x80)
#define oo (1LL << 62)
#define INV(n) ((n)*((n)-1)/2)

using i64= std::int64_t;

std::istream &is = std::cin;
std::ostream &os = std::cout;

double z[N][INV(N)+1];
std::vector<double> prefixSum[N], wps[N];

// calculate the number of probability
// of a permutation on "n" elements, with exactly "k" inversions
double calc_z(int n, int k) {
	if (k > INV(n))
		return 0.00;
	if (z[n][k] > -oo)
		return z[n][k];
	if (n == 1)
		return z[n][k]= 1.00;
	assert(n > 1);
	z[n][k]= 0;
	for (int j= std::min(n-1,k); j >= 0; --j) 
		z[n][k]+= calc_z(n-1,k-j)/n;
	return z[n][k];
}

double dp[INV(N)];

int main() {
#ifndef ONLINE_JUDGE
    freopen("seabub.txt","r",stdin);
#endif
	int i,j,ts;
	i64 n,k;
	for ( i= 0; i < N; ++i )
		for ( j= 0; j <= INV(i); z[i][j++]= -oo ) ;
	for ( n= 1; n < N; ++n ) {
		for ( k= 0; k <= INV(n); calc_z(n,k++) );
		std::partial_sum(z[n],z[n]+INV(n)+1,std::back_inserter(prefixSum[n]));
		wps[n].resize(INV(n)+1,0.00);
		for ( k= 1; k <= INV(n); ++k )
			wps[n][k]= wps[n][k-1]+k*calc_z(n,k);
	}

	for (is >> ts; ts--;) {
		is >> n >> k;
		std::vector<int> vec(n);
		for(auto &c : vec)
			is >> c;
		i64 inv= 0;
		for (i= 0; i < n; ++i)
			for (j= i+1; j < n; ++j)
				if (vec[j] < vec[i])
					++inv;
		if ( k == 0 ) {
			printf("%d\n",inv);
			continue ;
		}
		if ( k >= inv ) {
			printf("0\n");
			continue ;
		}
		assert( inv > k );
		k= std::min(k,static_cast<i64>(INV(n)+1));
		for ( dp[0]= 0.00, j= 1; j <= INV(n); dp[0]+= j*calc_z(n,j), ++j ) ;
		for ( i= 1; i < k; ++i ) {
			//[i+1,i+dp[i-1]]
			int r= static_cast<int>(i+std::floor(dp[i-1]));
			int left= i+1,
				right= std::min(INV(n),static_cast<i64>(r));
			dp[i]= (wps[n][right]-wps[n][left-1])-(left-1)*(prefixSum[n][right]-prefixSum[n][left-1]);
			dp[i]+= std::max(0.00, dp[i-1]*(prefixSum[n][INV(n)]-prefixSum[n][r]));
		}
		printf("%.7lf\n",std::min(dp[k-1],static_cast<double>(inv-k)));
	}
    return 0;
}

