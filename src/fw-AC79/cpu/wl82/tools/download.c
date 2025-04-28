#include "app_config.h"
// *INDENT-OFF*
#ifdef __SHELL__

${OBJDUMP} -D -print-dbg -mcpu=r3 $1.elf > ../../../$1.lst
${OBJDUMP} -section-headers -address-mask=0x1ffffff $1.elf
${OBJSIZEDUMP} -lite -skip-zero -enable-dbg-info $1.elf | sort -k 1 >  symbol_tbl.txt

/opt/utils/strip-ini -i isd_config.ini -o isd_config.ini

objsizedump -lite -skip-zero sdk.elf | sort -k 1 > ../../../map.lit

export PROJ_BUILD=download.bat

echo "echo 加一下中文注释,防止服务器导出不了报错" >> ${PROJ_BUILD}
echo "echo 加一下中文注释,防止服务器导出不了报错" >> ${PROJ_BUILD}
echo "echo 加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释,防止服务器导出不了报错,加一下中文注释" >> ${PROJ_BUILD}

#if defined CONFIG_UI_ENABLE
#if defined UI_USE_WIFI_CAMERA_PROJECT
echo "@echo                                        "是否输出UI资源文件_按Y_不生成继续_按N生成并继续_倒计时3s自动选择Y"" >> ${PROJ_BUILD}
echo "choice /t 3 /c ync /n /d y" >> ${PROJ_BUILD}
echo "if %errorlevel%==1 goto end" >> ${PROJ_BUILD}
echo "if %errorlevel%==2 goto continue" >> ${PROJ_BUILD}
echo ":continue" >> ${PROJ_BUILD}
echo "cd ..\..\..\ui_project\3.ui_demo_wifi_camera\ui_320x480_test\project" >> ${PROJ_BUILD}
echo "start auto_ui_res.bat" >> ${PROJ_BUILD}
echo "cd ..\..\..\..\cpu\wl82\tools" >> ${PROJ_BUILD}
echo "TIMEOUT /T 8" >> ${PROJ_BUILD}
#endif
echo ":end" >> ${PROJ_BUILD}
#endif

echo "set OBJDUMP=C:\JL\pi32\bin\llvm-objdump.exe" >> ${PROJ_BUILD}
echo "set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy.exe" >> ${PROJ_BUILD}
echo "set ELFFILE=sdk.elf" >> ${PROJ_BUILD}
echo "%OBJCOPY% -O binary -j .text %ELFFILE% text.bin" >> ${PROJ_BUILD}
echo "%OBJCOPY% -O binary -j .data %ELFFILE% data.bin" >> ${PROJ_BUILD}
echo "%OBJCOPY% -O binary -j .ram0_data  %ELFFILE% ram0_data.bin" >> ${PROJ_BUILD}
echo "%OBJCOPY% -O binary -j .cache_ram_data  %ELFFILE% cache_ram_data.bin" >> ${PROJ_BUILD}
echo "%OBJDUMP% -section-headers -address-mask=0x1ffffff %ELFFILE%" >> ${PROJ_BUILD}
echo "%OBJDUMP% -t %ELFFILE% > symbol_tbl.txt" >> ${PROJ_BUILD}
echo "copy /b text.bin+data.bin+ram0_data.bin+cache_ram_data.bin app.bin" >> ${PROJ_BUILD}
#if defined CONFIG_UI_ENABLE
#if defined CONFIG_UI_FILE_SAVE_IN_RESERVED_EXPAND_ZONE
echo "packres\packres.exe -n ui -o packres/UIPACKRES ui_res" >> ${PROJ_BUILD}
#else
echo "set UI_RES=ui_res" >> ${PROJ_BUILD}
#endif
#endif

#if defined CONFIG_AUDIO_ENABLE && defined CONFIG_VOICE_PROMPT_FILE_PATH
#if defined CONFIG_VOICE_PROMPT_FILE_SAVE_IN_RESERVED_EXPAND_ZONE
echo "packres\packres.exe -n tone -o packres/AUPACKRES audlogo" >> ${PROJ_BUILD}
#else
echo "set AUDIO_RES=audlogo" >> ${PROJ_BUILD}
#endif
#endif

