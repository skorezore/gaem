#!/bin/bash

uploader="dropbox_uploader -f .dropbox_uploader"

$uploader download "LAST_COMMIT" "LAST_COMMIT"
$uploader upload "CHANGELOG" "CHANGELOG"

{
	echo "0a"
	echo "Build #$TRAVIS_BUILD_NUMBER:"
	git log "$(cat LAST_COMMIT)..$TRAVIS_COMMIT" --pretty=oneline
	echo "."
	echo "w"
} | ed CHANGELOG  # Prepend to CHANGELOG, see http://stackoverflow.com/a/3272296/2851815

echo "$TRAVIS_COMMIT" > "LAST_COMMIT"
$uploader upload "LAST_COMMIT" "LAST_COMMIT"
$uploader upload "CHANGELOG" "CHANGELOG"
