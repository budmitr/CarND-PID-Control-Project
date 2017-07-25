# Reflection

With some manual tests the effects of PID components look same:
* P component increase causes very oscillating trajectory, with high speed it causes off-track issues.
Good order of magnitude is ~1.
* I component impacts a lot, so if there is a big value, car can immediately go off the track and never come back.
Good order of magnitude is ~0.1
* D component is kind of smoother for P, so its higher value decreased oscillations.
Good order of magnitude is ~1.5

The behaviour of PID with different coefficients follows the expectations.

**Resulting PID params are chosen with Twiddle procedure**

This took a long time for training, first tests were with smaller amounts of iterations to actually define a reasonable 
order of values, then I increased number of messages per iteration and left my machine working for several hours to
find the solution.
Result was found satisfying twiddle tolerance value of 0.2
