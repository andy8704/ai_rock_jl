@echo off

@echo *********************************************************************
@echo 			    AC791N SDK SD/U�������ļ�����
@echo *********************************************************************
@echo %date%

::��ִ��download.bat�����ļ�
call .\download.bat

if exist upgrade\*.ufw del upgrade\*.ufw

if not exist upgrade md upgrade

set mode=SD
start .\upgrade.bat %mode%

choice /t 2 /d y /n >nul 


::pause
exit
