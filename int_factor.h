//collection of integer factorization algorithms

//returns vector of prime factors. prime factors may repeat
std::vector<int> trial_division(int n) {
    std::vector<int> ans;
    int p = 2;
    while (n > 1) {
        if (n % p == 0) {
            n /= p;
            ans.push_back(p);
        } else {
            ++p;
        }
    }
    return ans;
}
