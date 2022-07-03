#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <functional>

using namespace std;

class SuffixArray {
  public:
    string text;
    int n;                      // Length of text
    vector<int> S;              // Permutation which sorts suffixes
    vector<int> R;              // Ranking array (an inverse of S)

    // Construct suffix array and ranking array for the given string
    // using the doubling algorithm.
    SuffixArray(const string &orig_text)
    {
        text = orig_text;
        n = text.size();
        S.resize(n+1);
        R.resize(n+1);

        sort_and_rank([this](int a, int b) -> bool { return text[a] < text[b]; });

        for (int k=1; k<n; k*=2) {
            sort_and_rank([this,k](int a, int b) -> bool {
                    pair<int,int> pa(R[a], (a+k < n) ? R[a+k] : -1);
                    pair<int,int> pb(R[b], (b+k < n) ? R[b+k] : -1);
                    return (pa < pb);
                    });
        }
    }

    // An auxiliary function used in the doubling algorithm.
    void sort_and_rank(function<bool(int a, int b)> comp)
    {
        for (size_t i=0; i<S.size(); i++)
            S[i] = i;

        sort(S.begin(), S.end(), comp);

        vector<int> R2(S.size());
        for (size_t i=0; i<S.size(); i++) {
            if (!i || comp(S[i-1], S[i]) || comp(S[i], S[i-1]))
                R2[S[i]] = i;
            else
                R2[S[i]] = R2[S[i-1]];
        }
        R.swap(R2);
    }

    // Return the number of distinct k-grams in the string.
    int num_kgrams(int k)
    {
        // TODO: Implement
        _LCP();

		int kgrams = 0;	
		int ind = 0; 
		int len = text.length();

		while (ind < int(S.size()))
		{
			if (len - S[ind] >= k)
			{
				kgrams++;
				while (ind < int(S.size()) && LCP[ind] >= k)
				{	ind++;}
			}
			ind++;

		}
		return kgrams;
    }
private:

	vector<int> LCP;
	void _LCP()
	{
		LCP.resize(S.size());

		int s_size = S.size();	
		for (int i = 0, _len = 0; i < s_size; i++) 
		{
			if (R[i] < s_size - 1) 
			{			
				int j = S[R[i] + 1];

				while (
					max(i, j) + _len < int(text.length())
					&&
					text[i + _len] == text[j + _len])
				{
					_len++;
				}

				LCP[R[i]] = _len;

				if (_len > 0)
					_len--; 
			}
		}
	}

};
