import os
import sys
import re

if __name__ == "__main__":
    version_major = ""
    version_minor = ""
    version_patch = ""

    # Change path to the directory of this file
    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(dname)

    file_name = "../../" + sys.argv[1] + "/src/version.h"

    # Search the version numbers in the version.h file
    with open(file_name) as file:
        for line in file:
            if "VERSION_MAJOR" in line:
                version_major = re.search("[0-9]+", line).group(0)
            elif "VERSION_MINOR" in line:
                version_minor = re.search("[0-9]+", line).group(0)
            elif "VERSION_PATCH" in line:
                version_patch = re.search("[0-9]+", line).group(0)
    
    version = "v{0}.{1}.{2}".format(version_major, version_minor, version_patch)

    print(version)
