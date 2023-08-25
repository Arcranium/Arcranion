param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("Build", "Run", "Clean")] 
    $Option
)

function Build {
    # Create build/ if it doesn't exists
    mkdir  -Force ./build > $null

    # Set directories
    $source = $PSScriptRoot
    $build = "$PSScriptRoot/build"

    # Run cmake
    cmake -G "Visual Studio 17 2022" -B"$build" -S"$source"

    # ninja -C $build
    & 'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe' build\Arcranion.sln
}

function Run {
    ./build/test/Debug/ArcranionTest.exe
}

function Clean {
    # Set directories
    $build = "$PSScriptRoot/build"

    # Remove and recreate the directory
    Remove-Item -LiteralPath $build -Force -Recurse > $null
    New-Item -ItemType Directory -Force -Path $build > $null
}

Write-Host "--------------------------" -ForegroundColor Green
Write-Host "Running Task [$Option]" -ForegroundColor Green
Write-Host "--------------------------" -ForegroundColor Green

switch ($Option) {
    Build { Build }
    Run { Run }
    Clean { Clean }
}

Write-Host "--------------------------" -ForegroundColor Green
Write-Host "Finished Task [$Option]" -ForegroundColor Green
Write-Host "--------------------------" -ForegroundColor Green