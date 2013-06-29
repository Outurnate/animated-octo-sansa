#!/bin/bash
curl -L http://sourceforge.net/projects/neotextureedit/files/0.6/0.6.4/NeoTextureEdit_0.6.4.tar.gz/download -o neo.tar.gz
curl -L https://github.com/ashima/webgl-noise/archive/master.tar.gz -o master.tar.gz
tar -xf master.tar.gz
tar -xf neo.tar.gz --wildcards --no-anchored 'NeoTextureEdit_0.6.4/lib/NeoTextureEdit.jar'
cp *-noise-*/src/*.glsl Shaders
rm -rf *-noise-*
cd Utilities
javac NeoGen.java -cp ../NeoTextureEdit_0.6.4/lib/NeoTextureEdit.jar
