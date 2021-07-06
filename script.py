import os

def find_files(filename, search_path):
   result = []

# Wlaking top-down from the root
   for root, dir, files in os.walk(search_path):
      if filename in files:
         result.append(os.path.join(root, filename))
   return result

#print("Trovato: ",find_files("mt.exe","C:"))

['C:Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Bin\\mt.exe',
'C:Program Files (x86)\\Microsoft SDKs\\Windows\\v7.1A\\Bin\\x64\\mt.exe',
'C:Program Files (x86)\\Microsoft Visual Studio\\Shared\\NuGetPackages\\microsoft.windows.sdk.buildtools\\10.0.19041.8\\bin\\10.0.19041.0\\x64\\mt.exe',
'C:Program Files (x86)\\Microsoft Visual Studio\\Shared\\NuGetPackages\\microsoft.windows.sdk.buildtools\\10.0.19041.8\\bin\\10.0.19041.0\\x86\\mt.exe',
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.16299.0\\arm64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.16299.0\\x64\\mt.exe',
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.16299.0\\x86\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.17134.0\\arm64\\mt.exe',
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.17134.0\\x64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.17134.0\\x86\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.17763.0\\arm64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.17763.0\\x64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.17763.0\\x86\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.18362.0\\arm64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.18362.0\\x64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.18362.0\\x86\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.19041.0\\arm64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.19041.0\\x64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\10.0.19041.0\\x86\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\arm64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\x64\\mt.exe', 
'C:Program Files (x86)\\Windows Kits\\10\\bin\\x86\\mt.exe']