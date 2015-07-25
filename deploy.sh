#!/bin/bash

export DROPBOX_SUBFOLDER_NAME=$TRAVIS_BUILD_NUMBER-$TRAVIS_COMMIT_RANGE-$TRAVIS_BRANCH

curl -Os "https://raw.githubusercontent.com/andreafabrizi/Dropbox-Uploader/master/dropbox_uploader.sh"
chmod +x dropbox_uploader.sh
./dropbox_uploader.sh -f .dropbox_uploader mkdir $DROPBOX_SUBFOLDER_NAME
./dropbox_uploader.sh -f .dropbox_uploader upload binaries/gaem $DROPBOX_SUBFOLDER_NAME/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME

