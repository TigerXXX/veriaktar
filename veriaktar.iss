; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "veriaktar"
#define MyAppVersion "1"
#define MyAppPublisher "M�min G�LER"
#define MyAppURL "http://veriaktar.blogspot.com.tr/"
#define MyAppExeName "veriaktar.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{50B9951A-0AB0-46BC-B1FB-FDAEE52F08B3}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar
OutputBaseFilename=veriaktar_setup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\Release\veriaktar.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\BIST100.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\BIST30.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\BIST50.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\BISTENDEKSLER.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\BISTTUM.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\BOLUNMELER.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\x64\Documents\Visual Studio 2015\Projects\veriaktar\veriaktar\VERSIYON.txt"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Flags: nowait postinstall skipifsilent runascurrentuser; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"
