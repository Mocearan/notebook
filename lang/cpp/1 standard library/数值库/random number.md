# random number

----



## PRNG：**Pseudo-random number generators**

​		A **pseudo-random number generator (PRNG)** is an algorithm that generates a sequence of numbers whose properties simulate a sequence of random numbers.

​		It’s easy to write a basic PRNG algorithm. Here’s a short PRNG example that generates 100 16-bit pseudo-random numbers:

```c++
#include <iostream>

// For illustrative purposes only, don't use this
unsigned int LCG16(unsigned int seed = 5323) // our PRNG
{
    static unsigned int state{ seed };

    // Generate the next number

    // Due to our use of large constants and overflow, it would be
    // hard for someone to casually predict what the next number is
    // going to be from the previous one.
    state = 8253729 * state + 2396403; // first we modify the state

    return state % 32768; // then we use the new state to generate the next number in the sequence
}

int main()
{
    // Print 100 random numbers
    for (int count{ 1 }; count <= 100; ++count)
    {
        std::cout << LCG16() << '\t';

        // If we've printed 10 numbers, start a new row
        if (count % 10 == 0)
            std::cout << '\n';
    }

    return 0;
}
```

​		 this particular algorithm isn’t very good as a random number generator. But most PRNGs work similarly to `LCG16()` -- they just typically use more state variables and more complex mathematical operations in order to generate better quality results.
​		

​		In order to be a good PRNG, the PRNG needs to exhibit a number of properties:

- The PRNG should generate each number with approximately the same probability.

  ​	This is called distribution uniformity.

- The method by which the next number in the sequence is generated shouldn’t be predictable.

- The PRNG should have a good dimensional distribution of numbers.

- The PRNG should have a high period for all seeds

- The PRNG should be efficient

​		

