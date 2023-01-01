# Important Note Before Start

These are the assignments for CS106X 2019 Fall.

Before starting, Qt needs to be installed. I found that the code provided in the assignments is not compatible with newer versions of Qt. In order to work on these assignments, I recommend installing **Qt 5** and configuring with **32-bit** toolchains.

1. Qt modules since Qt 6.0 may not be compatible with these assignments. For example, in the first assignment, [QMediaPlaylist Class](https://doc.qt.io/qt-5/qmediaplaylist.html) is used, which has been removed since Qt 6.0.

    Also, the Qt Multimedia module is not installed by default since Qt 6.0.
    
    Therefore, I recommend installing Qt 5 for this course. For Windows, this can be done with `Qt Maintenance Tool`, which should be attached when downloading Qt.
    The other method (which also works for other operating systems) is to download Qt or Qt Multimedia module only in version 5 from the [archive website](https://download.qt.io/archive/qt/)

2. Use the 32-bit toolchain instead of the 64-bit. The projects cannot be built when I used 64-bit.

#### The Stanford C++ Libraries

Stanford implements a library to replace some of the C++ standard libraries used to simplify the development process for their C++ courses. The Stanford C++ Libraries are recommended and widely used in these assignments. [The Stanford C++ Libraries documentation](http://www.martinstepp.com/cppdoc/) need to be frequently referred to when doing these assignments.

