# Maximum empty rectangle
Find a maximum area rectangle that is fully contained in rectangle A and does not contain any point of S in its interior.

The code was written for the sake of my [Bachelor thesis](thesis.pdf).

There is implementation and evaluation of the algorithms from the following publications:
* A. Naamad, D. T. Lee and W. L. Hsu, ”On the maximum empty rectangle problem”, Discrete Appl. Math.,8,1984, pp. 267-277.
* Subhas C. Nandy, Bhargab B. Bhattacharya, and Sibabrata Ray. 1990. Efficient algorithms for identifying all maximal isothetic empty rectangles in VLSI layout design. In Proceedings of the tenth conference on Foundations of software technology and theoretical computer science (FST and TC 10), Kesav V. Nori and C. E. Veni Madhavan (Eds.). Springer-Verlag New York, Inc., New York, NY, USA, 255-269.

### Usage
```
sh tools/prepare_tests.sh ./tests
sh tools/test_algos.sh ./tests
```
### Programs
* `naamad` - O(N^2) algorithm from chapter 3.

* `naamad_improved` - O(S*logN) algorithm from chapter 3.

* `nancy` - O(S+NlogN) algorithm from chapter 6.

* `gentest`, `judge` - described in chapter 4.

`rect_to_area` - helper program used for generating tests, calculates area of a rectangle based on its coordinates.

### Tools - useful scripts

* `evaluate.sh` - takes algorithm name (`naamad`, `naamad_improved` or `nancy`) and path to the tests. Save found rectangles to the `tmp` file. Prints useful infomation such as execution time or number of so-called `RRs` considered.

* `generate.sh` - takes number of tests to generate, path to the output directory, size of the bounding rectangle, number of interior points and method of its generating (0, 1 or 2).

* `prepare_tests.sh` - takes path to the output directory. For each method of generating interior points it creates 1000 tests with `N=100`, 100 tests with `N=10^4` and 10 tests with `N=10^6`.

* `test_algos.sh` - takes path to the directory with tests. It evaluates `naamad` on small and medium tests, and `naamad_improved` and `nancy` on all tests.
