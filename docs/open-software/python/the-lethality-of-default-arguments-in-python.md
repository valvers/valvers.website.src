# The Lethality of Default Arguments in Python

One of Python's many, many features is the ability to give function and method arguments default values. This means that if a caller doesn't provide a value for one of the parameters, a default value will be used.

But, programmer, beware the beast that is the Python interpreter for it can get you in a tangle with some rather unwanted results. Luckily, we have some basic rules we can use for default arguments allow us to avoid this bug minefield.

The quick-fire solution? Only EVER use immutable default arguments, examples are `None, (), True, False,` etc. To understand the problem, read on...

## The Problem

The problem is that the Python interpreter only creates the default value ONCE on initial evaluation of the function or method. The default argument is NOT evaluated for every function or method call.

Normally, this is not a problem. But it becomes a problem when the modify the value of a default argument. As the reference to the default value never changes, if it is a mutable type such as an empty list or dictionary, subsequent function or method calls will result in the modified version becoming the default value.

Let's see an example problem with a rather inoxuous function:

```python
from typing import List

def add(a: int, b: int, values: List[int] = []):
    values.append(a)
    values.append(b)
    return sum(values)

print("1+1 = {}".format(add(1, 1)))
print("1+1 = {}".format(add(1, 1)))
```

Here we think we've been pretty clever - we've defined this function add that requires two positional arguments and then can accept a list of additional values. All arguments are then summed and returned by the `sum` function.

This mistake we've made here is modifying the default value of `values` which we'd defined to be an empty list.

To see the problem, let's have a look at the output of this script:

```
1+1 = 2
1+1 = 4
```

Something is definitely wrong. In fact, when we call the function the second time, the call signature actually looks like the following:

```python
add(a=1, b=1, values = [1, 1])
```

Hence, we get the result of 4 for the call of `add(1, 1)`.

> **NOTE:* This is also true if you specifiy the `list()` callable as the default argument!

Let's have a look at a few others to see if this is true for other types:

### bool

```python
from typing import List

def add(a: int, b: int, values: List[int] = [], flag: bool = False):
    values.append(a)
    values.append(b)

    if sum(values) == 2:
        flag = True

    return sum(values), flag

print("1+1 = {}".format(add(1, 1)))
print("1+1 = {}".format(add(1, 1)))

# Output

# 1+1 = (2, True)
# 1+1 = (4, False)
```

The default value of a bool is changed in the first call, but in the second the default value is used and is correct according to the function signature. So `bool` and other simple, primitive types are safe.

### dict

```python
from typing import List

def add(a: int, b: int, values: List[int] = [], d: dict = {}):
    values.append(a)
    values.append(b)

    if sum(values) == 2:
        d["bool"] = True

    return sum(values), d

print("1+1 = {}".format(add(1, 1)))
print("1+1 = {}".format(add(1, 1)))

# Output

# 1+1 = (2, {'bool': True})
# 1+1 = (4, {'bool': True})
```

The dict type is changed in the first call and this changed value persists across multiple calls. So empty dictionaries must not be used.

### Mutliple Functions and Methods

Each unique function signature is evaluated separately and therefore results in a different object. For example, the default value in the two functions `add` and `subtract` below are not connected together. Individual function suffer the problem, but it does not cross functions.

```python
from typing import List

def add(a: int, b: int, values: List[int] = [], d: dict = {}):
    values.append(a)
    values.append(b)

    if sum(values) == 2:
        d["add"] = True

    return sum(values), d

def subtract(a: int, b: int, values: List[int] = [], d: dict = {}):
    values.append(-a)
    values.append(-b)

    if sum(values) == -2:
        d["sub"] = True

    return sum(values), d

print(" 1+1 = {}".format(add(1, 1)))
print(" 1+1 = {}".format(add(1, 1)))

print("-1-1 = {}".format(subtract(1, 1)))
print("-1-1 = {}".format(subtract(1, 1)))

# Output

#  1+1 = (2, {'add': True})
#  1+1 = (4, {'add': True})
# -1-1 = (-2, {'sub': True})
# -1-1 = (-4, {'sub': True})
```

If there were a problem here, the dictionary value from the first subtract would have an `add` key with the value `True`.


## The Solution

The solution here is to never use mutable values as default arguments. Always stick with `None` (An immutable object) or else Tuples. For example our earlier function signature could be changed to have values set as a `tuple`. Tuples are however immutable, so our function would also have to change in order to have the original functionality:

```python
def add(a: int, b: int, values: tuple = ()):
    values = list(values)
    values.append(a)
    values.append(b)
    return sum(values)

print("1+1 = {}".format(add(1, 1)))
print("1+1 = {}".format(add(1, 1)))

# Output

# 1+1 = 2
# 1+1 = 2
```

Sticking with None is my typical favourite because we can explicitly test for None in a Pythonic way within the function= with `if values is not None:`. But, this can add more lines of code to the start of our function than the immutable tuple method.

This whole problem is why you see many libraries starting to move to tuples and/or None as default arguments.

Keep your eyes peeled for these problematic default arguments! Otherwise one day they'll bite you hard.
