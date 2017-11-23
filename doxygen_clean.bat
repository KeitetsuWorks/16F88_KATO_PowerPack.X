@echo off

rem 
rem @file        doxygen_clean.bat
rem @brief       Doxygen 出力ファイル削除処理
rem @author      Keitetsu
rem @date        2017/03/06
rem @copyright   Copyright (c) 2017 Keitetsu
rem @par         License
rem              This software is released under the MIT License.
rem 

setlocal

set DOXYGEN_HTML_DIR=.\doxygen_html

echo Doxygen の出力ファイルを削除しています
rmdir /s /q %DOXYGEN_HTML_DIR%

echo 削除が完了しました

endlocal

exit /B
