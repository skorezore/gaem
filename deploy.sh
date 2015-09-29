#!/bin/bash

uploader="./dropbox_uploader.sh -f .dropbox_uploader"

upload_default=true

while [[ $# -gt 0 ]]
do
	case $1 in
		--report)
			upload_report=true
			;;
		--newest)
			upload_newest=true
			;;
		--no-default)
			upload_default=""
			;;
		*)
			echo "Unknown argument \"$1\"! Ignoring."
		;;
	esac
	shift
done


curl -Os "https://raw.githubusercontent.com/andreafabrizi/Dropbox-Uploader/master/dropbox_uploader.sh"
chmod +x dropbox_uploader.sh


upload_to() {
	$uploader mkdir "$1"
	$uploader upload "build/gaem" "$1/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME"
	$uploader upload "assets" "$1/assets"
	[[ $upload_report ]] && $uploader upload "report.html" "$1/report.html"
	$uploader delete "0-newest/assets/assets" || true
}


[[ $upload_default ]] && upload_to "$TRAVIS_BUILD_NUMBER-$TRAVIS_COMMIT_RANGE-$TRAVIS_BRANCH"
[[ $upload_newest  ]] && upload_to "0-newest"

