# ELECAP
A oscilloscope and wave generater project on Stm32H750 used in nuedc-training.

This branch modified framecache, which only save the index of color instead of the color itself, to save memory.
If the kind of color is less than 16, this method need only 1/4 memory of save RGB565 format.