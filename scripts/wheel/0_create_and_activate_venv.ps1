$projectRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
Push-Location $projectRoot

$venvName = "vclib_build_venv"
$qt6Dir = "C:\Qt\6.10.3\msvc2022_64"

Write-Host "Creating virtual environment '$venvName' in progress..." -ForegroundColor Cyan
python -m venv $venvName

$activatePs1 = Join-Path $projectRoot "$venvName\Scripts\Activate.ps1"
$activateBat = Join-Path $projectRoot "$venvName\Scripts\activate.bat"

if (Test-Path $activatePs1) {
    Write-Host "Inserting the Qt6_DIR variable into the activation scripts..." -ForegroundColor Cyan
    
    # --- 1. Modify Activate.ps1 (for PowerShell) ---
    Add-Content -Path $activatePs1 -Value "`n# Automatic setting of Qt6_DIR for the build"
    Add-Content -Path $activatePs1 -Value "`$env:Qt6_DIR = `"$qt6Dir`""
    
    # --- 2. Modify activate.bat (for CMD) ---
    if (Test-Path $activateBat) {
        Add-Content -Path $activateBat -Value "`r`nREM Automatic setting of Qt6_DIR for the build"
        Add-Content -Path $activateBat -Value "set Qt6_DIR=$qt6Dir"
    }

    Write-Host "Installing required packages (build, wheel, setuptools)..." -ForegroundColor Cyan
    
    # Use the python executable from the newly created venv
    $pythonExe = Join-Path $projectRoot "$venvName\Scripts\python.exe"
    & $pythonExe -m pip install --upgrade pip
    & $pythonExe -m pip install build wheel setuptools
    
    Write-Host "`nDone! The virtual environment is ready and the packages are installed." -ForegroundColor Green
    Write-Host "NOTE: A PowerShell script cannot activate a venv for you permanently." -ForegroundColor Yellow
    Write-Host "To activate it in your current terminal, run this command from the project root:`n"
    Write-Host "  . .\$venvName\Scripts\Activate.ps1`n" -ForegroundColor Cyan
} else {
    Write-Error "Unable to find the activation script in $activatePs1"
}

Pop-Location
