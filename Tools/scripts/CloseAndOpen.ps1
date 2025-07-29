function Get-DTEInstance {
    $rot = [System.Runtime.InteropServices.Marshal]::GetActiveObject("VisualStudio.DTE")
    return [System.Runtime.InteropServices.Marshal]::GetActiveObject("VisualStudio.DTE.17.0")
}

# Get the running Visual Studio instance (change 17.0 to your VS version)
$dte = Get-DTEInstance

if ($dte -ne $null) {
    # Bring Visual Studio to the foreground
    $dte.MainWindow.Activate()

    # Close all open documents
    $dte.Documents.CloseAll()

    # Open CANDIDATE.h (Modify the path accordingly)
    #$filePath = "%ALGEWINDOWS%\Platforms\CANDIDATE.h"
    $CurrentDirectory = Get-Location
    Write-Output $CurrentDirectory
    if ($args.Count -gt 0) {
        $tgt = $args[0]
        Write-Output "Selected App name passed as First Parameter=$($args[0])"
        #$filePath = "C:\Users\aliveb\REPOS\XP08-AlgeSDK\Platforms\CANDIDATE.h"
        $filePath = "$($CurrentDirectory)\..\Apps\$($tgt)\$($tgt).hpp"
        Write-Output "filepath=$($filepath)"
        $dte.ItemOperations.OpenFile($filePath)
    } else {
        Write-Output "Error: No argument provided!"
    }
} else {
    Write-Host "No running Visual Studio instance found."
}
