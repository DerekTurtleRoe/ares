rmdir /Q /S nall
rmdir /Q /S hiro
rmdir /Q /S ruby

mkdir nall
mkdir hiro
mkdir ruby

xcopy /E ..\..\..\nall nall
xcopy /E ..\..\..\hiro hiro
xcopy /E ..\..\..\ruby ruby
