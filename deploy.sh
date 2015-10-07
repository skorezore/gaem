#!/bin/bash

uploader="dropbox_uploader -f .dropbox_uploader"

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
		--newest)
			upload_changelog=true
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


upload_to() {
	$uploader mkdir "$1"
	$uploader upload "build/gaem" "$1/gaem-$2"
	$uploader upload "assets" "$1/assets"
	[[ $upload_report ]] && $uploader upload "report.html" "$1/report.html"
	[[ $upload_changelog ]] && $uploader upload "report.html" "$1/report.html"
	$uploader delete "$1/assets/assets" || true
}


[[ $upload_default ]] && upload_to "$TRAVIS_BUILD_NUMBER-$TRAVIS_COMMIT_RANGE-$TRAVIS_BRANCH" "$TRAVIS_BUILD_NUMBER-$COMPILER_NAME"
[[ $upload_newest  ]] && upload_to "0-newest" "$COMPILER_NAME"
