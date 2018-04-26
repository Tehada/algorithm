void BubbleSort(std::vector<int>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i] < v[i - 1])
            std::swap(v[i], v[i - 1]);
    }
}


// add modification, using bin_search when size of one vector is too small
// use GallopingBinarySearch here 
void Merge(std::vector<int> a, std::vector<int> b) {

}