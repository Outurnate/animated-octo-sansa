#!/bin/bash
curl -L http://sourceforge.net/projects/neotextureedit/files/0.6/0.6.4/NeoTextureEdit_0.6.4.tar.gz/download -o neo.tar.gz
tar -xf neo.tar.gz --xform='s#^[^/]+#.#x' --no-anchored 'NeoTextureEdit.jar'
