
!!! warning
    This is an <strong>old post</strong> and is probably out of date - but give it a go if it helps you out!

PyLab is a collection of libraries for Python which includes numpy, scipy, and matplotlib. Together these libraries can be used to great effect to do work similar to that of Matlab.

# Why this page?

Aren't there a million pages showing how to use SciPy and NumPy? Well, yes there are, and I've read a lot of them, but the trouble is they are mainly stuck in pure math mode. A place where we generate ideal signals and analyse effects. This is an essential part of signal analysis, but in the real world we always have a problem to solve and usually we've got real sampled data to hand. I want to show some examples of actually working with real data.

## Loading and saving signals as WAV files
This should be straight forward, but it's slightly more complex than it should be. PyLab's support for wav files comes through the scipy.io module in the form of wavread and wavwrite. Unfortunately these only provide basic support for wav files and so we have to read a whole wav file at a time instead of just being able to read N samples.

## Quick Example - FIR Filter WAV File
As an example, below is code to open a wav file, apply an FIR filter on the data and then save to a new wav file

```python
from scipy.io import wavfile
from scipy.signal import firwin, lfilter

fs, datain = wavfile.read("in.wav")

# Design a non-windowed bandpass FIR filter
# Number of taps
N = 256
cutlow = 300
cuthigh = 500

# The filter design is a seperate chapter to follow...
taps = firwin(N, [cutlow, cuthigh], pass_zero=False, nyq=fs/2)

# Now we have the filter design and the taps, apply the filter to the data
filtered = lfilter(taps, 1.0, datain)

# Scale back to integers as otherwise you'll probably get a wav file that
# nothing can open (due to 64-bit floats as the data set!)

# Write the filtered signal back to a wav file
# It is important to use the astype!! Otherwise you end up with a wavfile that
# is unreadable by most software!
wavfile.write("out.wav", fs, filtered.astype(numpy.int16))
```

So now we can load some audio, do some filtering on the data and then save the result to another file.

This is useful, but we are also interested in the filters characteristics. What is the impulse response like, how can we see exaclty how the filter is behaving?

## Using matplotlib to plot stuff

Again, lets start off as simple as we can to view something useful. Viewing the filter impulse response (which is shown by the filter coefficients) tells us some of the characteristics of the filter.

If you're using ipython, then all you need do is type in the following:

```python
plot(t)
```

Now you can't really get easier than that! Of course, you can title the plot and set the x and y label values if you want, but generally we know what we are looking at, we just want to see it visually and so plot() usually gives a good indication of what's going on.

## Looking Closer at signals
We're using wavs for our signals, but it doesn't matter what you use as a data source; Sometimes you need to see the original signal and the modified signal.

Plotting the signal is as easy as you think:

```python
from scipy.io import wavfile
from scipy.signal import firwin, lfilter

fs, datain = wavfile.read("in.wav")

plot(datain)
```

The first problem is that the plot takes ages to generate if we have a suitably large input signal. Or, of course, we run out of memory whilst trying to plot the entire signal.

What we could do with is just plotting a small section of the input signal, so
that it provides us with enough detail, but is also quick to render.

## Splitting data sets in python
Let's quickly look at how we can get just a small portion of an array in python. The : operator used in the index provides us with a way of specifying a start and end position for the part of the array we are interested in.

The syntax is like:

```python
datain[from:to]
```

If from is omitted, from is assumed to be the start of the array (index=0), and
if to is omitted, to is assumed to be the end of the array (len(datain)-1)

For example, to get 3 seconds of a wav file sampled at 8kHz, we end up with the following code snippet:

```python
    a = datain[:8000 * 3]
```

So, back to looking at the signal again, let's now try and plot 3 seconds of the original signal:

```python
from scipy.io import wavfile
from scipy.signal import firwin, lfilter
from scipy.fftpack import fft

fs, datain = wavfile.read("in.wav")

plot(datain[:fs * 3])
```

Lovely Jubely, nice quick plot showing us the original signal. Now let's filter it and have a look at the filtered signal.

But what we want to see now is the first 3 seconds after it has been passed through the digital filter.

```python
from scipy.io import wavfile
from scipy.signal import firwin, lfilter

fs, datain = wavfile.read("in.wav")

N=512
lo=500
hi=1000
taps = firwin(N, [lo,hi], pass_zero=False, nyq=fs/2)
filtered = lfilter(taps, 1.0, datain[:fs * 3])

plot(filtered[N:])
```

So, in the above we speed things up a bit more by filterering just the first 3 seconds of the input data. Then, we plot the filtered output. BUT: We only plot the valid filter output. The first N samples of filtered data are not valid so we do not plot.

