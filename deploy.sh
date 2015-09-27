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


if [[ $upload_default ]]
then
	subfolder_name="$TRAVIS_BUILD_NUMBER-$TRAVIS_COMMIT_RANGE-$TRAVIS_BRANCH"
	$uploader mkdir "$subfolder_name"
	$uploader upload "build/gaem" "$subfolder_name/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME"
	$uploader upload "assets" "$subfolder_name/assets"
	[[ $upload_report ]] && $uploader upload "report.html" "$subfolder_name/report.html"
	$uploader delete "0-newest/assets/assets" || true
fi

if [[ $upload_newest ]]
then
	$uploader mkdir "0-newest"
	$uploader upload "build/gaem" "0-newest/gaem-$COMPILER_NAME"
	$uploader upload "assets" "0-newest/assets"
	[[ $upload_report ]] && $uploader upload "report.html" "0-newest/report.html"
	$uploader delete "0-newest/assets/assets" || true
fi

