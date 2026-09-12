# CPP09 — Notes

## Overview

CPP09 is mainly about **STL containers, algorithms, and different ways of solving the same problem**.

The exercises focus on:

- Choosing the right STL container
- Understanding how containers behave
- Using standard algorithms
- Parsing and validating input
- Working with different container types
- Understanding Bitcoin exchange-rate calculations
- Implementing the Ford–Johnson / merge-insertion sorting algorithm
- Measuring performance

---

# Exercise 00 — Bitcoin Exchange

## Main Concept

The goal is to build a program that takes a date and a Bitcoin amount, then calculates the corresponding value using historical Bitcoin prices.

The important part is not really Bitcoin itself.

The exercise teaches how to:

- Read data from a file
- Parse text
- Validate input
- Store data in an STL container
- Search for the correct value
- Handle dates
- Deal with edge cases

---

## Data

There are two important pieces of information:

### Database

A database contains historical Bitcoin prices.

Conceptually:

```text
date → exchange rate
````

For example:

```text
2011-01-03 → price
2011-01-04 → price
2011-01-05 → price
```

The program needs to find the appropriate exchange rate for a requested date.

---

## Date Lookup

The requested date might not exist exactly in the database.

In that case, use the **closest previous date**.

Example:

```text
Database:

2020-01-01
2020-01-05
2020-01-10

Request:

2020-01-07
```

There is no entry for January 7.

The correct rate comes from:

```text
2020-01-05
```

because it is the most recent date that is not after the requested date.

---

## STL Container

A container such as `std::map` is useful here because it stores:

```text
key → value
```

In this exercise:

```text
date → Bitcoin price
```

A `map` also keeps its keys sorted.

This makes finding the closest previous date possible using the container's ordering.

---

## Input Validation

There are several things to validate.

### Date

The date should follow the expected format:

```text
YYYY-MM-DD
```

Need to consider:

* Correct number of characters
* Correct separators
* Valid month
* Valid day
* Reasonable calendar date

---

### Bitcoin Amount

The amount must be valid.

Things such as:

* Negative values
* Invalid characters
* Values outside the allowed range
* Invalid decimal formats

need to be handled.

---

## Important Lesson

This exercise is mainly about:

> **Parsing + validation + ordered lookup**

The interesting STL concept is that the container can do part of the searching work for us.

---

# Exercise 01 — RPN

## Main Concept

RPN means:

**Reverse Polish Notation**

Instead of writing:

```text
3 + 4
```

RPN writes:

```text
3 4 +
```

The operator comes **after** the operands.

---

## Why RPN?

RPN removes the need for:

* Parentheses
* Operator precedence rules
* Traditional expression parsing

The expression is evaluated from left to right using a stack.

---

## Stack

The main data structure is a:

```text
stack
```

A stack follows:

**LIFO**

> Last In, First Out

Think of a stack of plates.

The last plate placed on the stack is the first one removed.

---

## RPN Process

For each token:

### Number

Put it onto the stack.

```text
number → push
```

### Operator

Take the top two numbers from the stack.

Apply the operator.

Put the result back onto the stack.

Conceptually:

```text
stack:

A
B

operator

↓

B operator A

↓

