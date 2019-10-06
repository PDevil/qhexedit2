if [ ! -d build ];then
    mkdir build
else
    rm -rf build/*
fi

cd build
qmake -tp vc ../example/qhexedit.pro
make
cd ..
