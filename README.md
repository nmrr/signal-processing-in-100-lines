# signal-processing-in-100-lines
A 100 lines example of signal processing written in C++

In this example, a [noisy](https://en.wikipedia.org/wiki/Noise_(electronics)) [square wave](https://en.wikipedia.org/wiki/Square_wave) signal is generated. This signal is passed into a [low-pass filter](https://en.wikipedia.org/wiki/Low-pass_filter) to reduce noise. An [automatic gain control (AGC)](https://en.wikipedia.org/wiki/Automatic_gain_control) is used to prevent fluctuation of the amplitude and get a constant output signal. Then, a [limiter](https://en.wikipedia.org/wiki/Limiter) limits output value of each sample to a fixed value to avoid impossible value during conversion to 16-bit ([hard clipping](https://en.wikipedia.org/wiki/Clipping_(audio)))

![](https://github.com/nmrr/signal-processing-in-100-lines/blob/main/img/draw.png)

Build the example : 
```
g++ -Wall -pipe -Ofast main.cpp -o signalprocessing
```


Run the example :
```
./signalprocessing > out
```

Use audacity (or other audio software) and import output data as **Raw Data** using the following parameters :
- Signed 16-bit PCM
- Little-endian
- 1 Channel (Mono)
- Sample rate : 44100

[Cutoff frequency](https://en.wikipedia.org/wiki/Cutoff_frequency) can be adjusted by changing the value of the variable **lowPassSamples** : a lower value will rise cutoff frequency.
