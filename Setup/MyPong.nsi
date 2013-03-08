############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "MyPong"
!define COMP_NAME "BGI"
!define WEB_SITE "http://www.gamedev.net/blog/1369-speedruns-journal/"
!define PRODUCT_VERSION "2.0"
!define VERSION "00.00.02.00"
!define COPYRIGHT "AnkurSheel©2012"
!define DESCRIPTION "Application"
!define INSTALLER_NAME "C:\Users\SpeedRun\Projects\sppong\Setup\setup.exe"
!define MAIN_APP_EXE "Main.exe"
!define INSTALL_TYPE "SetShellVarContext all"
!define REG_ROOT "HKLM"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!define REG_START_MENU "Start Menu Folder"

var SM_Folder

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
InstallDir "C:\Games\MyPong"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

; Welcome page
!define MUI_WELCOMEPAGE_TITLE "This Will Install MPong(v${PRODUCT_VERSION}) on your computer"
!insertmacro MUI_PAGE_WELCOME
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page

!insertmacro MUI_PAGE_DIRECTORY

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "MyPong"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

######################################################################

AutoCloseWindow false
ShowInstDetails hide 
ShowUnInstDetails hide

Section "Main (Required)" SEC01
  ${INSTALL_TYPE}
  SectionIn RO
  SetOverwrite ifnewer
  SetOutPath "$INSTDIR"
  File "..\Retail\*.dll"
  File "..\Retail\*.exe"
  File "..\Retail\*.ini"
  SetOutPath "$INSTDIR\Media"
# File /r /x .svn "..\Retail\resources\*.*"
  File /r "..\Retail\Media\*.*"
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
CreateDirectory "$SMPROGRAMS\MyPong"
CreateShortCut "$SMPROGRAMS\MyPong\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\MyPong\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\MyPong\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
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

Var DirectXSetupError

Section "DirectX (Recommended)" SEC02
  ${INSTALL_TYPE}
  ; SectionIn RO
# NSISdl::download http://download.microsoft.com/download/1/7/1/1718CCC4-6315-4D8E-9543-8E28A4E18C4C/dxwebsetup.exe $TEMP\dxwebsetup.exe
 SetOutPath "$TEMP"
 File "dxwebsetup.exe"
 DetailPrint "Running DirectX Setup..."
 ExecWait '"$TEMP\dxwebsetup.exe" /Q' $DirectXSetupError
 DetailPrint "Finished DirectX Setup"

 Delete "$TEMP\dxwebsetup.exe"
 SetOutPath "$INSTDIR"

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

######################################################################
; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Main"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Checks And Installs the latest Directx runtime.\
Please make sure you have internet access.\
(Do not uncheck unless you are sure you have the latest DirectX installed)\"
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Section Uninstall
${INSTALL_TYPE}
  Delete "$INSTDIR\*.*"
  Rmdir /r "$INSTDIR\Media"
	 
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
  Delete "$SMPROGRAMS\MyPong\${APP_NAME}.lnk"
  Delete "$SMPROGRAMS\MyPong\Uninstall ${APP_NAME}.lnk"
  !ifdef WEB_SITE
  Delete "$SMPROGRAMS\MyPong\${APP_NAME} Website.lnk"
  !endif
  Delete "$DESKTOP\${APP_NAME}.lnk"

  RmDir "$SMPROGRAMS\MyPong"
  !endif

  DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
  DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"

SectionEnd

######################################################################
Function UninstallMe
  Delete "$INSTDIR\*.*"
  Rmdir /r "$INSTDIR\Media"
	 
  Delete "$INSTDIR\uninstall.exe"
  !ifdef WEB_SITE
  Delete "$INSTDIR\${APP_NAME} website.url"
  !endif

  RmDir "$INSTDIR"

  !ifndef NEVER_UNINSTALL
  Delete "$FONTS\VLADIMIR.TTF"
  !endif

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
  Delete "$SMPROGRAMS\MyPong\${APP_NAME}.lnk"
  Delete "$SMPROGRAMS\MyPong\Uninstall ${APP_NAME}.lnk"
  !ifdef WEB_SITE
  Delete "$SMPROGRAMS\MyPong\${APP_NAME} Website.lnk"
  !endif
  Delete "$DESKTOP\${APP_NAME}.lnk"

  RmDir "$SMPROGRAMS\MyPong"
  !endif

  DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
  DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"

FunctionEnd

Function .onInit
 
  ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
  "UninstallString"
  StrCmp $R0 "" done
 
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "${APP_NAME} is already installed. $\n$\nClick `OK` to remove the \
  previous version or `Cancel` to cancel this upgrade." \
  IDOK uninst
  Abort
 
;Run the uninstaller
uninst:
  ClearErrors
Call UninstallMe
 
done:
 
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd