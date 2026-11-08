# OpenRGB Skin

<img src="icon.png" width="64" align="right">

An [OpenRGB](https://openrgb.org) plugin that restyles OpenRGB with a Qt stylesheet and darkens the Windows
title bar. Ships with **Midnight**: graphite panels with a violet accent.

- Applies `%APPDATA%\OpenRGB\skin.qss` at startup and **live-reloads it on save**, so you can restyle while OpenRGB runs.
- Images beside the skin can be referenced as `url(skin:name.png)`.
- Dark title bar on every OpenRGB window and dialog (Windows 11).

Works with **OpenRGB 1.1 on Windows x64**.

## Install

1. Download `openrgb-skin-<version>-openrgb-1.1-win64.zip` from [Releases](https://github.com/KrapFey/openrgb-skin/releases) and unzip it.
2. Exit OpenRGB (tray icon > Exit).
3. In the unzipped folder run:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\install.ps1
   ```
4. Start OpenRGB.

Manual install: copy `plugins\OpenRGBSkinPlugin.dll` to `%APPDATA%\OpenRGB\plugins\`, `skin\midnight.qss` to
`%APPDATA%\OpenRGB\skin.qss`, and `skin\*.png` to `%APPDATA%\OpenRGB\`.

## Customise

Edit `%APPDATA%\OpenRGB\skin.qss`; changes apply when you save. The title-bar colours live in `SkinPlugin.cpp`
(`DarkCaption`).

## Uninstall

Delete `OpenRGBSkinPlugin.dll` from `%APPDATA%\OpenRGB\plugins\` (and `skin.qss` plus the PNGs if you like).

## Build from source

Requirements: Visual Studio 2022, CMake 3.21+, and **the same Qt that OpenRGB ships with** (OpenRGB 1.1 =
Qt 6.8.3 `msvc2022_64`). OpenRGB refuses plugins built against a newer Qt.

```powershell
# Qt 6.8.3 (core only, ~270 MB)
pip install aqtinstall
aqt install-qt windows desktop 6.8.3 win64_msvc2022_64 --archives qtbase --outputdir C:\Qt-6.8.3

# OpenRGB headers at the 1.1 release commit (headers only, no full checkout)
git clone --filter=blob:none --no-checkout https://gitlab.com/CalcProgrammer1/OpenRGB.git openrgb-src
git -C openrgb-src sparse-checkout set --no-cone '/*.h' '/RGBController/*.h' '/dependencies/json/**'
git -C openrgb-src checkout 8e16de27858cf2be6af54a33ce003df74cb9fd3e

cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=C:\Qt-6.8.3\6.8.3\msvc2022_64 -DOPENRGB_SRC=..\openrgb-src
cmake --build build --config Release                    # plugin DLL
cmake --build build --config Release --target package   # release zip
```

## License

GPL-2.0-or-later, the same as OpenRGB, whose headers this plugin builds against. See [LICENSE](LICENSE).
