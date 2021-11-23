# CIL

## LibJPEG Support

```
wget https://ijg.org/files/jpegsrc.v9d.tar.gz
tar -xfv jpegsrc.v9d.tar.gz
cd jpeg-9d/
mkdir build && cd build
LIBJPEG_INSTALL_DIR = <install_path>
../configure --prefix=$LIBJPEG_INSTALL_DIR
make -j8 install
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$LIBJPEG_INSTALL_DIR/lib/pkgconfig/
```

