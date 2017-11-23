@echo off

rem 
rem @file        doxygen_make.bat
rem @brief       Doxygen 出力処理
rem @author      Keitetsu
rem @date        2017/03/06
rem @copyright   Copyright (c) 2017 Keitetsu
rem @par         License
rem              This software is released under the MIT License.
rem 

setlocal

set DOXYGEN_CONFIG_FILE=Doxyfile
set DOXYGEN_HTML_DIR=.\doxygen_html

if not exist %DOXYGEN_CONFIG_FILE% (
	echo Error: 設定ファイルが見つかりません
	pause
	goto END_MAKE
)
doxygen %DOXYGEN_CONFIG_FILE%

del /Q %DOXYGEN_HTML_DIR%\*.md5

:END_MAKE
endlocal

exit /B