result pushed back
```

The order matters for:

* Subtraction
* Division

---

## Example

Expression:

```text
3 4 +
```

Start:

```text
[]
```

Read `3`:

```text
[3]
```

Read `4`:

```text
[3, 4]
```

Read `+`:

```text
3 + 4
```

Result:

```text
[7]
```

---

## Invalid Expressions

The program needs to detect things such as:

* Too many operators
* Not enough operands
* Invalid characters
* Invalid numbers
* Division by zero
* Extra values remaining at the end

A valid RPN expression should finish with:

```text
exactly one value on the stack
```

---

## Important Lesson

This exercise teaches:

> **Using a container to model a specific algorithm**

The stack is not just storage.

Its behavior is part of the algorithm.

---

# Exercise 02 — PmergeMe

## Main Concept

This is the most algorithmically complicated exercise in CPP09.

The goal is to implement:

**Ford–Johnson merge-insertion sort**

using at least two different STL containers.

For example:

```text
std::vector
std::deque
```

---

# Ford–Johnson / Merge-Insertion Sort

The main idea is:

> Sort elements by creating pairs, sorting the larger elements first, then inserting the smaller elements in a special order.

The algorithm tries to reduce the number of comparisons.

---

## Step 1 — Create Pairs

Take the input two elements at a time.

Example:

```text
8 3 7 4 6 2
```

Create:

```text
(8, 3)
(7, 4)
(6, 2)
```

---

## Step 2 — Order Each Pair

Inside every pair:

```text
small, large
```

So:

```text
(8, 3)
```

becomes:

```text
(3, 8)
```

and:

```text
(7, 4)
```

becomes:

```text
(4, 7)
```

Now:

```text
(3, 8)
(4, 7)
(2, 6)
```

The important relationship is:

```text
small ↔ large
```

The two values must stay associated with each other.

---

# Step 3 — Separate Small and Large Values

From:

```text
(3, 8)
(4, 7)
(2, 6)
```

we get the large values:

```text
8 7 6
```

and the pending small values:

```text
3 4 2
```

The large values are the first part we focus on.

---

# Step 4 — Recursion

Now apply the same Ford–Johnson algorithm to the large values.

For example:

```text
8 7 6
```

becomes pairs:

```text
(7, 8)
```

with:

```text
6
```

as the leftover.

This continues recursively until the problem becomes small enough to solve directly.

---

## Why Recursion?

The original problem:

```text
sort many elements
```

is transformed into:

```text
sort fewer elements
```

Specifically, the algorithm recursively sorts the larger elements.

Once those are sorted, the smaller elements can be inserted into their correct positions.

---

# Step 5 — Build the Main Chain

After recursively sorting the large elements, they form the beginning of the sorted sequence.

For example:

```text
6 7 8
```

This is called the:

**main chain**

The smaller elements have not all been inserted yet.

They are the:

**pending elements**

---

# Step 6 — Insert the First Pending Element

The first small element is special.

For each pair:

```text
small ≤ large
```

Therefore, the first small element can be placed before its corresponding large element.

Conceptually:

```text
small₁
large₁
large₂
large₃
```

This gives the main chain a starting point.

---

# Step 7 — Jacobsthal Sequence

This is one of the most important parts of the exercise.

The pending elements are **not inserted simply from left to right**.

Ford–Johnson uses an insertion order based on the:

**Jacobsthal sequence**

The useful insertion pattern is approximately:

```text
b3, b2, b5, b4, b11, b10, b9, ...
```

The reason for this unusual ordering is to keep the binary searches small and balanced.

---

## Why Does the Order Matter?

Suppose we insert:

```text
b2
b3
b4
b5
```

in normal order.

The search ranges don't stay optimally balanced.

Ford–Johnson instead chooses an order that takes advantage of the number of elements already available.

The goal is to minimize comparisons.

---

# Step 8 — Binary Search

When inserting a pending element, we don't scan the whole chain one element at a time.

We use:

**binary search**

The important restriction is:

> A pending element should only be searched before its corresponding large element.

Example:

```text
pending value: 4
corresponding large value: 9
```

Instead of searching:

```text
entire main chain
```

we search only:

```text
from the beginning
up to 9
```

Because we already know:

```text
4 ≤ 9
```

---

# Odd Number of Elements

If the input contains an odd number of elements, one element does not have a pair.

Example:

```text
8 3 7 4 6
```

Pairs:

```text
(3, 8)
(4, 7)
```

Leftover:

```text
6
```

The leftover must eventually be inserted into the sorted main chain.

It does not have a corresponding large element, so it can be inserted using a normal binary search over the whole chain.

---

# Vector vs Deque

The exercise requires at least two containers.

For example:

```text
std::vector
std::deque
```

The important point is that the **algorithm stays conceptually the same**, but the container implementation is separate.

---

## std::vector

Characteristics:

* Contiguous memory
* Fast random access
* Very cache-friendly
* Inserting in the middle can be expensive because elements may need to move

---

## std::deque

Characteristics:

* Not stored as one contiguous block
* Fast access by index
* Efficient insertion/removal at the front and back
* Different memory organization from vector

---

# Why Implement Both Separately?

The exercise specifically encourages implementing the algorithm separately for each container.

This helps understand:

> The algorithm is independent from the container, but the implementation details depend on the container.

For example:

```text
vector algorithm
    ↓
vector operations

deque algorithm
    ↓
deque operations
```

Rather than hiding everything behind a generic template.

---

# Pair Relationship

One of the easiest things to get wrong is losing the relationship between:

```text
small
large
```

If we have:

```text
(3, 8)
```

and later sort:

```text
8 6 7
```

we need to remember that:

```text
8 ↔ 3
```

Otherwise, we won't know which small value belongs before which large value.

This is why keeping the pairs is important.

---

# Timing

The exercise also asks us to compare the performance of the two containers.

Example output:

```text
Time to process a range of 3000 elements with std::vector : ...
Time to process a range of 3000 elements with std::deque : ...
```

The timing should cover the relevant:

* Data management
* Sorting
* Container operations

---

## Debug Printing

During development, printing intermediate states is useful.

For example:

```text
pairs
large values
recursive result
pending insertion
```

But these prints should be removed before benchmarking.

Why?

Because:

```text
std::cout
```

is much slower than normal memory operations.

If thousands of debug lines are printed while the timer is running, the timing measures:

```text
sorting + terminal output
```

instead of primarily:

```text
sorting + data management
```

This can make the measured time dramatically larger.

---

# Important C++98 Concepts

CPP09 is still using:

**C++98**

So remember:

* No `auto`
* No range-based `for`
* No initializer lists
* No `std::stoi`
* No `std::stol`
* No lambdas
* No modern smart pointers

Use the older STL style:

* Iterators
* Explicit types
* `std::make_pair`
* `std::lower_bound`
* `std::find`
* `std::vector`
* `std::deque`
* `std::map`
* `std::stack`

---

# Key STL Algorithms

## `std::find`

Searches for a specific value.

Conceptually:

```text
container
    ↓
