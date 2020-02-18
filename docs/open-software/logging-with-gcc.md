# Logging with gcc

Usually it’s one of the first things we need to set up - some logging functions so we can see what our program’s doing!

Here’s some useful information regarding doing just that. Like a cheat sheet - only shorter!

## Standard Pre-Defined macros

GCC Defines several macros that are useful when logging. They include:

`__FILE__` = A null terminated string of type char[] which is set to the currently compiling source code filename.

`__func__` = A null terminated string of type char[] which is set to the current function name context.

`__LINE__` = An integer which is always the current line number of the source code currently being compiled.

It’s worth noting quickly, that we can define macros which include these macros in a header file somewhere, `__FILE__` and `__LINE__` will be correctly set where we use our wrapper macro – i.e. they will not always be set to the same filename and linenumber of the header file where we defined our macro.

## Variable Argument Macros

GCC Allows us to define variable argument macros – also really handy for logging. Remembering the above, we could always define a macro, something like:

```c
#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define DBG(x, ...) fprintf( stdout, "%s: " x "\n", "DBG", ##__VA_ARGS__)

#endif
```

This works well, it means we can just add this to our code. It takes advantage of the fact that string constants placed next to each other are concatenated by the compiler.

```c
/* ... */
DBG( "This was an error! %s", errorString );
/* ... */
```

But when we want that error message to show us where we will find it in the source code, we can define debug differently:

```c
#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define DBG(x, ...) fprintf( stdout, "%s:%s:%d: " x "\n", "DBG", __func__, __LINE__, ##__VA_ARGS__)

#endif
```

So now we have a pretty good debug definition. However, the usual approach to building software projects is to have a debug build where this debug information is printed, and a release version where this debug information is not printed. For the best performing release version we ideally just want DBG() to be blank. So we can test for a DEBUG definition and choose whether to use a functional DBG() or not:

```c
#ifndef LOG_H
#define LOG_H

#if DEBUG

#include <stdio.h>

#define DBG(x, ...) fprintf( stdout, "%s:%s:%d: " x "\n", "DBG", __func__, __LINE__, ##__VA_ARGS__)

#else

#define DBG(x, ...)

#endif
#endif
```

There are more problems, we’re not done yet! Consider the common C programming idiom:

```c
if( something )
    DBG("Something should really be nothing, but it's %d", something);
```

free( buffer );
When this is compiled in debug mode (i.e. DEBUG is defined), everything works fine because DBG compiles to a C statement. However, in release mode DBG() is compiled to nothing and we end up with a memory leak because the code in release mode becomes:

```c
if( something )

free( buffer );
```

Definitely not what we intended! Some may say that this is a lesson learnt in always putting code block sharp braces after an if statement. I disagree with that! What it does teach us is that textual substitution by the C pre-processor should be considered carefully! Using sharp braces to block a single statement after an if statement makes the code much less readable for me. So we end up modifying the code to the following definitions in the header:

```c
#ifndef LOG_H
#define LOG_H

#if DEBUG

#include <stdio.h>

#define DBG(x, ...) { fprintf( stdout, "%s:%s:%d: " x "\n", "DBG", __func__, __LINE__, ##__VA_ARGS__) }

#else

#define DBG(x, ...) { }

#endif

#endif
```

The more observant (perhaps sad?) people will observe that now we’re inserting a semi-colon after a code block closing sharp bracket. But this is totally legal to end a statement. So with this version, we shoudl br all good to go?

Not Quite!

There’s a place where a semi-colon after a block breaks compilation, and that’s in an if…else block. A semi-colon cannot be between the end of an if block statement and an else statement. So the following will break:

```c
if( something )
    DBG( "Opps, there'll be an error here!" );
else
    DBG( "Not Something!" );
```

There is a quick trick we can perform to get around this, ensuring that having the semi-colon at the end of the line will always perform the same in all situations, and that’s enclosing everything in a `do...while` loop:

```c
#ifndef LOG_H
#define LOG_H

#if DEBUG

#include <stdio.h>

#define DBG(x, ...) do { fprintf( stdout, "%s:%s:%d: " x "\n", "DBG", __func__, \
                __LINE__, ##__VA_ARGS__) } while (0)

#else

#define DBG(x, ...) do { } while(0);

#endif

#endif
```

The `do...while` loop guarantee’s at least one execution of the statement block. The compiler should drop the empty statement block as there’s nothing to do. The one with the logging functionality should also boil down through the compiler to just the single fprintf call with no further checking around it. Otherwise, the compiler is not very optimal! This solution works in all cases and should allow us to write seamless debug printing code.

Now you can add different logging levels and perhaps an error macro which will log to stderr instead of stdout.
