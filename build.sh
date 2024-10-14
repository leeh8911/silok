pyinstaller -y --clean --windowed --name silok \
--exclude-module _tkinter \
--exclude-module Tkinter \
--exclude-module enchant \
--exclude-module twisted \
/Users/isang-won/Desktop/develop/silok/silok/main.py

pyinstaller -y --clean --windowed silok.spec       

pushd dist   
hdiutil create ./silok.dmg -srcfolder silok.app -ov
popd