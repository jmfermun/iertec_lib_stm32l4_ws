cd "$(dirname "$0")"

source ../global.sh

ARTIFACT=../output/artifacts/$(date +'%Y%m%d')_${ARTIFACT_NAME}_DOC.zip

rm -f ${ARTIFACT}

7z a ${ARTIFACT} ../output/${PROJECT_NAME}_doc/

read -p 'Press enter to continue'
