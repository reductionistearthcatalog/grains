# GRAINS Projects
By Sean Luke (sean@cs.gmu.edu)

This is a collection of code which runs on the Tangible Waves GRAINS module, though it can be adapted to run on any Arduino with little difficulty.

## Lattice

A clone of Mutable Instruments GRIDS.  This is essentially a "TOPOGRAF Light".  It does the same thing but, due to limitations in GRAINS's interface, it has to simplify the parameters.  There are different configurations provided for doing so.  

Status: Working

## Beats

A four-track step sequencer with song mode, capable of performing songs of up to 800 trigger patterns, each pattern between 1 and 256 steps long.  You program the patterns in the code, it's set up to be very easy. Each pattern can loop for some N iterations (specified per-pattern), or loop forever.  You can reset, advance to the next pattern, and add swing and randomness in real time.

Status: Working

## Geiger

A three-output random trigger generator and Bernoulli trigger.  You provide a clock input and the probability that, for each output, the output will generate a trigger for a given clock.  You can make the track outputs independent or exclusive (only and exactly one triggers each clock).

Status: Working

## Pulses

A two- or three-output square-wave LFO generator.  Options include two independent LFOs, one with pulsewidth; three Square-wave LFOs all with 50% pulse width; and a mixture of three Square-wave LFOs in the same output.  You can reset and sync two LFOs to the third.

Status: Working

## Theory

One, Two, or Three digital outputs set up as functions of four, five, or six analog inputs.  You specify the functions in the code (it's set up to make it easy!), including special randomness and logic functions.

Status: Working

## Multiple

A two-track clock multiplier with a variety of multiplication options.

Status: Coming Up.  Not Yet Finished.


