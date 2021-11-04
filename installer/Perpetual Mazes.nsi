############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "Perpetual Mazes"
!define COMP_NAME "Shup & Co."
!define WEB_SITE "https://shupantha.wordpress.com/2019/10/26/perpetual-mazes/"
!define VERSION "10.00.00.01"
!define COPYRIGHT "Shup � 2021"
!define DESCRIPTION "Maze Game"
!define INSTALLER_NAME "C:\bitbucket\pm_s2d_console\installer\pm-setup.exe"
!define MAIN_APP_EXE "pm_s2d_console.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\perpetualmazes"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Perpetual Mazes"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "C:\bitbucket\pm_s2d_console\bin\glew32.dll"
File "C:\bitbucket\pm_s2d_console\bin\libFLAC-8.dll"
File "C:\bitbucket\pm_s2d_console\bin\libfreetype-6.dll"
File "C:\bitbucket\pm_s2d_console\bin\libjpeg-9.dll"
File "C:\bitbucket\pm_s2d_console\bin\libmodplug-1.dll"
File "C:\bitbucket\pm_s2d_console\bin\libmpg123-0.dll"
File "C:\bitbucket\pm_s2d_console\bin\libogg-0.dll"
File "C:\bitbucket\pm_s2d_console\bin\libopus-0.dll"
File "C:\bitbucket\pm_s2d_console\bin\libopusfile-0.dll"
File "C:\bitbucket\pm_s2d_console\bin\libpng16-16.dll"
File "C:\bitbucket\pm_s2d_console\bin\libtiff-5.dll"
File "C:\bitbucket\pm_s2d_console\bin\libvorbis-0.dll"
File "C:\bitbucket\pm_s2d_console\bin\libvorbisfile-3.dll"
File "C:\bitbucket\pm_s2d_console\bin\libwebp-7.dll"
File "C:\bitbucket\pm_s2d_console\bin\pm_s2d_console.exe"
File "C:\bitbucket\pm_s2d_console\bin\SDL2.dll"
File "C:\bitbucket\pm_s2d_console\bin\SDL2_image.dll"
File "C:\bitbucket\pm_s2d_console\bin\SDL2_mixer.dll"
File "C:\bitbucket\pm_s2d_console\bin\SDL2_ttf.dll"
File "C:\bitbucket\pm_s2d_console\bin\simple2d.dll"
File "C:\bitbucket\pm_s2d_console\bin\zlib1.dll"
SetOutPath "$INSTDIR\sfx"
File "C:\bitbucket\pm_s2d_console\bin\sfx\bg01.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\bg02.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\bg03.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\bg04.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\bg05.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\credits.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\eatdragon.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\eatkiwi.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\eatmango.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\eatmonster.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\eatorange.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\eatpill.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\eatpower.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\endlevel.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\gameover.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\killrunner.wav"
File "C:\bitbucket\pm_s2d_console\bin\sfx\startLevel.wav"
SetOutPath "$INSTDIR\msc"
File "C:\bitbucket\pm_s2d_console\bin\msc\settings - default.txt"
File "C:\bitbucket\pm_s2d_console\bin\msc\settings - red.txt"
File "C:\bitbucket\pm_s2d_console\bin\msc\settings.txt"
File "C:\bitbucket\pm_s2d_console\bin\msc\Vera.ttf"
SetOutPath "$INSTDIR\gfx"
File "C:\bitbucket\pm_s2d_console\bin\gfx\Thumbs.db"
SetOutPath "$INSTDIR\gfx\r"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\d0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\d0_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\d1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\d1_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\d2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\d2_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r0_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r1_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r2_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r3.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r3_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r4.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r4_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r5.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\r5_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\r\Thumbs.db"
SetOutPath "$INSTDIR\gfx\pills"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\d0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\d1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\d2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\k0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\k1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\k2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\m0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\m1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\m2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\n0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\n1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\n2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\o0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\o1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\o2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\p0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\p1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\p2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\pills\Thumbs.db"
SetOutPath "$INSTDIR\gfx\monsters"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky0_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky1_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky2_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky3.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky3_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky4.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky4_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky5.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\binky5_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead0_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead1_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead2_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead3.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead3_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead4.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead4_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead5.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\dead5_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible0_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible1_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible2_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible3.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible3_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible4.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible4_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible5.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\edible5_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky0_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky1_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky2_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky3.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky3_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky4.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky4_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky5.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\ginky5_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky0_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky1_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky2_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky3.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky3_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky4.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky4_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky5.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\rinky5_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\Thumbs.db"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky0_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky1_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky2_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky3.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky3_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky4.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky4_.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky5.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\monsters\yinky5_.png"
SetOutPath "$INSTDIR\gfx\doors"
File "C:\bitbucket\pm_s2d_console\bin\gfx\doors\en0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\doors\en1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\doors\en2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\doors\ex0.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\doors\ex1.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\doors\ex2.png"
File "C:\bitbucket\pm_s2d_console\bin\gfx\doors\Thumbs.db"
SetOutPath "$INSTDIR\gfx\common"
File "C:\bitbucket\pm_s2d_console\bin\gfx\common\bg.bmp"
File "C:\bitbucket\pm_s2d_console\bin\gfx\common\st.bmp"
File "C:\bitbucket\pm_s2d_console\bin\gfx\common\Thumbs.db"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\Perpetual Mazes"
CreateShortCut "$SMPROGRAMS\Perpetual Mazes\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\Perpetual Mazes\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\Perpetual Mazes\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\glew32.dll"
Delete "$INSTDIR\libFLAC-8.dll"
Delete "$INSTDIR\libfreetype-6.dll"
Delete "$INSTDIR\libjpeg-9.dll"
Delete "$INSTDIR\libmodplug-1.dll"
Delete "$INSTDIR\libmpg123-0.dll"
Delete "$INSTDIR\libogg-0.dll"
Delete "$INSTDIR\libopus-0.dll"
Delete "$INSTDIR\libopusfile-0.dll"
Delete "$INSTDIR\libpng16-16.dll"
Delete "$INSTDIR\libtiff-5.dll"
Delete "$INSTDIR\libvorbis-0.dll"
Delete "$INSTDIR\libvorbisfile-3.dll"
Delete "$INSTDIR\libwebp-7.dll"
Delete "$INSTDIR\pm_s2d_console.exe"
Delete "$INSTDIR\SDL2.dll"
Delete "$INSTDIR\SDL2_image.dll"
Delete "$INSTDIR\SDL2_mixer.dll"
Delete "$INSTDIR\SDL2_ttf.dll"
Delete "$INSTDIR\simple2d.dll"
Delete "$INSTDIR\zlib1.dll"
Delete "$INSTDIR\sfx\bg01.wav"
Delete "$INSTDIR\sfx\bg02.wav"
Delete "$INSTDIR\sfx\bg03.wav"
Delete "$INSTDIR\sfx\bg04.wav"
Delete "$INSTDIR\sfx\bg05.wav"
Delete "$INSTDIR\sfx\credits.wav"
Delete "$INSTDIR\sfx\eatdragon.wav"
Delete "$INSTDIR\sfx\eatkiwi.wav"
Delete "$INSTDIR\sfx\eatmango.wav"
Delete "$INSTDIR\sfx\eatmonster.wav"
Delete "$INSTDIR\sfx\eatorange.wav"
Delete "$INSTDIR\sfx\eatpill.wav"
Delete "$INSTDIR\sfx\eatpower.wav"
Delete "$INSTDIR\sfx\endlevel.wav"
Delete "$INSTDIR\sfx\gameover.wav"
Delete "$INSTDIR\sfx\killrunner.wav"
Delete "$INSTDIR\sfx\startLevel.wav"
Delete "$INSTDIR\msc\settings - default.txt"
Delete "$INSTDIR\msc\settings - red.txt"
Delete "$INSTDIR\msc\settings.txt"
Delete "$INSTDIR\msc\Vera.ttf"
Delete "$INSTDIR\gfx\Thumbs.db"
Delete "$INSTDIR\gfx\r\d0.png"
Delete "$INSTDIR\gfx\r\d0_.png"
Delete "$INSTDIR\gfx\r\d1.png"
Delete "$INSTDIR\gfx\r\d1_.png"
Delete "$INSTDIR\gfx\r\d2.png"
Delete "$INSTDIR\gfx\r\d2_.png"
Delete "$INSTDIR\gfx\r\r0.png"
Delete "$INSTDIR\gfx\r\r0_.png"
Delete "$INSTDIR\gfx\r\r1.png"
Delete "$INSTDIR\gfx\r\r1_.png"
Delete "$INSTDIR\gfx\r\r2.png"
Delete "$INSTDIR\gfx\r\r2_.png"
Delete "$INSTDIR\gfx\r\r3.png"
Delete "$INSTDIR\gfx\r\r3_.png"
Delete "$INSTDIR\gfx\r\r4.png"
Delete "$INSTDIR\gfx\r\r4_.png"
Delete "$INSTDIR\gfx\r\r5.png"
Delete "$INSTDIR\gfx\r\r5_.png"
Delete "$INSTDIR\gfx\r\Thumbs.db"
Delete "$INSTDIR\gfx\pills\d0.png"
Delete "$INSTDIR\gfx\pills\d1.png"
Delete "$INSTDIR\gfx\pills\d2.png"
Delete "$INSTDIR\gfx\pills\k0.png"
Delete "$INSTDIR\gfx\pills\k1.png"
Delete "$INSTDIR\gfx\pills\k2.png"
Delete "$INSTDIR\gfx\pills\m0.png"
Delete "$INSTDIR\gfx\pills\m1.png"
Delete "$INSTDIR\gfx\pills\m2.png"
Delete "$INSTDIR\gfx\pills\n0.png"
Delete "$INSTDIR\gfx\pills\n1.png"
Delete "$INSTDIR\gfx\pills\n2.png"
Delete "$INSTDIR\gfx\pills\o0.png"
Delete "$INSTDIR\gfx\pills\o1.png"
Delete "$INSTDIR\gfx\pills\o2.png"
Delete "$INSTDIR\gfx\pills\p0.png"
Delete "$INSTDIR\gfx\pills\p1.png"
Delete "$INSTDIR\gfx\pills\p2.png"
Delete "$INSTDIR\gfx\pills\Thumbs.db"
Delete "$INSTDIR\gfx\monsters\binky0.png"
Delete "$INSTDIR\gfx\monsters\binky0_.png"
Delete "$INSTDIR\gfx\monsters\binky1.png"
Delete "$INSTDIR\gfx\monsters\binky1_.png"
Delete "$INSTDIR\gfx\monsters\binky2.png"
Delete "$INSTDIR\gfx\monsters\binky2_.png"
Delete "$INSTDIR\gfx\monsters\binky3.png"
Delete "$INSTDIR\gfx\monsters\binky3_.png"
Delete "$INSTDIR\gfx\monsters\binky4.png"
Delete "$INSTDIR\gfx\monsters\binky4_.png"
Delete "$INSTDIR\gfx\monsters\binky5.png"
Delete "$INSTDIR\gfx\monsters\binky5_.png"
Delete "$INSTDIR\gfx\monsters\dead0.png"
Delete "$INSTDIR\gfx\monsters\dead0_.png"
Delete "$INSTDIR\gfx\monsters\dead1.png"
Delete "$INSTDIR\gfx\monsters\dead1_.png"
Delete "$INSTDIR\gfx\monsters\dead2.png"
Delete "$INSTDIR\gfx\monsters\dead2_.png"
Delete "$INSTDIR\gfx\monsters\dead3.png"
Delete "$INSTDIR\gfx\monsters\dead3_.png"
Delete "$INSTDIR\gfx\monsters\dead4.png"
Delete "$INSTDIR\gfx\monsters\dead4_.png"
Delete "$INSTDIR\gfx\monsters\dead5.png"
Delete "$INSTDIR\gfx\monsters\dead5_.png"
Delete "$INSTDIR\gfx\monsters\edible0.png"
Delete "$INSTDIR\gfx\monsters\edible0_.png"
Delete "$INSTDIR\gfx\monsters\edible1.png"
Delete "$INSTDIR\gfx\monsters\edible1_.png"
Delete "$INSTDIR\gfx\monsters\edible2.png"
Delete "$INSTDIR\gfx\monsters\edible2_.png"
Delete "$INSTDIR\gfx\monsters\edible3.png"
Delete "$INSTDIR\gfx\monsters\edible3_.png"
Delete "$INSTDIR\gfx\monsters\edible4.png"
Delete "$INSTDIR\gfx\monsters\edible4_.png"
Delete "$INSTDIR\gfx\monsters\edible5.png"
Delete "$INSTDIR\gfx\monsters\edible5_.png"
Delete "$INSTDIR\gfx\monsters\ginky0.png"
Delete "$INSTDIR\gfx\monsters\ginky0_.png"
Delete "$INSTDIR\gfx\monsters\ginky1.png"
Delete "$INSTDIR\gfx\monsters\ginky1_.png"
Delete "$INSTDIR\gfx\monsters\ginky2.png"
Delete "$INSTDIR\gfx\monsters\ginky2_.png"
Delete "$INSTDIR\gfx\monsters\ginky3.png"
Delete "$INSTDIR\gfx\monsters\ginky3_.png"
Delete "$INSTDIR\gfx\monsters\ginky4.png"
Delete "$INSTDIR\gfx\monsters\ginky4_.png"
Delete "$INSTDIR\gfx\monsters\ginky5.png"
Delete "$INSTDIR\gfx\monsters\ginky5_.png"
Delete "$INSTDIR\gfx\monsters\rinky0.png"
Delete "$INSTDIR\gfx\monsters\rinky0_.png"
Delete "$INSTDIR\gfx\monsters\rinky1.png"
Delete "$INSTDIR\gfx\monsters\rinky1_.png"
Delete "$INSTDIR\gfx\monsters\rinky2.png"
Delete "$INSTDIR\gfx\monsters\rinky2_.png"
Delete "$INSTDIR\gfx\monsters\rinky3.png"
Delete "$INSTDIR\gfx\monsters\rinky3_.png"
Delete "$INSTDIR\gfx\monsters\rinky4.png"
Delete "$INSTDIR\gfx\monsters\rinky4_.png"
Delete "$INSTDIR\gfx\monsters\rinky5.png"
Delete "$INSTDIR\gfx\monsters\rinky5_.png"
Delete "$INSTDIR\gfx\monsters\Thumbs.db"
Delete "$INSTDIR\gfx\monsters\yinky0.png"
Delete "$INSTDIR\gfx\monsters\yinky0_.png"
Delete "$INSTDIR\gfx\monsters\yinky1.png"
Delete "$INSTDIR\gfx\monsters\yinky1_.png"
Delete "$INSTDIR\gfx\monsters\yinky2.png"
Delete "$INSTDIR\gfx\monsters\yinky2_.png"
Delete "$INSTDIR\gfx\monsters\yinky3.png"
Delete "$INSTDIR\gfx\monsters\yinky3_.png"
Delete "$INSTDIR\gfx\monsters\yinky4.png"
Delete "$INSTDIR\gfx\monsters\yinky4_.png"
Delete "$INSTDIR\gfx\monsters\yinky5.png"
Delete "$INSTDIR\gfx\monsters\yinky5_.png"
Delete "$INSTDIR\gfx\doors\en0.png"
Delete "$INSTDIR\gfx\doors\en1.png"
Delete "$INSTDIR\gfx\doors\en2.png"
Delete "$INSTDIR\gfx\doors\ex0.png"
Delete "$INSTDIR\gfx\doors\ex1.png"
Delete "$INSTDIR\gfx\doors\ex2.png"
Delete "$INSTDIR\gfx\doors\Thumbs.db"
Delete "$INSTDIR\gfx\common\bg.bmp"
Delete "$INSTDIR\gfx\common\st.bmp"
Delete "$INSTDIR\gfx\common\Thumbs.db"
 
RmDir "$INSTDIR\gfx\common"
RmDir "$INSTDIR\gfx\doors"
RmDir "$INSTDIR\gfx\monsters"
RmDir "$INSTDIR\gfx\pills"
RmDir "$INSTDIR\gfx\r"
RmDir "$INSTDIR\gfx"
RmDir "$INSTDIR\msc"
RmDir "$INSTDIR\sfx"
 
Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\Perpetual Mazes\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\Perpetual Mazes\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\Perpetual Mazes\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\Perpetual Mazes"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################
