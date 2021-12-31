/*
MIT License
Copyright (c) 2021 https://github.com/nmrr 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>
#include <random>

using namespace std;

#define SAMPLES 44100*10

void int16ToChar(int16_t input, char &a, char &b)
{
    union
    {
        int16_t value;
        char array[2];
    };

    value = input;
    a = array[0];
    b = array[1];
}

int main(int argc, char** argv)
{
    random_device rd;
    uniform_real_distribution<> dis(-1, 1);

    double doubleArray[SAMPLES];

    // 441 Hz NOISY SQUARE WAVE GENERATOR
    uint32_t counter = 0;
    int output = -1;
    double outputDouble = 0;
    for (uint32_t i=0; i<SAMPLES; i++)
    {
        if(counter == 50)
        {
            counter = 1;
            if (output == -1) output = 1;
            else output = -1;
        }
        else counter++;

        if (output == -1) outputDouble = -1 + dis(rd);
        else outputDouble = 1 + dis(rd);

        doubleArray[i] = 0.2*outputDouble;
    }

    // LOW-PASS FILTER
    uint32_t lowPassSamples = 5;
    bool end = false;
    for (uint32_t i=0; i<SAMPLES; i++)
    {
        if (i+lowPassSamples >= SAMPLES)
        {
            lowPassSamples = SAMPLES - i;
            end = true;
        }

        double average = 0;
        for (uint32_t j=i; j<i+lowPassSamples; j++) average += doubleArray[j];

        average /= lowPassSamples;
        for (uint32_t j=i; j<i+lowPassSamples; j++) doubleArray[j] = average;

        if (end == true) break;
    }
    
    // AUTOMATIC GAIN CONTROL
    double amplifer = 1;
    double maximum = doubleArray[0];
    double minimum = doubleArray[0];
    counter = 0;
    for (uint32_t i=1; i<SAMPLES; i++)
    {
        doubleArray[i] *= amplifer;
        counter++;
        
        if (doubleArray[i] > maximum) maximum = doubleArray[i];
        if (doubleArray[i] < minimum) minimum = doubleArray[i];

        if (counter == 1000)
        {
            double difference = sqrt(minimum*minimum + maximum*maximum);
            
            if (difference < 1.1) amplifer += 0.1;
            else if (difference < 1.15) amplifer += 0.01;
            else if (difference > 1.25 && difference < 1.3) amplifer -= 0.01;
            else amplifer -= 0.1;
            
            counter = 0;
            maximum = doubleArray[i];
            minimum = doubleArray[i];
        }
    }

    // LIMITER
    for (uint32_t i=0; i<SAMPLES; i++)
    {
        if (doubleArray[i] > 1) doubleArray[i] = 1;
        else if (doubleArray[i] < -1) doubleArray[i] = -1;
    }

    // TO SIGNED 16-BIT
    for (uint32_t i=0; i<SAMPLES; i++)
    {
        char a,b;
        int16ToChar(32767*doubleArray[i],a,b);
        cout << a << b;
    }

    return 0;
}