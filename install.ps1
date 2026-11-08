# Installs the Skin plugin and the Midnight skin for the current user. Run from the unzipped release folder.
$ErrorActionPreference = 'Stop'
$cfg = Join-Path $env:APPDATA 'OpenRGB'
if (Get-Process OpenRGB -ErrorAction SilentlyContinue | Where-Object SessionId -ne 0) {
    Write-Warning 'Close OpenRGB first (tray icon > Exit), then run this again.'; exit 1
}
New-Item -ItemType Directory -Force (Join-Path $cfg 'plugins') | Out-Null
Copy-Item (Join-Path $PSScriptRoot 'plugins\*.dll') (Join-Path $cfg 'plugins') -Force
$qss = Join-Path $cfg 'skin.qss'
if (Test-Path $qss) { Copy-Item $qss "$qss.bak" -Force; Write-Host "Existing skin.qss backed up to skin.qss.bak" }
Copy-Item (Join-Path $PSScriptRoot 'skin\midnight.qss') $qss -Force
Copy-Item (Join-Path $PSScriptRoot 'skin\*.png') $cfg -Force
Write-Host "Installed to $cfg. Start OpenRGB."