#if CONFIG_DOUBLE_BANK_ENABLE
echo "set UPDATE_FILES=-update_files normal" >> ${PROJ_BUILD}
#endif

#if defined CONFIG_SDFILE_EXT_ENABLE
echo -n "fat_image_tool.exe --size " >> ${PROJ_BUILD}
echo -n CONFIG_SDNAND_HFAT_LEN_TEXT >> ${PROJ_BUILD}
echo -n " --sectors-per-cluster " >> ${PROJ_BUILD}
echo -n CONFIG_SDNAND_HFAT_CLUSTER_SIZE >> ${PROJ_BUILD}
echo " --lfn true --n-root 512 --volume-name HFAT_IMG --output jl_hfat.bin --fat-dir hfat_dir" >> ${PROJ_BUILD}
echo -n "fat_image_tool.exe --size " >> ${PROJ_BUILD}
echo -n CONFIG_SDNAND_FAT1_LEN_TEXT >> ${PROJ_BUILD}
echo -n " --sectors-per-cluster " >> ${PROJ_BUILD}
echo -n CONFIG_SDNAND_FAT1_CLUSTER_SIZE >> ${PROJ_BUILD}
echo " --lfn true --n-root 512 --volume-name FAT1_IMG --output jl_fat1.bin --fat-dir fat1_dir" >> ${PROJ_BUILD}
echo "isd_download.exe isd_config.ini -gen2 -to-sdcard -dev wl82 -boot 0x1c02000 -div1 -app app.bin cfg_tool.bin -res %AUDIO_RES% %UI_RES% cfg -extend-bin -output-bin jl_hfs.bin %UPDATE_FILES% -no-app-bin-enc" >> ${PROJ_BUILD}
#elif defined CONFIG_SFC_ENABLE
echo "isd_download.exe isd_config.ini -gen2 -tonorflash -dev wl82 -boot 0x1c02000 -div1 -wait 300 -uboot uboot.boot -app app.bin cfg_tool.bin -res %AUDIO_RES% %UI_RES% cfg -reboot 500 %UPDATE_FILES% -extend-bin" >> ${PROJ_BUILD}
#else
echo "set run_addr=0x2000" >> ${PROJ_BUILD}
echo "set load_addr=0x4000" >> ${PROJ_BUILD}
echo "set mask_addr=0x100000" >> ${PROJ_BUILD}
echo "uboot_lz4.exe app.bin app.lz4 %run_addr% %load_addr% rom.image %mask_addr%" >> ${PROJ_BUILD}
echo "isd_download.exe isd_config.ini -gen2 -tonorflash -dev wl82 -boot 0x1c02000 -div1 -wait 300 -uboot uboot.boot -app app.lz4 cfg_tool.bin -res %AUDIO_RES% %UI_RES% cfg -reboot 500 %UPDATE_FILES% -extend-bin" >> ${PROJ_BUILD}
#endif

#if 0
echo "if exist *.mp3 del *.mp3" >> ${PROJ_BUILD}
echo "if exist *.PIX del *.PIX" >> ${PROJ_BUILD}
echo "if exist *.TAB del *.TAB" >> ${PROJ_BUILD}
echo "if exist *.res del *.res" >> ${PROJ_BUILD}
echo "if exist *.sty del *.sty" >> ${PROJ_BUILD}
#endif

#if defined CONFIG_SDFILE_EXT_ENABLE
echo "copy isd_config_loader.ini loader_tools\isd_config.ini" >> ${PROJ_BUILD}
echo "copy jl_hfs.bin loader_tools\jl_hfs.bin" >> ${PROJ_BUILD}
echo "copy jl_hfat.bin loader_tools\jl_hfat.bin" >> ${PROJ_BUILD}
echo "copy jl_fat1.bin loader_tools\jl_fat1.bin" >> ${PROJ_BUILD}
#else
#if !(CONFIG_DOUBLE_BANK_ENABLE)
echo "fw_add.exe -noenc -fw jl_isd.fw -add ota.bin -type 100 -out jl_isd.fw" >> ${PROJ_BUILD}
#endif
echo "fw_add.exe -noenc -fw jl_isd.fw -add script.ver -out jl_isd.fw" >> ${PROJ_BUILD}

