# Hash

Here we are trying to find out how different optimizations and
implementations effect on speed of hash table queries

## Hash tables

Of course, if you want to optimize smth, you need to understand how it works. Hash table with chain method (the one I use) is a structure where some Hash function gives for each your key some value from the given range - size of hash table. Then for big array of data we have Linked list of nodes with keys for each value from the range that allows us to find one faster. The case when two values are in one bucket (have the same value after hashing) we will call a collision

#### Load Factor
Also we will need to know what Load Factor (LF) really is.
For chain method table it shows mean of the number of words in one chain and can be  computed by: $\frac{DistinctWords}{SizeOfTable}$

> #### For example:
> If you want to hash an array 1, 5, 7, 8 with hash function (mod 4) you will get table
> 0: 8
> 1: 1 <--> 5
> 2:
> 3: 7

You can read more about Hash Tables [here](https://en.wikipedia.org/wiki/Hash_table)

## Experiment

We are loading hash table from file and making thousands/millions of queries to it.

As the source for my hash table, I took The Lord Of The Rings. Here you can find actual source in `txtsrc/input.txt`.

#### Preprocessing
Then I decided to use word alignment for accelerating my algorithm. I used processing program which you can run with
`make proc`
After that you will find a new file `table/output.txt` in which ready data is located. There words are in order of increasing hash with already counted number of its occurences in initial text. This is made for faster load of hash table on tests.

The asymptotic complexity of the algorithm O(NumOfQueries*LoadFactor) but our
project aims to find implementation with the best constant though
we are overloading the table (LF > 10) to find out which functions
are need to be accelerated

Here I have about 15 000 distinct words so for hash table of size 1024 we will have LF about 15.

#### Usage Case

Here to implement real usage case I do $10^6$ queries from the buffer with words, where I do not use alignment as in reality

We are considering different optimizations such as:

- vectorizations with Intrinsics
- assembler implementation
- logical optimizations

### Run

To compile and run program use
`make run` command

To switch between given optimizations, you will need to uncomment
```
# CC_FLAGS += -D NOSTRCMP
# CC_FLAGS += -D NOHASH
# CC_FLAGS += -D NOSTRCPY
```
one of these flags

## Experiment flow

After the first run, I got such a profiler output:
![pic 1](images/1.png)

So I decided to improve strcmp, in order with the fact that I have alignment it became easy to copmpare strings without any cycles

After done optimizations we can see
![pic 2](images/2.png)

Then we can see that slowhash function is also could be improved because xmm registers allow s to hash whole word in one. Then we get

![pic 3](images/3.png)

As we can see the next function that I can try to improve is strcpy.

But it only got a bit worse so, I decided to stop optimizing

## Methods

> ### Hash
> We are using 2 implementations of computing hash
> which you can find in `src/hash.cpp` they are `slowhash` and
> `xxh3_hash` with `nonaligned_hash`
>
> ##### slow
> Simple implementation with usage of byte-by-byte hashing
>
> ##### faster
> Here we are using instructions set AVX-512 for hashing all word
> with one instruction moreover we have words in our initial file aligned as 32
> bytes which is simples our further work
>
> ### Strcmp
> You can find my function in `src/mystrcmp.s`
>
> ##### slow
> Original one
>
> ##### fast
> Here we are winning due to the fact that one of our strings is always
> aligned as 32 bytes so we can use its structure to void any computing
> of string length
>
> ### Strcpy
> You can find my implementation in `src/hash.cpp`

## Experimental setup

Testing machine:
    RAM : 16GB
    OS  : Windows 11 Home with usage of archlinux WSL for profiling
    CPU:
    ![MachineCPU](images/image.png)
    Compiler: gcc (GCC) 15.1.1 20250425

> ### Test results
> We are using perf for profile and estimate our program
> especially
> `perf stat` for time measurement
> `perf record -g` and `perf report --sort=dso,symbol` for finding the narrowest places of program
>
> | Optimizations  | perf res                                |
> |--------------- |-----------------------------------------|
> |No optimizations| 20.6118  +- 0.0359 seconds  ( +- 0.98% )   |
>  |Strcmp          | 0.4398 +- 0.0199 seconds ( +- 4.13% ) |
> |Hash            | 0.06677 +- 0.00133 seconds  ( +- 1.99% )|
> |Strcpy          |  0.07486 +- 0.00410 seconds ( +- 5.48% )|
>
> As We needed to provide 3 different implementations, We tried to improve
> the `strcpy` function but as we can see it did not bear fruit.
> In our opinion it happens so because -O3 optimizations can load word
> into xmm register earlier due to the fact that it controls all the
> calling between the functions and that it considers all intrinsics
> function as inline ones




