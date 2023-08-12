cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_IDE:$PATH
PATH_OUT=$PWD/../output/compile

cd ../../

mkdir -p $PATH_OUT

stm32cubeidec --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data . -cleanBuild $PROJECT_NAME/Debug 2>&1 | tee $PATH_OUT/compile_debug_log.txt

cd $PROJECT_NAME/Debug

compiledb --verbose --parse $PATH_OUT/compile_debug_log.txt
