


## Score: 0/0 (0.00%)


### ✗ - [0/0] - Output from `make`

- **Points**: 0 / 0

```
test.cpp:185:1: fatal error: error writing to /tmp/cc45O0gF.s: No space left on device
  185 | }
      | ^
compilation terminated.
make: *** [test] Error 1

```
```
gcc -I. -W -Wall -g -pthread msort.c -o msort
gcc -I. -W -Wall -g gen.c -o gen
g++-9 test.cpp -o test
Makefile:19: recipe for target 'test' failed

```


---

This report was generated for **renzol2** using **e4e8dc0875426faef44a1e0393e989fe4621acc6** (from **March 9th 2021, 2:00:00 am**)
