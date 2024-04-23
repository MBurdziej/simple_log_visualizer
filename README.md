
![ss](https://github.com/MBurdziej/simple_log_visualizer/assets/108184079/4256d5f8-5016-4b26-b887-592a1b016c5e)

Program is a simple windowed application written in C++ using the gdiplus.h library for visualizing data from a log file.

Features:
1. Data Loading: The program automatically loads data from the "outputRobotForwardA02.log" file, which contains data from three axes of motion.
2. Drawing: Users can choose which data to draw (x, y, z axes, vector sum, or all simultaneously) using buttons.
3. Parameter Modification: Users can modify drawing parameters such as time axis, amplitude, drawing step, and data offset.
4. Average Calculation: The program allows calculation of the average acceleration value g.
5. Timer: An option to enable and disable a timer, which automatically shifts the data view, is available.
6. Displaying Information: The program displays information about the number of steps, step time, and idle time.