It's quick and we can get the result from our filter quickly.

When you're looking at the plot you're probably thinking it'd be nice if the axis were now a bit more relevant. For example, the X Axis is the number of samples. As we'd probably like the X Axis to be time, we must adjust the x-Axis to suit

In order to plot the time, we need to generate the time! This is done by creating a range (or actually a linspace). The length of the range must be the same length as the data being plotted, so again we get rid of the inaccurate first N samples.

```python
t = numpy.linspace(0.0, 3.0, (3*fs)-N)
plot(t, filtered[N:], '-g')
```

This plots the same as before, but now the x axis is scaled correctly to represent time. It is also coloured green, which is what '-g' means!

Let's label the X Axis so that it's more obvious:

```python
xlabel("Time (s)")
```

What we really want to do of course is plot both the original signal and the filtered signal at the same time so that we can see the difference between them.

```python
plot(t, datain[N:len(filtered)], '-b', t, filtered[N:], '-g')
xlabel("Time (s)")
title("500Hz to 1kHz Bandbass")
```

Note that you can look around the plot interactively with matplotlib. Click the four arrow icon and then pan with the mouse whilst holding the left button down and zoom (with axis) with the mouse whilst holding the right button down.

Also note, as we previously only filtered a portion of the input data, make sure we limit the data sets to the same time range.

I'll leave it to you to zoom and pan in to discover the phase delay in the filtered output. ;-)

## Frequency Domain Analysis

The frequency domain can tell us a lot more about a signal than the time domain can sometimes. As we're applying a filter, it would be nice to see a frequency domain plot of the input signal and a similar one for the filtered signal.

Working with Fast Fourier Transforms (FFT) is also made pretty easy in pylab. I shan't go into the theory about FFT's as there is plenty of information available in almost every Digital Signal Processing (DSP) book. It is an essenial part of DSP work and it is worth having a good understanding of the FFT and how it behaves. Hence, having an interactive python shell to do FFT calculations for you and plotting the results is a good bonus and will set you on your way with understanding them.

I highly recommend Understaing Digital Signal Processing for this subject as it is an awesome book to read: http://www.amazon.co.uk/Understanding-Digital-Signal-Processing-Richard/dp/0137027419/ref=sr_1_1?ie=UTF8&qid=1310391681&sr=8-1

The FFT creates a number of frequency bins. The FFT puts a value in each bin according to that frequency's energy level in the data. The FFT produces results of + and - frequencies! Make sure you understand that.

So, let's continue where we left off, we still have datain which contains our input data and we want to do an fft of the data.

The number of bins generated by the FFT is equivalent to the number of samples over which the FFT is performed. In actual fact the bins are mirrored around the nyquist frequency. So in fact if we are sampling at 8kHz and we do a 1024 FFT (powers of two are best for FFTs) we will end up with 512 bins equidistantly spread between 0 to fs/2.

Let's FFT:

```python
N=1024
freq = fft(datain[:N])
```

Well, that was pretty easy! But there is a bit more required yet! If you haven't already, go ahead and plot the fft result:

```python
plot(freq)
```

First off, we get an error about discarding the imaginary part of the values. That is okay, that is essentially the phase and we are not really interested in the phase.

But, we also have negative magnitudes! Let's fix that first before we do anything else:

```python
plot(abs(freq))
```

Okay, that's a bit better, and abs gets rid of the complex part of the values too so there is now no warning from plot about discarding them. At least it now looks a bit more like we'd expect from an FFT. But there is another big problem, as I mentioned before an FFT should generate frequency bins between 0 and +/- fs/2 so the x axis should read (if sampling at 8kHz) -4000 through to +4000. With that scale, the results should mirror around 0 on the x-axis.

In actual fact it is quite easy to re-arrange the results from the FFT to produce what we want to plot

Firstly, we need to create the X Axis, just like we did converting samples into time. Now though we want to convert bins into frequency. We do this by creating a range for the X axis:

```python
f = numpy.linspace(-fs/2, fs/2, N)
```

Now we create a new array, re-arranging the results of the FFT so that negative values come first and then positive values. The actual results are the opposite way around!

```python
freq2 = abs(concatenate((freq[N/2:], freq[:N/2])))
```

In most cases, the negative half of the plot can be disregarded as it is a mirror of the positive frequency half. I'll leave that up to the reader to solve . Hopefully this page has given you some useful snippets of code which can get you up and running when trying to start analysing signals with pylab.

## References / Documentation

The official documentation is very good for these projects.

The official Numpy/Scipy documentation can be found here: http://docs.scipy.org/doc/

and the Matplotlib documentation can be found here: http://matplotlib.sourceforge.net/
