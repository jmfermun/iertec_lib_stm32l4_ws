cd "$(dirname "$0")"

source ../global.sh

cd ../../${PROJECT_NAME}/Release

compiledb --verbose --parse ../../.metadata/.plugins/org.eclipse.cdt.ui/${PROJECT_NAME}.build.log
