@echo off

color f2
title OTA/SD������  
                        
@echo ����ϸ�Ķ�note.txt��Ϣ�����Ա���������ʱ���ֵ�һЩ���⡣

start "" "note.txt"
set /p var=�Ƿ��Ѿ������Ķ�note.txt��ȷ�����������ļ���y/n����������:

if /i %var% == y (
echo "�������������ļ�..."

if /i %1 == OTA (
echo "��ǰ����ģʽ��OTA"
copy db_update_files_data.bin upgrade\update-ota.ufw
echo.
echo �����ļ��Ѿ��ڵ�ǰupgradeĿ¼�����ɣ������ļ����ƣ�update-ota.ufw������OTA�����������ɣ�2���Ӻ��Զ��رմ��ڣ�
)else (

if /i %1 == SD (
echo "��ǰ����ģʽ��SD"
copy jl_isd.ufw upgrade\update.ufw
echo.
echo �����ļ��Ѿ��ڵ�ǰupgradeĿ¼�����ɣ������ļ����ƣ�update.ufw����update.ufw������SD��/U�̵ĸ�Ŀ¼���忨�ϵ缴��������2���Ӻ��Զ��رմ��ڣ�
) else (
echo "��ǰ����ģʽ��ERROR"
)

)

) else (
echo "���������ļ�ʧ��"
)

choice /t 2 /d y /n >nul 	

exit