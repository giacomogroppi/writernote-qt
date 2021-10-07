# Writernote
[![Snap](https://github.com/giacomogroppi/writernote-qt/actions/workflows/snapcraft.yml/badge.svg)](https://github.com/giacomogroppi/writernote-qt/actions/workflows/snapcraft.yml) [![Linux build](https://github.com/giacomogroppi/writernote-qt/actions/workflows/linux.yml/badge.svg)](https://github.com/giacomogroppi/writernote-qt/actions/workflows/linux.yml)
[![Windows package](https://github.com/giacomogroppi/writernote-qt/actions/workflows/windows.yml/badge.svg)](https://github.com/giacomogroppi/writernote-qt/actions/workflows/windows.yml) [![macos build](https://github.com/giacomogroppi/writernote-qt/actions/workflows/macos.yml/badge.svg)](https://github.com/giacomogroppi/writernote-qt/actions/workflows/macos.yml) [![Android](https://github.com/giacomogroppi/writernote-qt/actions/workflows/android.yml/badge.svg)](https://github.com/giacomogroppi/writernote-qt/actions/workflows/android.yml)
## Linux
<table>
<tr>
<td>

<img src="readme/linux-example.png" width=100% title="writernote Screenshot on Linux"/>
</td>
</tr>
</table>

<a href="https://snapcraft.io/writernote">
  <img alt="writernote" src="https://snapcraft.io/writernote/badge.svg" />
</a>
<a href="https://snapcraft.io/writernote">
  <img alt="writernote" src="https://snapcraft.io/writernote/trending.svg?name=0" />
</a>

## Features:
- Pen support
  - Zoom in and zoom on
  - Rubber size and type
  - Pen size option and type
  - Move object with the pen
  - Insert image
  - Enter text in pen mode
  - Reposition the page in case the application has encountered a bug
- Export file as PDF
- Print file
- Keyboard support
- Audio record:
  - Include audio file in writernote file
  - or decide to save the file in a specific location
  - listen to the audio again and visually see what was being written [only keyboard]
- Create style of sheet
- Dynamically decide how to create the sheet
- Extract audio from writernote file
- When closing the application, the program understands if the user has written something
- Decrease file size [only in pen mode]
- Open last open file 
   3 options:
    - Set the number of recent files to show
    - Disable the option, and show writernote directly
    - Directly open the last opened file
- Drag and drop:
  - Image
  - Writernote file
- Draw with mouse
- In case the application closes unexpectedly it is possible to recover the file, also setting the saving time of the restore file
- Ready pdf file and take notes

## Experimental Features:
- Import images
- Zoom in and Zoom on
- Android support
- Pen [different types of pen]
- Insert text into pen mode 
- Remove page
- Insert image into file

## To do
- Project .writer to image
- Convert audio to text
- Compile for IOS and mac
- Load file from pdf
- Create launcher for windows and linux to:
  - Send log information to a server
  - Restart the application
- Create external program to update writernote, it automatically has to download a zip archive of the latest release on github, extract it, and update the files
- Group

## Known bugs:
- Include audio file into .writer file
- Playing an audio in pen mode is not dynamic but static
- Save image:
  - In pen mode, moving an image is not supported
  - In keyboard mode it is possible to add images, and move them, but when the file is closed the images will be deleted, as with linux systems [snapcraft], a temporary position of the file is created, which will be deleted when closed, in windows systems instead the image will be automatically added if and only if it is not moved to disk, as the html link inside the file remains

## Supported platforms:
- GNU/Linux x86 [snap]
- GNU/Linux arm64 [snap]
- GNU/Linux armhf [snap]
- Android armeabi [apk]
- Android arm64 [apk]
- Android x86 [apk]
- Android x86_64 [apk]
- IOS
- Windows x86 [installer, zip]

## Installing
  - Linux: 
    
    Install stable version [from snapd]
    ```bash
    sudo snap install writernote
    sudo snap connect writernote:audio-record
    ```

    Install unstable version [last]
    With multipass you can specify how many thread, ram, and disk, for the virtual machine for compilation.
    Personaly i use 20 thread, 24G ram, and 100G of disk.

    ```bash
    sudo snap install multipass 
    sudo snap install snapcraft --classic
    git clone https://github.com/giacomogroppi/writernote-qt.git
    cd writernote-qt
    multipass launch --name snapcraft-writernote --cpus 20 --mem 24G --disk 100G
    snapcraft

    sudo snap install writernote*.snap --devmode --dangerous
    sudo snap connect writernote:audio-record
    ```

  - Windows and MacOS:
    [here](https://github.com/giacomogroppi/writernote-qt/releases) you can find all the version for windows.

## Compiling
  - Linux:
    dependence:
      it's raccomend to download qt creator, with all the component, from the official website.
      in case you can install all the dependence by install all the package from build-packages and the stage-packages in snapcraft.yaml [writernote]

    compile:
      ```
        mkdir build
        qmake writernote.pro
        make
      ```
  - Windows:
    dependence:
      mingw:
      ```
        pacman -S git
        git clone https://github.com/giacomogroppi/writernote-qt.git
        ./windows/dipendenze.sh
        ./windows/compile.sh
      ```
      power Shell: [to create an installation file]
      ```
        cd windows
        .\package.bat
      ```
  - Android:
    In /android you can find all the rule to compile writernote for android.
    It's an experimental project and a lot of things don't work for android.

## File format

The _.writer format is a compressed archive, in which a file `` indice.xml '' can be found, in which all the notebooks and all the audio are saved, whether they are recorded, or whether they are imported by the application.

When the file is open, only the indice.xml and the file of the copybook is loaded.

## Directory structure
- 3rdparty: all script for compile external library that we can't download directly
- andreoid: all roles for compile writernote for android
- images: all images
- language: all translation
- snap: snapcraft instructions for release the app in the snapstore
- windows: all script for compile writernote for windows
- updater: updater Qt project for update writernote [windows]
- src: [all file for application]
  - audioplay: file for play an audio
  - audiorecord: file for record an audio
  - audiosetting: file for load the setting of the audio
  - autosave: module for manage autosave
  - cloud: cloud file
  - currenttitle: class for the copybook
  - datamanage: file for manage file after stop recording of audio
  - dataread: module for read file .writer 
  - datawrite: module for writernote file .writer
  - dropEvent: file for manage drop event:
    - image
    - writernote file
  - frompdf: module for load a pdf
  - image: file for manage image inside writernote
  - last_open: mdule for load the last open file
  - lastedit: module for manage redo and redo
  - log/log_ui: module for manage writernote log
  - pdf: export pdf
  - restore_file: module for restore lost file after crash
  - sheet: style for the sheel
  - utils: some modules
  - windows: manage update
  - touch: 
    - copy: files to copy area
    - datastruct: class for manage data 
    - event: event for touch, resize event, mouse event ...
    - paintevent: module to draw object to pixmap
    - reduce_size: module to reduce precision of file
    - riascolto: module to load and play audio dynamically
    - rubber: rubber
    - scrollKentic: module for scrolling kinetic [pen mode]
    - square: module to move object with finger
    - text: insert text into file
    - zoom: module to manage zoom
    



## Development
The application is entirely developed by Giacomo Groppi.
For developing new features, write a Ticket, so others know what you are doing. For development create a fork, and use the test as base. Create a Pull request for each fix. Do not create big pull requests, as long as you don't break anything features also can be merged, even if they are not 100% finished.. To report a problem send an email to the same address [possible with the error log and/or with a photo].

See [GitHub:writernote](http://github.com/giacomogroppi/writernote-qt) for current development.

# Todo
In the code there are some references to TODOs

## Third-party
* Share file in Android and IOS ([LGPL](https://github.com/ekke/ekkesSHAREexample))
* Qt 5.15.2, 5.6.2 and 5.3.2 slightly patched ([LGPL](http://doc.qt.io/qt-5/lgpl.html))
* OpenSSL 1.1.1 and 1.0.1 ([OpenSSL License](https://www.openssl.org/source/license.html))
* zlib 1.2.11 ([zlib License](http://www.zlib.net/zlib_license.html))
* CMake ([New BSD License](https://github.com/Kitware/CMake/blob/master/Copyright.txt))
* libzip ([BSD License](https://libzip.org/license/))