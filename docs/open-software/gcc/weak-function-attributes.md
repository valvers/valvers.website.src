# Weak Function Attributes

In GCC it’s possible to have weak and strong function attributes. Two strongly named functions cannot be present in
code objects being linked. This would be an error produced by the linker – it doesn’t know how to resolve two symbols
with the same name. A simple example of this would be to compile two copies of the same file, which works fine, and
then try to link them – this will then fail because there are multiple definitions for the same symbol.

Weak functions are usually blank. Normally they are found in libraries that will be linked with application code.
They provide a basis for optional extended functionality. The application layer can choose to implement these
functions or not. To implement them the application layer simply defines a strong function for the name which the
linker will then link in preference to the weak named function.

Here is the GCC manual extract for the weak function attribute

> weak The weak attribute causes the declaration to be emitted as a weak symbol rather than a global. This is
> primarily useful in defining library functions that can be overridden in user code, though it can also be used with
> non-function declarations. Weak symbols are supported for ELF targets, and also for a.out targets when using the
> GNU assembler and linker

So in GCC we can define a weak function by using the following syntax

```c
#include <stdio.h>

void onKeypress(char c) __attribute__((weak))
{

}

void* KeyboardScanThread(void* args)
{
    char c;

    while(!threadQuit)
    {
        c = getch();
        onKeypress(c);
    }
}
```

Here we see an excerpt of psuedo code where the library is running a thread which continually scans the keyboard.
After each key press onKeypress is called with the character that was pressed. If this library is linked with an
application that does not have a strong onKeypress function, the blank weak function is used from the library.
Generally the optimiser will eliminate this call anyway (though in embedded versions of GCC it will make that blank
call, as I’ve seen!)

Now lets say the application does want to know when the q button has been pressed, and then we’ll quit the library.
All the application needs to do is have a strong onKeypress function. Here’s a quick bit of psuedo-code that does
that:

```c
#include <stdio.h>

void onKeypress(char c)
{
    if((c == 'q') || (c == 'Q'))
    {
        LibraryQuitNow();
    }
}

int main(int argc, char* argv[])
{
    LibraryInit();

    while(!LibraryHasQuit())
    {

    }

    return EXIT_SUCCESS;
}
```

It is important to note in the psuedo-code that we’ve introduced cross-thread complications here. It is only as I’m
typing off the top of my head to find an example. There are of course times we might want to use this in a
single-threaded environment too. Here we’re just looking at the difference between weak and strong functions and
looking at how we can use the weak function attribute in GCC.

Now the linker resolves the strong function when linking and throws the weak function away.

## Aliasing a weak function

Normally we alias all weak functions to a single blank function in GCC. This saves codespace when we have a lot of
these weak functions as they are generally all blank. To alias a function in GCC we simply add in another function
attribute, alias.

This is what the GCC manual has to say about the alias function attribute:

> `alias (“target”)`
> The alias attribute causes the declaration to be emitted as an alias for another symbol, which must be specified.
> For instance,
> ```c
> void __f () { /* Do something. */; }
> void f () __attribute__ ((weak, alias ("__f")));
> ```
> defines `f` to be a weak alias for `__f`. In C++, the mangled name for the target must be used. It is an error
> if `__f` is not defined in the same translation unit.

Not all target machines support this attribute.

In fact, as you can see weak and alias are used together so much, the alias example is the example we need in this
section of our documentation. Putting it in the context of our keyboard scanner above. If we altered the library so
that we had a callback for each character, we would end up with a lot of blank functions in the library (assuming in
this case that the optimiser didn’t get rid of the calls!). But if we do this with aliasing we can have a single
blank function which all the weak symbols will resolve too:

```c
#include <stdio.h>

void onKeyBlank(void)
{

}

void onKeyA() __attribute__((weak, alias("onKeyBlank")));
void onKeyB() __attribute__((weak, alias("onKeyBlank")));
void onKeyC() __attribute__((weak, alias("onKeyBlank")));
void onKeyD() __attribute__((weak, alias("onKeyBlank")));
void onKeyE() __attribute__((weak, alias("onKeyBlank")));
void onKeyF() __attribute__((weak, alias("onKeyBlank")));
void onKeyG() __attribute__((weak, alias("onKeyBlank")));

/*  ...  */

void* KeyboardScanThread(void* args)
{
    char c;

    while(!threadQuit)
    {
        c = getch();
        switch(c)
        {
            case 'A':
            case 'a':
                onKeyA();
            break;

            case 'B':
            case 'b':
                onKeyB();
            break;

            /*  ...  */

            default:
                OnKeyBlank(c);
                break;
        }
    }
}
```

Now our code would be slightly different in the main application code:

```c
#include <stdio.h>

void onKeyQ(void)
{
    LibraryQuitNow();
}

int main(int argc, char* argv[])
{
    LibraryInit();

    while(!LibraryHasQuit())
    {

    }

    return EXIT_SUCCESS;
}
```

As to the worth of this GCC extension, well that’s where you’ll have to do your own evaluation. I’ve used it in the
past when generating serial comms libraries in order to have events on fairly mundane errors or asserts within the
code which only really need any attention in a debug build. We can then build a version which ignores (or doesn’t
explicitly) report these errors and assertions which will be our release build, and a slower debug build which may
well take memory dumps or other diagnostic information. A conditional compile on a code module which has hooks for
these events is all we need.

It can be useful, but it can also be omitted or worked around if you don’t appreciate gcc extensions (but then this
whole page is a bit useless to you!)
