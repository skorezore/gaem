#!/bin/bash

curl -Oq "https://raw.githubusercontent.com/andreafabrizi/Dropbox-Uploader/master/dropbox_uploader.sh"
chmod +x dropbox_uploader.sh
./dropbox_uploader.sh -f .dropbox_uploader mkdir $TRAVIS_BUILD_NUMBER
./dropbox_uploader.sh -f .dropbox_uploader upload binaries/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME $TRAVIS_BUILD_NUMBER/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME

