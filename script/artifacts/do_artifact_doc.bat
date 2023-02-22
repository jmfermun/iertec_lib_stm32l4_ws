@echo off

cd /d %~dp0

call ..\global.bat

%PATH_MSYS_LAUNCHER%\msys2_shell.cmd -mingw64 -defterm -no-start -here -c "bash do_artifact_doc.sh"
