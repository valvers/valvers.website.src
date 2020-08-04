# Posix Shell Scripting

Some neat tricks and methods for scripting.

## Directory of the Script

At the start of my scripts is always the same line:

```
scriptdir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
```

Looks odd? Well, let's explain it and then you'll be enlightened. This is very portable

`CDPATH= ` Unset the `CDPATH` environment variable to make sure we don't search outside
of where we tell cd to go. If you `man cd` you'll possibly find that `CDPATH` is an
environment variable that can be used like as a search list, much like the `PATH`
environment variable. We don't want surprises.

`cd --` Change directory. The double heiphen marks the end of options input to the `cd`
command so that directories with odd names that start with a heiphen and will be mistaken
for options instead of the directory name don't cause problems.

`$()` Use a subshell - substitutes the output of the command run in the subshell

`dirname -- "$0"` Output the directory name of the command line used to execute the
shell script (which is in `"${0}"`).

`&& pwd` Once the current working directory has been changed to the script's directory
we run the `pwd` tool to get the absolute path of the program's working directory.

## Input Arguments

There are many ways to process input arguments for scripts. You can check out the man page for the [getopts](https://www.man7.org/linux/man-pages/man1/getopts.1p.html) command.

I prefer the while loop approach however

### While Loop Arguments

```sh
while [ $# -gt 0 ]; do
    case "${1}" in
        --clean)
            echo "CLEANING"
            ;;
        --verbose|-v)
            echo "VERBOSE"
            ;;
        --file|-f)
            shift
            echo "FILE: ${1}"
            ;;
    esac

    shift
done
```

Some examples of this in use:

```
[bjs@localhost bin]$ ./collect-static
```
```
[bjs@localhost bin]$ ./collect-static --clean
CLEANING
```

```
[bjs@localhost bin]$ ./collect-static --verbose --clean
VERBOSE
CLEANING
```

```
[bjs@localhost bin]$ ./collect-static -v --clean
VERBOSE
CLEANING
```

```
[bjs@localhost bin]$ ./collect-static -v --clean --file hello.txt
VERBOSE
CLEANING
FILE: hello.txt
```

## Looping
Various ways of looping depending on the data set.

### Multiple Line Variables

Sometimes we end up with a multiline list in a variable and we want to do something
for each item in the list. Looping over this list portably is straight forward using
a `HEREDOC` directed into a while loop:

```
files=$(ls -1 /etc)

while read -r file; do
    echo "Do something with ${file}"
done << EOF
${files}
EOF
```

Output:

```
Do something with abrt
Do something with adjtime
Do something with aliases
Do something with alsa
Do something with alternatives
Do something with anaconda
...
```

### Space Separated Variables

Looping over a space-separated list is a bit easier and can be done in a for loop.
Can't have any spaces in the list items though.

```
packages="gcc gcc-g++ gdb autotools automake"
for package in ${packages}; do
    echo "Do something with ${package}"
done
```

Output:
```
Do something with gcc
Do something with gcc-g++
Do something with gdb
Do something with autotools
Do something with automake
```

## Heredocs

These extremely useful things can be used in many places. Some examples of how they
can be used below. I end up using them for writing configuration files often.

### Write File from Heredoc

Variables are substituted normally when using a heredoc with variables inside it:

```
language=en-GB

cat << EOF > config.yaml
language: ${language}
EOF
```

Output

```
[brian@brian-2920x ~]$ cat config.yaml
language: en-GB

If you single quote the heredoc variables are not substituted:

```bash
language=en-GB

cat << 'EOF' > config.yaml
language: ${language}
EOF
```

Output

```
[brian@brian-2920x ~]$ cat config.yaml
language: ${language}
```
