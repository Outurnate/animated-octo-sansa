#!/bin/bash
cd Textures
java -cp ../Utilities:../NeoTextureEdit_0.6.4/lib/NeoTextureEdit.jar NeoGen 256 `ls *.256.tgr`
java -cp ../Utilities:../NeoTextureEdit_0.6.4/lib/NeoTextureEdit.jar NeoGen 1024 `ls *.1024.tgr`
mogrify -format tga *.png
rm -f *.png
