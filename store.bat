del sheep.zip
mkdir sheep
xcopy /s SDLBasic\SDLBasic.exe sheep
xcopy /s SDLBasic\*.dll sheep
xcopy /s /i SDLBASIC\assets sheep\assets
powershell -command "Compress-Archive -Path sheep -DestinationPath sheep.zip
rmdir /s /q sheep