set /p var=build_number.txt
set /a var=%var%+1
echo %var% > build_number.txt
