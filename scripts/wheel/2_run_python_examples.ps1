$projectRoot = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)

Push-Location $projectRoot
try {
    $venvName = "vclib_build_venv"
    $pythonExe = Join-Path $projectRoot "$venvName\Scripts\python.exe"

    if (Test-Path $pythonExe) {
        Write-Host "Running Python examples using '$venvName'..." -ForegroundColor Cyan
        $failed = @()
        $examples = Get-ChildItem -Path "examples\bindings\core\*.py" | Sort-Object Name

        foreach ($f in $examples) {
            $relPath = Resolve-Path -Relative $f.FullName
            Write-Host "=== Running $relPath ==="
            & $pythonExe $f.FullName
            if ($LASTEXITCODE -ne 0) {
                $failed += $relPath
            }
            Write-Host ""
        }

        if ($failed.Count -gt 0) {
            Write-Error "Failed examples:`n  $($failed -join "`n  ")"
            exit 1
        }
    } else {
        Write-Error "Virtual environment not found. Run scripts\wheel\0_create_and_activate_venv.ps1 first!"
        exit 1
    }
} finally {
    Pop-Location
}