​		Over the years, many different kinds of PRNG algorithms have been developed (Wikipedia has a good list [here](https://en.wikipedia.org/wiki/List_of_random_number_generators)). Every PRNG algorithm has strengths and weaknesses that might make it more or less suitable for a particular applications, so selecting the right algorithm for your application is important.



## seed

​		When a PRNG is instantiated, an initial value (or set of values) called a **random seed** (or **seed** for short) can be provided to initialize the state of the PRNG.		

​		All of the values that a PRNG will produce are deterministically calculated from the seed value(s).

​		the size of the seed value can be smaller than the size of the state of the PRNG, we say the PRNG has been **underseeded**.

​		Only seed a given pseudo-random number generator once, and do not reseed it.

> ​		Many PRNGs can be reseeded after the initial seeding. This essentially re-initializes the state of the random number generator, causing it to generate results starting from the new seed state. Reseeding should generally be avoided unless you have a specific reason to do so, as it can cause the results to be less random, or not random at all.

```c++
#include <iostream>
#include <random>

int getCard()
{
    std::mt19937 mt{ std::random_device{}() }; // this gets created and seeded every time the function is called
    std::uniform_int_distribution card{ 1, 52 };
    return card(mt);
}

int main()
{
    std::cout << getCard();

    return 0;
}
```

​		A better option in most cases is to create a global random number generator (inside a namespace!).

```c++
#include <iostream>
#include <random> // for std::mt19937 and std::random_device

namespace Random // capital R to avoid conflicts with functions named random()
{
	std::mt19937 mt{ std::random_device{}() };

	int get(int min, int max)
	{
		std::uniform_int_distribution die{ min, max }; // we can create a distribution in any function that needs it
		return die(mt); // and then generate a random number from our global generator
	}
}

int main()
{
	std::cout << Random::get(1, 6) << '\n';
	std::cout << Random::get(1, 10) << '\n';
	std::cout << Random::get(1, 20) << '\n';

	return 0;
}
```



​		

## **Randomization in C++**

​		The randomization capabilities in C++ are accessible via the `<random>` header of the standard library. Within the random library, there are 6 PRNG families available for use (as of C++20):

![image-20220603182124764](https://raw.githubusercontent.com/Mocearan/picgo-server/main/image-20220603182124764.png)



​		There is zero reason to use `knuth_b`, `default_random_engine`, or `rand()` (which is a random number generator provided for compatibility with C).

As of C++20, the Mersenne Twister algorithm is the only PRNG that ships with C++ that has both decent performance and quality.

> A test called [PracRand](http://pracrand.sourceforge.net/) is often used to assess the performance and quality of PRNGs (to determine whether they have different kinds of biases). You may also see references to SmallCrush, Crush or BigCrush -- these are other tests that are sometimes used for the same purpose.
>
> If you want to see what the output of Pracrand looks like, [this website](https://arvid.io/2018/06/30/on-cxx-random-number-generator-quality/) has output for all of the PRNGs that C++ supports as of C++20.

​		For most applications, Mersenne Twister is fine, both in terms of performance and quality.

> However, it’s worth noting that by modern PRNG standards, Mersenne Twister is [a bit outdated](https://en.wikipedia.org/wiki/Mersenne_Twister#Disadvantages). The biggest issue with Mersenne Twister is that its results can be predicted after seeing 624 generated numbers, making it non-suitable for any application that requires non-predictability.

​		If you are developing an application that requires the highest quality random results (e.g. a statistical simulation), the fastest results, or one where non-predictability is important (e.g. cryptography), you’ll need to use a 3rd party library.

> - The [Xoshiro family](https://prng.di.unimi.it/) and [Wyrand](https://github.com/wangyi-fudan/wyhash) for non-cryptographic PRNGs.
> - The [Chacha family](https://cr.yp.to/chacha.html) for cryptographic (non-predictable) PRNGs.



## **C++ using Mersenne Twister**

​		Although it is a bit old by today’s standards, it generally produces quality results and has decent performance.

​		The random library has support for two Mersenne Twister types:

- `mt19937` is a Mersenne Twister that generates 32-bit unsigned integers
- `mt19937_64` is a Mersenne Twister that generates 64-bit unsigned integers

```c++
#include <iostream>
#include <random> // for std::mt19937

int main()
{
    std::mt19937 mt; // Instantiate a 32-bit Mersenne Twister

    // Print a bunch of random numbers
    for ( int count { 1 }; count <= 40; ++count ) {
        std::cout << mt() << '\t'; // genetratea random number

        // If we've print5ed 5 numbers, start a new row
        if ( count % 5 == 0 ) std::cout << '\n';
    }
    return 0;
}
```



​		A **random number distribution** converts the output of a PRNG into some other distribution of numbers.

​		 a **uniform distribution** is a random number distribution that produces outputs between two numbers X and Y (inclusive) with equal probability.

```c++
#include <iostream>
#include <random> // for std::mt19937 and std::uniform_int_distribution

int main()
{
    std::mt19937 mt;

    // Create a reusable random number generator that generates uniform numbers between 1 and 6
    //! std::uniform_int_distribution die6 { 1,6 }; // for c++14, use std::uniform_init_distribution<> die6{1,6};
    std::uniform_int_distribution<> die6 { 1,6 };

    // Print a bunch of random numbers
    for ( int count { 1 }; count <= 40; ++count ) {
        std::cout << die6( mt ) << '\t'; // generate a roll of the die here

        // If we've printed 10 numbers, start a new row
        if ( count % 10 == 0 ) std::cout << '\n';
    }

    return 0;
}
```

​		Because we are default initializing our Mersenne Twister, it is being initialized with the same seed every time the program is run. And because the seed is the same, the random numbers being generated are also the same.

​		In order to make our entire sequence randomized differently each time the program is run, we need to pick a seed that’s not a fixed number. 

​		we just need to pick something that changes each time the program is run. Then we can use our PRNG to generate a unique sequence of pseudo-random numbers from that seed.

There are two methods that are commonly used to do this:

- Use the system clock
- Use the system’s random device



### seeding with the system clock

​		 C and C++ have a long history of PRNGs being seeded using the current time (using the `std::time()` function), so you will probably see this in a lot of existing code.

​		Fortunately, C++ has a high resolution clock that we can use to generate a seed value. This time is measured in “ticks”, which is a very small unit of time (usually nanoseconds, but could be milliseconds).

```c++
#include <iostream>
#include <random>    // for std::mt19937
#include <chrono> // for std::chrono

int main()
{
    // Seed out Mersenne Twister using the
    std::mt19937 mt { static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count() ) };

    // Create a resuable random number generator that generates uniform numbers between 1 and 6
    //! std::uniform_int_distribution die6 { 1, 6 }; // for C++14, use std::uniform_init_distribution<> die6{1,6};
    std::uniform_int_distribution<> die6 { 1,6 };

    // Print a bunch of random numbers
    for ( int count { 1 }; count <= 40; ++count ) {
        std::cout << die6( mt ) << '\t'; // generate a roll of the die here

        // If we've printed 10 numbers, start a new row
        if ( count % 10 == 0 ) std::cout << '\n';
    }
    return 0;
}
```

> `std::chrono::high_resolution_clock` is a popular choice instead of `std::chrono::steady_clock`. `std::chrono::high_resolution_clock` is the clock that uses the most granular unit of time, but it may use the system clock for the current time, which can be changed or rolled back by users. `std::chrono::steady_clock` may have a less granular tick time, but is the only clock with a guarantee that users can not adjust it.



### seeding with the random device

​		The random library contains a type called `std::random_device` that is an implementation-defined PRNG.

> ​		Normally we avoid implementation-defined capabilities because they have no guarantees about quality or portability, but this is one of the exception cases.
>
> ​		Typically `std::random_device` will ask the OS for a random number (how it does this depends on the OS).

```c++
#include <iostream>
#include <random> // for std::mt19937 and std::random_device

int main()
{
    std::mt19937 mt { std::random_device{} ( ) };

    //Create a reusable random number generatoer that generates uniform numbers between 1 and 6
    //! std::uniform_int_distribution die6 { 1,6 }; // for c++14, use std::uniform_int_distribution<> dit6{1,6};
    std::uniform_int_distribution<> die6 { 1,6 };

    // Print a bunch of random numbers
    for ( int count { 1 }; count <= 40; ++count ) {

        std::cout << die6 << '\t'; // generate a rall of the die here
        // If we've printed 10 numbers, start a new row
        if ( count % 10 == 0 ) std::cout << '\n';
    }
    return 0;
}
```

​		One potential problem with `std::random_device`: it isn’t required to be non-deterministic, meaning it *could*, on some systems, produce the same sequence every time the program is run, which is exactly what we’re trying to avoid. There was a [bug in MinGW](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85494) (fixed in GCC 9.2) that would do exactly this, making `std::random_device` useless.

​		**Use `std::random_device` to seed your PRNGs (unless it’s not implemented properly for your target compiler/architecture).**

> Q: If std::random_device is random itself, why don’t we just use that instead of Mersenne Twister?
>
> Because std::random_device is implementation defined, we can’t assume much about it. It may be expensive to access or it may cause our program to pause while waiting for more random numbers to become available. The pool of numbers that it draws from may also be depleted quickly, which would impact the random results for other applications requesting random numbers via the same method. For this reason, std::random_device is better used to seed other PRNGs rather than as a PRNG itself.



**Mersenne Twister and underseeding issues**

​		The internal state of a Mersenne Twister is 624 bytes in size. In the examples above, where we seed from the clock or std::random_device, our seed is only a single 32-bit integer. This means we’re essentially initializing a 624-byte object with a 4-byte value, which is significantly underseeding the Mersenne Twister PRNG. The random library does the best it can to fill in the remaining 620 bytes with “random” data… but it can’t work magic. 

​		Underseeded PRNG can generate results that are suboptimal for applications that need the highest quality results. For example, seeding `std::mt19937` with a single 32-bit value will never generate the number `42` as its first output.

​		So how do we fix this? As of C++20, there’s no easy way. But we do have some suggestions.

- First, let’s talk about `std::seed_seq` (which stands for “seed sequence”).

  ​		 we mentioned that a seed can be either a single value, or a set of values. `std::seed_seq` is a type that performs two functions.

  - First, it can hold multiple seed values, so we can use it to seed our PRNG with more than one value. 

  - Second, it will generate as many additional unbiased seed values as needed to initialize a PRNG’s state. 

    ​	So if you initialize `std::seed_seq` with a single 32-bit integer (e.g. from std::random_device) and then initialize a Mersenne Twister with the `std::seed_seq` object, `std::seed_seq` will generate 620 bytes of additional seed data. The results won’t be amazingly high quality, but it’s better than nothing.

  

  ```c++
  // First, the more pieces of random data we can give std::seed_seq to work with, the better. So the easiest idea is to simply use std::random_device to give std::seed_seq more data to work with. If we initialize std::seed_seq with 8 numbers from std::random_device instead of 1, then the remaining numbers generated by std::seed_seq should be that much better:
  
  std::random_device rd;
  std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() }; // get 8 integers of random numbers from std::random_device for our seed
  std::mt19937 mt{ ss }; // initialize our Mersenne Twister with the std::seed_seq
  
  /Second, you can use other “random” inputs to `std::seed_seq`. 
  ```

- An alternate path is to use a different PRNG with a smaller state. 

  ​	Many good PRNGs use 64 or 128 bits of state, which can easily be initialized using `std::seed_seq` filled with 8 calls to `std::random_device`.