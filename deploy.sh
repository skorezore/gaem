#!/bin/sh

dropbox_subfolder_name=$TRAVIS_BUILD_NUMBER-$TRAVIS_COMMIT_RANGE-$TRAVIS_BRANCH

curl -Os "https://raw.githubusercontent.com/andreafabrizi/Dropbox-Uploader/master/dropbox_uploader.sh"
chmod +x dropbox_uploader.sh

./dropbox_uploader.sh -f .dropbox_uploader mkdir "$dropbox_subfolder_name"
case "$1" in
	"")
		./dropbox_uploader.sh -f .dropbox_uploader upload binaries/gaem "$dropbox_subfolder_name/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME"
		./dropbox_uploader.sh -sf .dropbox_uploader upload assets "$dropbox_subfolder_name"
		./dropbox_uploader.sh -sf .dropbox_uploader upload binaries/*cpponfig.* "$dropbox_subfolder_name"
		;;
	--report)
		./dropbox_uploader.sh -sf .dropbox_uploader upload report.html "$dropbox_subfolder_name/report.html"
		;;
esac