search for value
    ↓
iterator to value
```

---

## `std::lower_bound`

Used with sorted data.

Finds the first position where a value can be inserted while keeping the range sorted.

Conceptually:

```text
[1 3 5 7 9]

insert 6

↓

[1 3 5 | 7 9]
```

The insertion point is before `7`.

This is especially important in PmergeMe because pending elements are inserted using binary-search logic.

---

# Error Handling

All exercises need proper error handling.

The general idea is:

```text
detect invalid input
        ↓
throw exception
        ↓
catch in main
        ↓
print error to stderr
```

Errors should not normally be mixed with normal program output.

Normal output:

```text
std::cout
```

Errors:

```text
std::cerr
```

---

# CPP09 Big Picture

The three exercises teach different ideas.

| Exercise | Main Concept                                        |
| -------- | --------------------------------------------------- |
| ex00     | `std::map` + parsing + ordered lookup               |
| ex01     | `std::stack` + expression evaluation                |
| ex02     | `std::vector` / `std::deque` + Ford–Johnson sorting |

---

# What I Should Remember

## ex00

**Bitcoin Exchange**

Think:

```text
file
 ↓
parse
 ↓
validate
 ↓
map
 ↓
find closest previous date
 ↓
calculate
```

Main STL idea:

**ordered associative container**

---

## ex01

**RPN**

Think:

```text
number → push
operator → pop 2 → calculate → push
```

Main STL idea:

**stack**

Main algorithm idea:

**LIFO**

---

## ex02

**PmergeMe**

Think:

```text
input
 ↓
make pairs
 ↓
sort each pair: small < large
 ↓
extract large elements
 ↓
recursively Ford-Johnson-sort the large elements
 ↓
reconnect small ↔ corresponding large
 ↓
insert small elements using Jacobsthal order
 ↓
bounded lower_bound before corresponding large
 ↓
insert odd leftover
 ↓
sorted result
```

Main STL ideas:

**vector + deque**

Main algorithm idea:

**Ford–Johnson / merge-insertion sort**

---

```text
ORIGINAL
11 4 9 2 8 1 7 3 10 6 5
        │
        │ make pairs
        ▼
(4,11) (2,9) (1,8) (3,7) (6,10)
        │
        │ take large
        ▼
     11 9 8 7 10
        │
        │ RECURSION
        ▼
    (9,11) (7,8) + 10
        │
        │ take large
        ▼
       11 8
        │
        │ RECURSION
        ▼
       (8,11)
        │
        │ take large
        ▼
        11
        │
        │ only 1 number
        ▼
       STOP

    Then fall back:
        11
        │
        │ insert 8
        ▼
        8 11
        │
        │ insert 7 and 9
        ▼
        7 8 9 11
        │
        │ insert leftover 10
        ▼
        7 8 9 10 11
    
    Now Jacob algo
    We know:
    3 < 7  b1
    1 < 8  b2
    2 < 9  b3
    6 < 10 b4
    4 < 11 b5
    
    Sorted large chain:
    7 8 9 10 11
        │
        │ first small => 3
        ▼
    3 7 8 9 10 11

    Jacobsthal:
    1, 3, 5, 11, ...

    Groups:
    [b1]
    [b2 b3]
    [b4 b5]

    Insert backwards:
    b1
    b3 b2
    b5 b4

    We already inserted 1st:
        │
        ▼
    Insert b3 = 2
    2 < 9
    Search before 9
        │
        ▼
    2 3 7 8 | 9 10 11

        │
        ▼
    Insert b2 = 1
    1 < 8
    Search before 8
        │
        ▼
    1 2 3 7 | 8 9 10 11

        │
        ▼
    Insert b5 = 4
    4 < 11
    Search before 11
        │
        ▼
    1 2 3 4 7 8 9 10 | 11

        │
        ▼
    Insert b4 = 6
    6 < 10
    Search before 10
        │
        ▼
    1 2 3 4 6 7 8 9 | 10 11

        │
        ▼
    Leftover = 5
    Search entire chain
        │
        ▼
    1 2 3 4 5 6 7 8 9 10 11
```
