#!/bin/bash

if [ "$TRAVIS_PULL_REQUEST" != "false" ]
	then export DROPBOX_SUBFOLDER_NAME=PR#$TRAVIS_PULL_REQUEST-`echo $TRAVIS_REPO_SLUG | sed s:/:_:g | tr [:upper:] [:lower:]`-$TRAVIS_BRANCH
	else export DROPBOX_SUBFOLDER_NAME=$TRAVIS_BUILD_NUMBER-$TRAVIS_COMMIT_RANGE-$TRAVIS_BRANCH
fi

curl -Os "https://raw.githubusercontent.com/andreafabrizi/Dropbox-Uploader/master/dropbox_uploader.sh"
chmod +x dropbox_uploader.sh
./dropbox_uploader.sh -f .dropbox_uploader mkdir $DROPBOX_SUBFOLDER_NAME
./dropbox_uploader.sh -f .dropbox_uploader upload binaries/gaem $DROPBOX_SUBFOLDER_NAME/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME

