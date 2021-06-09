#define MyAppName "writernote"
#define MyAppVersion "_"
#define MyAppPublisher "Writernote"
#define MyAppURL "https://github.com/giacomogroppi/writernote-qt"
#define MyAppExeName "writernote.exe"
#define MyAppAssocName "Writernote"
#define MyAppAssocExt ".writer"
#define MyAppAssocKey StringChange(MyAppAssocName, " ", "") + MyAppAssocExt
#define IconPath "..\images\icon-writernote.ico"
;#define IconPath "C:\Users\giamg\Desktop\PROVA.ico"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{36213220-64DC-43B3-9019-B5B18DE05E89}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
ChangesAssociations=yes
DisableProgramGroupPage=yes

LicenseFile=..\LICENSE

SetupIconFile={#IconPath}

; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputBaseFilename=writernote_setup
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: modifypath; Description: "Add ""{app}"" to your PATH";
Name: fileassoc; Description: "Associate .writer files with Writernote"; Flags: unchecked

[Files]
Source: "..\build\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs                        
Source: "..\updater\build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "..\updater\build\updater.exe"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs      
Source: "{#IconPath}"; DestDir: "{app}"
Source: "C:\msys64\mingw64\lib\libssl.dll.a"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Registry]
Root: HKCR; Subkey: ".writer"; ValueType: string; ValueName: ""; ValueData: "writernote"; Flags: uninsdeletevalue; Tasks: fileassoc
Root: HKCR; Subkey: "writernote"; ValueType: string; ValueName: ""; ValueData: "Writernote File"; Flags: uninsdeletekey; Tasks: fileassoc
Root: HKCR; Subkey: "writernote\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\icon-writernote.ico,0"; Tasks: fileassoc
Root: HKCR; Subkey: "writernote\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\writernote.exe"" ""%1"""; Tasks: fileassoc

[Icons]
Name: "{group}\Writernote"; Filename: "{app}\writernote.exe"; IconFilename: "{app}\icon-writernote.ico" 
; Name: "{group}\writernote"; Filename: "{app}\writernote.exe"; IconFilename: {app}\icon-writernote.ico

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
const
	ModPathName = 'modifypath';
	ModPathType = 'user';
function ModPathDir(): TArrayOfString;
begin
	setArrayLength(Result, 1);
	Result[0] := ExpandConstant('{app}');
end;