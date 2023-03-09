cd "$(dirname "$0")"

source ../global.sh

export JAVA_HOME="$PATH_JAVA/.."

cd ../../$PROJECT_NAME

bash $PATH_SONAR_SCANNER/sonar-scanner -Dsonar.login=$SONARQUBE_TOKEN -Dproject.settings=../script/ci_cd/sonar-project.properties
