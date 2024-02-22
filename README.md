Small program to detect and change hues (OpenCV, Qt/Qml)

In the "demo" folder there is a short recording of the application's operation.

Some notes on the project:
 - I compiled the project using MSVC2019 and ran it on a Widnows 10 system.
 - It uses Qt6 (it should work from Qt version 5.15, but I didn't have time to check it anymore)
 - I used OpenCV version 4.5.2 (which I also compiled using MSVC2019)
 - The image window is native to OpenCV, I created the GUI window in QML, with the help of Qt.Quick, QtQuick.Controls, QtQuick.Layouts and Qt.labs.platform
 - This was the first time I was exposed to OpenCV, so while working I used heavily the information posted on StackOverflow and the code posted here: [GItHub: bsdnoobz](https://github.com/bsdnoobz/opencv-code/blob/master/shape-detect.cpp)

 Program operation
 1. After launching the application, press the "Open file" button and select the *png file
 2. An image will appear, then we press the "Detect shapes" button
 3. The figures are detected and loaded into a list in the GUI. Above the list is information about the number of detected triangles, rectangles, circles and ellipses.
 4. From this point, you can freely select the shape in the list or directly on the image.
 5. With the slider at the bottom, we change the HUE value for the selected element.
 6. We change the color of the selection by clicking on the colored rectangle below the words "Selection color (click below)". A color selection window will appear.

 Functionalities that I did not do due to time. I am also posting the planned approach to doing these functionalities:
 1. Moving the display from the OpenCV window to the GUI in qml. The planned approach: I would use QImageProvider and capture coordinates when the mouse is pressed which I would pass to the functionality that deals with selecting elements on the board
 2  Read other graphic formats. Planned approach: I would add other allowed types in the fileDialog in qml and check that they load and are handled correctly.
 3  Saving the active file. Planned approach: file dialog in qml has ready functionality - it would be enough to write a function in cpp that provides the path to save the file.
 4. Group selection of shapes. Planned approach: I would replace the active selected contour with a vector of selected contours (the rest of the cpp is already prepared to handle multiple selected contours). I would add my own support for selecting active indexes in qml.
 5  Reduce the number of variables in the HueController class. Planned approach: I would make related structures that need to be registered as a new type to be visible and usable in qml.
 6. Improve ellipsis detection. Planned approach: a good lead seems to be the use of the Hough transform, which in addition to line detection allows the detection of circles and ellipses - I would check how the implementation of this algorithm looks like in scikit-image.
    
## Application assumptions
- The application should be able to load a test image from the specified path in **png** format,
- After loading the image, the application should calculate the number of figures found in the image by shape (ellipses, triangles and rectangles),
- The user should be able to select any figure detected in the image (the selected figure should be marked with an outline in any color, which should be possible to define in the UI),
- After selecting the figure, the user should be allowed to change its color (shift **hue**).
