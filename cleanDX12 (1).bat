@echo off
@echo ��o�ɕs�v�ȃt�@�C�����폜���܂��B���̃t�H���_�����{�łȂ��R�s�[�ł��邱�Ƃ��m�F������Ŏ��s���Ă��������B
pause

set resdir1=Resources
set resdir2=lib

echo "%resdir1%","%resdir2%"�ȊO����s�v�t�@�C�����폜���܂�

cd /d %~dp0
call :delfiles

echo �폜���������܂���
pause

exit /b

:sub
cd %1
call :delfiles
cd ..
exit /b

:delfiles
if exist .vs rmdir /s /q .vs
if exist x64 rmdir /s /q x64
if exist Bin rmdir /s /q Bin
if exist Debug rmdir /s /q Debug
if exist Release rmdir /s /q Release
if exist *.obj del *.obj
if exist *.lib del *.lib
if exist *.cmd del *.cmd
if exist *.ilk del *.ilk
if exist *.db del *.db
if exist *.log del *.log
if exist *.tlog del *.tlog
if exist *.idb del *.idb
if exist *.pdb del *.pdb
if exist *.suo del *.suo
if exist *.pch del *.pch
if exist *.ipch del *.ipch
if exist *.iobj del *.iobj
if exist *.ipdb del *.ipdb
if exist *.cso del *.cso
if exist *.exe del *.exe
if exist *.enc del *.enc
for /d %%i in (.\*) do  (	
	if %%~ni == %resdir1% (
		echo %resdir1%���X�L�b�v
	) else if %%~ni == %resdir2% (
		echo %resdir2%���X�L�b�v
	) else (
		call :sub %%i
	)
)
exit /b
