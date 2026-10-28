param([Parameter(Mandatory=$true)][string]$UpstreamDir)
$ErrorActionPreference='Stop'
$root=$PSScriptRoot
$upstream=(Resolve-Path -LiteralPath $UpstreamDir).Path
$build=Join-Path $root 'build'
New-Item -ItemType Directory -Force "$build/src" | Out-Null
Copy-Item "$upstream/src/*" "$build/src/"
Copy-Item "$root/source/patches.c","$root/source/window_fix.c","$root/source/viewport_math.h" "$build/src/"
$sources='dsetup_shim.c','mci_music.c','file_trace.c','patches.c','window_fix.c','registry_redirect.c','mixer_trace.c','miniaudio_impl.c','ddraw.def' | ForEach-Object {"$build/src/$_"}
& "$upstream/.venv/Scripts/python.exe" -m ziglang cc -target x86-windows-gnu -O2 -shared -DGOC_AS_DDRAW -o "$build/ddraw.dll" -I "$upstream/third_party/miniaudio" @sources -lkernel32 -luser32 -lgdi32 -lole32
if($LASTEXITCODE){throw 'Compatibility build failed'}
Get-FileHash "$build/ddraw.dll"
