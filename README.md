### benchmark:
if u want to pick the best compression utility for your case you should consider these key points:

* ..
* ...

### known candidates:
* gzip
* zstd
* zip
* 7zip
* bz
* tar
* ??

### todo:

* move code from here to dedicated algo repo
* write script which will run some benchmarks to compare time/ratio of different archivers on given data


## old info:

### build

```
mkdir build
cd build
cmake ../
make
```

### usage

Run `./test_lzw` without arguments to see help message. Same works with other binaries.

Generate compressed file:
```
./test_lzw -c ../tests/data/input ../tests/data/compressed
```

Search in compressed file:
```
./test_slg_search ../tests/data/compressed ../tests/data/patterns
```

### some refs

Collection of some compression algorithms:

[LZ77](https://www2.cs.duke.edu/courses/spring03/cps296.5/papers/ziv_lempel_1977_universal_algorithm.pdf)

[LZW](https://en.wikipedia.org/wiki/Lempel-Ziv-Welch)
