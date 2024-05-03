@echo off

set CompilerFlags=-GR -Od -Oi -WX -FC -Z7
set LinkerFlags=-incremental:no -opt:ref user32.lib gdi32.lib winmm.lib xinput.lib Ole32.lib d3d11.lib dxgi.lib D3DCompiler.lib dxguid.lib

IF NOT EXIST build mkdir build
pushd build

cl %CompilerFlags% ..\win_main.cpp -Fmwin_main.map /link %LinkerFlags%
popd