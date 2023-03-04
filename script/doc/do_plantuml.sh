cd "$(dirname "$0")"

source ../global.sh

PATH=$PATH_JAVA:$PATH

java -jar $PATH_PLANTUML/plantuml.jar "../../docs/diagram" -o "../../docs/images"
