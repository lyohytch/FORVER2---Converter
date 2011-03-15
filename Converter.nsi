!include MUI2.nsh
; Converter.nsi 
; Script for converter installer

; General---------------------------------------------
Name "Forver2-Converter"
Caption "Converter for FORVER2"
OutFile "install.exe"
InstallDir "$PROGRAMFILES\NNSU\Forver2-Converter"
RequestExecutionLevel admin

;Interface settings
!define MUI_ABORTWARNING

;Pages
!insertmacro MUI_PAGE_LICENSE "LICENSE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------------------- 
;Language
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"

;Reserve Files
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "Main" mainSec
  SectionIn RO
  SetOutPath $INSTDIR\Main\bin
  File "Converter.nsi"
  ; uninstall.exe in $INSTDIR
  writeUninstaller "..\..\uninstall.exe"
SectionEnd

;--------------------------------
;Installer Functions
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "Resources" resSec
  SetOutPath $INSTDIR\Resources
  File "diplom.pro"
SectionEnd  

Section "Source files" srcSec
   SetOutPath $INSTDIR\Src
   File "diplom.pro"
SectionEnd

Section "Docs" docsSec
  SetOutPath $INSTDIR\Docs
  File "diplom.pro"
SectionEnd
  
Section "Start menu Shortcuts" shortcutSec
  CreateDirectory "$SMPROGRAMS\Forver2-Converter"
  CreateShortCut "$SMPROGRAMS\Forver2-Converter\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Forver2-Converter\Converter.lnk" "$INSTDIR\Main\bin\Converter.nsi" "" "$INSTDIR\Main\bin\Converter.nsi" 0
SectionEnd 

;Write text for Russian
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${mainSec} "Main (executable files and dlls)"
  !insertmacro MUI_DESCRIPTION_TEXT ${resSec} "Resource files: example data file, translation, icons"
  !insertmacro MUI_DESCRIPTION_TEXT ${srcSec} "Source files"
  !insertmacro MUI_DESCRIPTION_TEXT ${docsSec} "Documentation of source code"
  !insertmacro MUI_DESCRIPTION_TEXT ${shortcutSec} "Shourtcut files"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"
  Delete $INSTDIR\Main\bin\Converter.nsi
  Delete $INSTDIR\uninstall.exe
  
  Delete "$SMPROGRAMS\Forver2-Converter\*.*"
  
  RMDir "$SMPROGRAMS\Forver2-Converter"
  RMDir "$INSTDIR"
SectionEnd 
;Uninstaller Functions
Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE  
FunctionEnd