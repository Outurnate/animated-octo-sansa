#!/bin/bash
cd Textures
java -cp ../Utilities:../lib/NeoTextureEdit.jar NeoGen 256 `ls *.tgr`
mogrify -format tga *.png
