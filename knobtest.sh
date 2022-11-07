#!/bin/bash

TXS0108_OE=6

gpio mode  $TXS0108_OE out
gpio write $TXS0108_OE 1

~/projects/mcp3008/knobtest

