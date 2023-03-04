cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_IDE:$PATH

cd ../../

stm32cubeidec --launcher.suppressErrors -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data . -import $PROJECT_NAME
