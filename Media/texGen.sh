#!/bin/bash
cd Textures
java -cp ../Utilities:../NeoTextureEdit_0.6.4/lib/NeoTextureEdit.jar NeoGen 256 `ls *.tgr`
mogrify -format tga *.png
rm -f *.png