echo "ufw_maker.exe -fw_to_ufw jl_isd.fw" >> ${PROJ_BUILD}
#endif
echo "ping /n 2 127.1>null" >> ${PROJ_BUILD}
//echo "IF EXIST null del null" >> ${PROJ_BUILD}
echo "del cache_ram_data.bin" >> ${PROJ_BUILD}
echo "del data.bin" >> ${PROJ_BUILD}
echo "del ram0_data.bin" >> ${PROJ_BUILD}
echo "del text.bin" >> ${PROJ_BUILD}
#if defined CONFIG_SDFILE_EXT_ENABLE
echo "del jl_hfs.bin" >> ${PROJ_BUILD}
echo "del jl_hfat.bin" >> ${PROJ_BUILD}
echo "del jl_fat1.bin" >> ${PROJ_BUILD}
echo "cd loader_tools" >> ${PROJ_BUILD}
echo "call download.bat" >> ${PROJ_BUILD}
echo "cd ..\\" >> ${PROJ_BUILD}
#endif
echo "TIMEOUT /T 3" >> ${PROJ_BUILD}
echo "exit /b 0" >> ${PROJ_BUILD}

#if defined CONFIG_SDFILE_EXT_ENABLE
files="isd_config.ini isd_config_loader.ini download.bat"
#else
files="isd_config.ini download.bat"
#endif

host-client -project ${NICKNAME}$2 -f ${files} $1.elf

rm download.bat
rm isd_config.ini

#else


@echo off

@echo *********************************************************************
@echo                           AC791N SDK
@echo *********************************************************************
@echo %date%

cd /d %~dp0

echo %*

#if defined CONFIG_UI_ENABLE
#if defined UI_USE_WIFI_CAMERA_PROJECT
cd ..\..\..\ui_project\3.ui_demo_wifi_camera\ui_320x480_test\project
start auto_ui_res.bat
cd ..\..\..\..\cpu\wl82\tools
TIMEOUT /T 8
#endif
#endif

set OBJDUMP=C:\JL\pi32\bin\llvm-objdump.exe
set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy.exe
set ELFFILE=sdk.elf

REM %OBJDUMP% -D -address-mask=0x1ffffff -print-dbg %ELFFILE% > sdk.lst
%OBJCOPY% -O binary -j .text %ELFFILE% text.bin
%OBJCOPY% -O binary -j .data %ELFFILE% data.bin
%OBJCOPY% -O binary -j .ram0_data  %ELFFILE% ram0_data.bin
%OBJCOPY% -O binary -j .cache_ram_data  %ELFFILE% cache_ram_data.bin

%OBJDUMP% -section-headers -address-mask=0x1ffffff %ELFFILE%
%OBJDUMP% -t %ELFFILE% > symbol_tbl.txt

copy /b text.bin+data.bin+ram0_data.bin+cache_ram_data.bin app.bin

#if defined CONFIG_UI_ENABLE
#if defined CONFIG_UI_FILE_SAVE_IN_RESERVED_EXPAND_ZONE
packres\packres.exe -n ui -o packres/UIPACKRES ui_res
#else
set UI_RES=ui_res
#endif
#endif

#if defined CONFIG_AUDIO_ENABLE
#if defined CONFIG_VOICE_PROMPT_FILE_PATH && \
defined CONFIG_VOICE_PROMPT_FILE_SAVE_IN_RESERVED_EXPAND_ZONE
packres\packres.exe -n tone -o packres/AUPACKRES audlogo
#else
set AUDIO_RES=audlogo
#endif
#endif

REM set KEY_FILE=-key JL_791N-XXXX.key
REM set KEY_FILE=-key1 JL_791N-XXXX.key1 -mkey JL_791N-XXXX.mkey

set CFG_FILE=cfg

REM  添加新参数-update_files说明
REM  1/normal就是普通的文件目录结构
REM  2/embedded就是每个文件数据前都会有一个头

REM  只生成code + res升级文件
REM  -update_files normal
REM  生成的文件名字为：db_update_files_data.bin

REM  只生成预留区资源升级文件
REM  -update_files embedded_only $(files) ,其中$(files)为需要添加的资源文件
REM  生成的文件名字为：db_update_files_data.bin

