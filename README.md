# Human Tracking in Video Sequences

A C/C++ implementation of pedestrian detection and tracking of original paper:

Breitenstein et al, Online Multiperson Tracking-by-Detection from a Single, Uncalibrated Camera, PAMI 2011.

=========

The main function in Main.m

The tracking method: mf_boostingclassifier.cpp (using mex to build, it was included in Main.m)

1. Prepare the dataset.
2. Run hogdetector.exe in /hogdetector to detect automatically bounding box in video.
3. Run Main.m 

You need to make sure the correct path of OpenCV to build mex file. 

Notice that  You have to run hogdetector at first to have bounding boxes before running tracking program.
