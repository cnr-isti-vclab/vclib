$vsPath = & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath
$vcvars = "$vsPath\VC\Auxiliary\Build\vcvars64.bat"

$projectRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)

if (Test-Path $vcvars) {
    Write-Host "Loading MSVC from: $vcvars" -ForegroundColor Green
    
    Push-Location $projectRoot
    try {
        $venvActivate = "vclib_build_venv\Scripts\activate.bat"
        if (Test-Path $venvActivate) {
            Write-Host "Activating virtual environment..." -ForegroundColor Cyan
            cmd.exe /c "`"$venvActivate`" && `"$vcvars`" && set CC=cl && set CXX=cl && python -m build --wheel --outdir dist"
            $wheelFile = Get-ChildItem -Path "dist\vclib-*.whl" | Select-Object -First 1 -ExpandProperty FullName
            if ($wheelFile) {
                cmd.exe /c "`"$venvActivate`" && `"$vcvars`" && python -m pip install --force-reinstall `"$wheelFile`""

                Write-Host "To test any library example, remember to activate the virtual environment first:`n"
                Write-Host "  .\vclib_build_venv\Scripts\Activate.ps1`n" -ForegroundColor Cyan
            } else {
                Write-Error "No wheel found in dist folder!"
            }
        } else {
            Write-Error "Virtual environment not found. Run scripts\wheel\create_and_activate_venv.ps1 first!"
        }
    } finally {
        Pop-Location
    }
} else {
    Write-Error "vcvars64.bat not found. Ensure Visual Studio is installed with C++ tools."
}
