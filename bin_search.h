// O(log(n)) -- optimal
// given sorted vector, find index of element equals value
void BinarySearch(std::vector<int> v, int value) {
    // if size == 1???
    size_t left = 0, right = v.size();
    size_t mid = (right - left) / 2;
}
// check good impl in cppref.com

// find minimal i, s.t. x[i] >= x
void LowerBound(std::vector<int> v, int x) {}

// O(k), k -- index of answer
void LinearSearch(std::vector<int> v, int x) {}

// O(log(k)), k -- index of answer
// efficient if answer close to left side, in this case it is better than LinearSearch
// use if k << n
void GallopingBinarySearch() {}