# Working with the Screen Command

Various times when working on remote servers, they're configured to automatically
log you out after a certain period of inactivity.

This can be annoying to say the least when you're running a command and it takes
a long time to complete.

`screen` is a good way of running that command because even if you've been logged
out, the screen session stays active and the command carries on executing within
the screen session.

It's another useful tool to have in your back pocket.

Let's have a look at how we can start managing screen sessions.

## Starting a New Screen Session

In order to start a new screen session, you simply type the command `screen`
with no parameters.

You are then presented with a screen session that looks very much like your
standard terminal. There is a difference though - screen only keeps what you can
see on your terminal. You cannot scroll back through output that has run off of
the screen like you can with a terminal. So if you need the output, or might
want to check something in the output, make sure any commands you run pipe or
redirect their output to a file you can investigate after the command has
completed.

## Detaching From a Screen Session

While a long running command is running, or at any other time, you can detach
from the screen session to return to your terminal.

Press `Ctrl-A` and release, then press `d` to detach. Detaching from a session
does not stop the screen session running.

## Listing and Reattaching to Screen Sessions

You can see what screen sessions are currently running for the current user
using `screen -ls`. This will list the sessions that are currently running.

You can re-attach to any session in the list by running `screen -r full.session.id`.

## Killing a Screen Session

When you've finished with a screen session, you can kill it so it is no longer running,

Press `Ctrl-A` and release, then press `k` to kill the screen session. Confirm the
question of `Do you want to kill this session` by pressing `y`.


## More Information

`screen` is really powerful. There's lots more you can do with it. For the definitive
guide, type `man screen` to read the manual for screen. There's loads of information
in there.

It makes a great serial line monitor for example when you're trying to debug serial
communications.
