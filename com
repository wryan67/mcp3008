g++ -lwiringPi -lm knobtest.cpp -o knobtest
g++ -lwiringPi -lm other.c  -o other