REM  生成code + res + 预留区资源升级文件
REM  -update_files embedded $(files) ,其中$(files)为需要添加的资源文件
REM  生成的文件名字为：db_update_files_data.bin

#if CONFIG_DOUBLE_BANK_ENABLE
set UPDATE_FILES=-update_files normal
#endif

#if defined CONFIG_SDFILE_EXT_ENABLE
fat_image_tool.exe --size CONFIG_SDNAND_HFAT_LEN_TEXT --sectors-per-cluster CONFIG_SDNAND_HFAT_CLUSTER_SIZE --lfn true --n-root 512 --volume-name HFAT_IMG --output jl_hfat.bin --fat-dir hfat_dir
fat_image_tool.exe --size CONFIG_SDNAND_FAT1_LEN_TEXT --sectors-per-cluster CONFIG_SDNAND_FAT1_CLUSTER_SIZE --lfn true --n-root 512 --volume-name FAT1_IMG --output jl_fat1.bin --fat-dir fat1_dir
isd_download.exe isd_config.ini -gen2 -to-sdcard -dev wl82 -boot 0x1c02000 -div1 -app app.bin cfg_tool.bin -res %AUDIO_RES% %UI_RES% cfg -extend-bin -output-bin jl_hfs.bin %UPDATE_FILES% -no-app-bin-enc
#elif defined CONFIG_SFC_ENABLE
isd_download.exe isd_config.ini -gen2 -tonorflash -dev wl82 -boot 0x1c02000 -div1 -wait 300 -uboot uboot.boot -app app.bin cfg_tool.bin -res %AUDIO_RES% %UI_RES% %CFG_FILE% -reboot 500 %KEY_FILE% %UPDATE_FILES% -extend-bin
#else
REM @@@@@@@无效参数
set run_addr=0x2000
set load_addr=0x4000
set mask_addr=0x100000
uboot_lz4.exe app.bin app.lz4 %run_addr% %load_addr% rom.image %mask_addr%
REM @@@@@@@@@@@@@@@

isd_download.exe isd_config.ini -gen2 -tonorflash -dev wl82 -boot 0x1c02000 -div1 -wait 300 -uboot uboot.boot -app app.lz4 cfg_tool.bin -res %AUDIO_RES% %UI_RES% %CFG_FILE% -reboot 500 %KEY_FILE% %UPDATE_FILES% -extend-bin
#endif


@REM 常用命令说明
@rem -format vm         // 擦除VM 区域
@rem -format all        // 擦除所有
@rem -reboot 500        // reset chip, valid in JTAG debug

echo %errorlevel%

@REM 删除临时文件
if exist *.mp3 del *.mp3
if exist *.PIX del *.PIX
if exist *.TAB del *.TAB
if exist *.res del *.res
if exist *.sty del *.sty

#if defined CONFIG_SDFILE_EXT_ENABLE
copy isd_config_loader.ini loader_tools\isd_config.ini
copy jl_hfs.bin loader_tools\jl_hfs.bin
copy jl_hfat.bin loader_tools\jl_hfat.bin
copy jl_fat1.bin loader_tools\jl_fat1.bin
#else
#if !(CONFIG_DOUBLE_BANK_ENABLE)
@REM 生成固件升级文件
fw_add.exe -noenc -fw jl_isd.fw -add ota.bin -type 100 -out jl_isd.fw
#endif

@REM 添加配置脚本的版本信息到 FW 文件中
fw_add.exe -noenc -fw jl_isd.fw -add script.ver -out jl_isd.fw

ufw_maker.exe -fw_to_ufw jl_isd.fw
#endif

@REM 生成配置文件升级文件
rem ufw_maker.exe -chip AC630N %ADD_KEY% -output config.ufw -res bt_cfg.cfg

ping /n 2 127.1>null
IF EXIST null del null

::del app.bin
del cache_ram_data.bin
del data.bin
del ram0_data.bin
del text.bin

#if defined CONFIG_SDFILE_EXT_ENABLE
del jl_hfs.bin
del jl_hfat.bin
del jl_fat1.bin
cd loader_tools
call download.bat
cd ..\

#endif

::退出当前批处理加返回值(0),才能多个批处理嵌套调用批处理
exit /b 0

#endif
// *INDENT-ON*
