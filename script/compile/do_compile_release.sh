cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_IDE:$PATH
PATH_OUT=$PWD/../output/compile

cd ../../

mkdir -p $PATH_OUT

stm32cubeidec --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data . -cleanBuild $PROJECT_NAME/Release 2>&1 | tee $PATH_OUT/compile_release_log.txt

cd $PROJECT_NAME/Release

compiledb --verbose --parse $PATH_OUT/compile_release_log.txt
