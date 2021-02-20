


## Score: 14/15 (93.33%)


### ✓ - [0/0] - Output from `make`

- **Points**: 0 / 0


```
g++-9 -x c add_int.c -c -o add_int.o
g++-9 -x c capitalize.c -c -o capitalize.o
g++-9 -x c emoji.c -c -o emoji.o
g++-9 test.cpp -c -o test.o -fpermissive -w -DPART1 -DPART2 -DPART3
g++-9 add_int.o capitalize.o emoji.o test.o -o test

```


### ✓ - [1/1] - test_add_ints

- **Points**: 1 / 1





### ✓ - [1/1] - test_add_intMixed

- **Points**: 1 / 1





### ✓ - [1/1] - test_add_intPtrs

- **Points**: 1 / 1





### ✓ - [1/1] - test_add_ints_alloc

- **Points**: 1 / 1





### ✗ - [0/1] - test_capitalize_first_letter_lowercase

- **Points**: 0 / 1


```
Original: strcmp(s, "Hello!") == 0
Expanded: 32 == 0
```


### ✓ - [1/1] - test_capitalize_all_letters_uppercase

- **Points**: 1 / 1





### ✓ - [1/1] - test_capitalize_all

- **Points**: 1 / 1





### ✓ - [1/1] - test_capitalize_all_complex

- **Points**: 1 / 1





### ✓ - [1/1] - test_capitalize_all_alloc

- **Points**: 1 / 1





### ✓ - [1/1] - test_emoji_favorite

- **Points**: 1 / 1





### ✓ - [1/1] - test_emoji_count_basic

- **Points**: 1 / 1





### ✓ - [1/1] - test_emoji_count_complex

- **Points**: 1 / 1





### ✓ - [1/1] - test_emoji_invert_char

- **Points**: 1 / 1





### ✓ - [1/1] - test_emoji_invert_all

- **Points**: 1 / 1





### ✓ - [1/1] - test_emoji_random_alloc

- **Points**: 1 / 1





---

This report was generated for **renzol2** using **d132015fa3942eb9bf0a9359a7dfda033e3f77bc** (from **February 16th 2021, 1:00:00 am**)
