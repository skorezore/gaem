#!/bin/bash

uploader="./dropbox_uploader.sh -f .dropbox_uploader"
subfolder_name="$TRAVIS_BUILD_NUMBER-$TRAVIS_COMMIT_RANGE-$TRAVIS_BRANCH"
temporary_folder=$(mktemp -d /tmp/gaem_deploy.XXXXXXXXXX)

upload_default=true
explicit_upload_default=false

while [[ $# -gt 0 ]]
do
	case $1 in
		--report)
			upload_report=true
			upload_default=false
			;;
		--tarball)
			upload_tarball=true
			upload_default=false
			;;
		--newest)
			upload_newest=true
			upload_default=false
			;;
		--default)
			explicit_upload_default=true
			;;
		*)
			echo "Unknown argument \"$1\"! Ignoring."
		;;
	esac
	shift
done


curl -Os "https://raw.githubusercontent.com/andreafabrizi/Dropbox-Uploader/master/dropbox_uploader.sh"
chmod +x dropbox_uploader.sh


[[ -n $upload_default ]] && upload_default=$explicit_upload_default
[[ $upload_newest ]] && subfolder_name="$subfolder_name 0-newest"


for folder in $subfolder_name
do
	if [[ $folder = *newest* ]]
	then
		binarysuffix="$COMPILER_NAME"
	else
		binarysuffix="$TRAVIS_BUILD_NUMBER-$COMPILER_NAME"
	fi

	all_files_temporary_folder=$(mktemp --dry-run allfiles.XXXXXXXXXX)
	mkdir "$temporary_folder/$all_files_temporary_folder"

	cp "binaries/gaem" "$temporary_folder/$all_files_temporary_folder/gaem-$binarysuffix"
	cp -r "assets" "$temporary_folder/$all_files_temporary_folder/"

	[[ $upload_report ]] && cp "report.html" "$temporary_folder/$all_files_temporary_folder/"


	for folder in $subfolder_name
	do
		mv "$temporary_folder/$all_files_temporary_folder" "$temporary_folder/$folder"

		pushd "$temporary_folder" > /dev/null
		tar -caf "$folder-$COMPILER_NAME.tar.bz2" "$folder"
		popd > /dev/null

		mv "$temporary_folder/$folder" "$temporary_folder/$all_files_temporary_folder"
	done


	$uploader mkdir "$folder"

	[[ $upload_default ]] && $uploader upload "$temporary_folder/$all_files_temporary_folder"/* "$folder"
	[[ $upload_report ]] && $uploader -s upload "$temporary_folder/$all_files_temporary_folder/report.html" "$folder"
	[[ $upload_tarball ]] && $uploader upload "$temporary_folder/$folder-$COMPILER_NAME.tar.bz2" "$folder"
done

$uploader delete "0-newest/gaem-$TRAVIS_BUILD_NUMBER-$COMPILER_NAME" || true

rm -rf "$temporary_folder"
