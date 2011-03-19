!include MUI2.nsh
; Converter.nsi
; Installation script for Forver2-Converter 
; Useful constants
  !define PRODUCT_NAME              "Forver2-Converter"
  !define PRODUCT_NAME_SHORT        "frvconv"  
  !define PRODUCT_VERSION           "1.0.0"            
  !define PRODUCT_FILENAME          "Demo.Converter.exe"              
  !define PRODUCT_UNFILENAME        "uninst.exe"                                                      
  !define PRODUCT_PUBLISHER         "Alexey Kulyasov"
  !define PRODUCT_DESCRIPTION       "FORVER2-Converter"
  !define LICENSE_NAME              "LGPL v2.1 1999"
  !define PRODUCT_WEB_SITE          "http://github.com/lyohytch/FORVER2---Converter"                                           
  !define /date                     NOW "%Y%m%d"    
;Compression level
SetCompress force
SetCompressor /SOLID lzma
; Installer settings
Name "${PRODUCT_NAME}"
Caption "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\${PRODUCT_NAME_SHORT}-${PRODUCT_VERSION}_build-${NOW}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
;set installer icon
!define MUI_ICON "Resources\Icons\converter.ico"
;Interface settings
!define MUI_ABORTWARNING
;Pages
;installer
!insertmacro MUI_PAGE_LICENSE "LICENSE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
;uninstaller
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
;Language
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"
;Reserve Files
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "Main" mainSec
  SectionIn RO
  SetOutPath $INSTDIR\Main\bin
  File "build\bin\*.*"
  File "Resources\Libs\*.*"
  File /r "Resources\Libs\sqldrivers"
  ; uninstall.exe in $INSTDIR
  writeUninstaller "${PRODUCT_UNFILENAME}"
SectionEnd

;Installer Functions
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "Resources" resSec
  SetOutPath $INSTDIR\Resources
  File /r /x "Libs" "Resources\*"
SectionEnd  

Section "Source files" srcSec
   SetOutPath $INSTDIR\Src
   File  "*.pro"
   File  "*.pri"
   File  "*.qrc"
   File  "*.rc"
   File  "*.py"
   SetOutPath $INSTDIR\Src\build
   File /r /x "Makefile*" /x "bin" /x "lib" /x "mocs" /x "objs" /x "res" "build\*.*" 
   SetOutPath $INSTDIR\Src\model
   File /r /x "Makefile*" /x "Doxyfile" "model\*.*"
   SetOutPath $INSTDIR\Src\presenter
   File /r /x "Makefile*" /x "Doxyfile" "presenter\*.*"
   SetOutPath $INSTDIR\Src\query
   File /r /x "Makefile*" /x "Doxyfile" "query\*.*"
   SetOutPath $INSTDIR\Src\ui
   File /r /x "Makefile*" /x "Doxyfile" "ui\*.*"
SectionEnd

Section "Docs" docsSec
  SetOutPath $INSTDIR\Docs
  File /r /x "*.rar" "Docs\*"
SectionEnd
  
Section "Start menu Shortcuts" shortcutSec
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  SetOutPath "$INSTDIR\Main\bin"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\Main\bin\${PRODUCT_FILENAME}" "" "$INSTDIR\Main\bin\${PRODUCT_FILENAME}" 0
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\Main\bin\${PRODUCT_FILENAME}" "" "$INSTDIR\Main\bin\${PRODUCT_FILENAME}" 0
  SetOutPath "$INSTDIR"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_UNFILENAME}.lnk" "$INSTDIR\${PRODUCT_UNFILENAME}" "" "$INSTDIR\${PRODUCT_UNFILENAME}" 0
SectionEnd 

;Russian
LangString descMain ${LANG_RUSSIAN} "Основные файлы программы: исполняемые и динамические библиотеки"
LangString descRes ${LANG_RUSSIAN} "Ресурсы: примерные файлы данных, перевод, иконки"
LangString descSrc ${LANG_RUSSIAN} "Исходный код"
LangString descDocs ${LANG_RUSSIAN} "Документация исходного кода"
LangString descShort ${LANG_RUSSIAN} "Значки в меню и на рабочем столе"
;English
LangString descMain ${LANG_ENGLISH} "Main (executable files and dlls)"
LangString descRes ${LANG_ENGLISH} "Resource files: example data file, translation, icons"
LangString descSrc ${LANG_ENGLISH} "Source files"
LangString descDocs ${LANG_ENGLISH} "Documentation of source code"
LangString descShort ${LANG_ENGLISH} "Shourtcut files"



!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${mainSec} $(descMain)
  !insertmacro MUI_DESCRIPTION_TEXT ${resSec} $(descRes)
  !insertmacro MUI_DESCRIPTION_TEXT ${srcSec} $(descSrc)
  !insertmacro MUI_DESCRIPTION_TEXT ${docsSec} $(descDocs)
  !insertmacro MUI_DESCRIPTION_TEXT ${shortcutSec} $(descShort)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"
  Delete $INSTDIR\${PRODUCT_UNFILENAME}
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\*.*"
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  
  RMDir /r "$INSTDIR\Main"
  RMDir /r "$INSTDIR\Src"
  RMDir /r "$INSTDIR\Docs"
  RMDir /r "$INSTDIR\Resources"
  
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"
  RMDir "$INSTDIR"
SectionEnd 
; info in file
  VIProductVersion ${PRODUCT_VERSION}.0
  VIAddVersionKey /LANG=${LANG_RUSSIAN} ProductName     "${PRODUCT_NAME}"
  VIAddVersionKey /LANG=${LANG_RUSSIAN} ProductVersion  "${PRODUCT_VERSION}"
  VIAddVersionKey /LANG=${LANG_RUSSIAN} CompanyName     "${PRODUCT_PUBLISHER}"
  VIAddVersionKey /LANG=${LANG_RUSSIAN} CompanyWebsite  "${PRODUCT_WEB_SITE}"
  VIAddVersionKey /LANG=${LANG_RUSSIAN} FileVersion     "${PRODUCT_VERSION}"
  VIAddVersionKey /LANG=${LANG_RUSSIAN} FileDescription "${PRODUCT_DESCRIPTION}"
  VIAddVersionKey /LANG=${LANG_RUSSIAN} LegalCopyright  "${LICENSE_NAME}"
  VIAddVersionKey /LANG=${LANG_RUSSIAN} Comments        "Windows XP, Vista, Win7 $\t"
  VIAddVersionKey /LANG=${LANG_RUSSIAN} OriginalFilename "${PRODUCT_FILENAME}"