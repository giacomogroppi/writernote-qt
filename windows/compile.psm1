rm build/writernote.exe
rm -r build/audio
rm -r build/bearer
rm -r build/iconengines
rm -r build/imageformats
rm -r build/mediaservice
rm -r build/platforms
rm -r build/playlistformats
rm -r build/printsupport
rm -r build/styles
rm -r build/translations
rm build/Qt5Network.dll
rm build/Qt5Multimedia.dll
rm build/Qt5Gui.dll
rm build/Qt5Core.dll
rm build/Qt5PrintSupport.dll
rm build/Qt5Svg.dll
rm build/Qt5Widgets.dll

qmake writernote.pro
make

windeployqt.exe ./build/writernote.exe --force


ldd ./build/writernote.exe | grep '\/mingw.*\.dll' -o | sort -u | xargs -I{} cp "{}" ./build
