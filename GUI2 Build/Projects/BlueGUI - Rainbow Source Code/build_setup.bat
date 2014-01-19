rmdir build

mkdir build
mkdir build\graphics
mkdir build\graphics\toolbar
mkdir build\samples

copy rainbow.exe build\rainbow.exe
copy *.manifest build
copy graphics build\graphics
copy graphics\toolbar build\graphics\toolbar
copy samples\*.dba build\samples
copy samples\*.gui build\samples
copy samples\*.dbpro build\samples
copy config.ini build\config.ini