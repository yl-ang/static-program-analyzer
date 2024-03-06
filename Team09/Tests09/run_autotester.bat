@echo off
setlocal enabledelayedexpansion

:: Get the current script's directory
set "CURRENT_DIR=%~dp0"

:: Set the path to the autotester binary
set "AUTOTESTER_BINARY=%CURRENT_DIR%\..\Code09\out\build\x64-Debug\src\autotester\autotester.exe"

set "TESTS_FOLDER=."

for /D %%M in (%TESTS_FOLDER%\milestone*) do (
    set "MILESTONE_FOLDER=%%M"

    for /f "tokens=2 delims=milestone" %%N in ("!MILESTONE_FOLDER!") do (
        set "MILESTONE_NUMBER=%%N"
    )

    :: Iterate through source and queries files
    set "i=1"
    set "SOURCE_FILE=!MILESTONE_FOLDER!\milestone!MILESTONE_NUMBER!_source_!i!.txt"

    :: Check if the source file exists
    if exist "!SOURCE_FILE!" (
        :process_files
        set "QUERIES_FILE=!MILESTONE_FOLDER!\milestone!MILESTONE_NUMBER!_queries_!i!.txt"
        set "OUTPUT_FILE=!MILESTONE_FOLDER!\milestone!MILESTONE_NUMBER!_out_!i!.xml"

        "%AUTOTESTER_BINARY%" "!SOURCE_FILE!" "!QUERIES_FILE!" "!OUTPUT_FILE!"

        echo Autotester completed for Milestone !MILESTONE_NUMBER! - Source !i! and Queries !i!
        set /a i+=1

        set "SOURCE_FILE=!MILESTONE_FOLDER!\milestone!MILESTONE_NUMBER!_source_!i!.txt"
        if exist "!SOURCE_FILE!" goto :process_files
    )
)
endlocal
