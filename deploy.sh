#!/bin/sh

uploader="./dropbox_uploader.sh -f .dropbox_uploader"
subfolder_name=$TRAVIS_BUILD_NUMBER-$TRAVIS_COMMIT_RANGE-$TRAVIS_BRANCH

curl -Os "https://raw.githubusercontent.com/andreafabrizi/Dropbox-Uploader/master/dropbox_uploader.sh"
chmod +x dropbox_uploader.sh

$uploader mkdir "$subfolder_name"
case "$1" in
	"")
		$uploader upload binaries/gaem "$subfolder_name/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME"
		$uploader -s upload assets "$subfolder_name"
		$uploader -s upload binaries/*cpponfig.* "$subfolder_name"
		;;
	--report)
		$uploader -s upload report.html "$subfolder_name/report.html"
		;;
esac

