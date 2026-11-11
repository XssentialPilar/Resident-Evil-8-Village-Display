[CmdletBinding()]
param([string]$ProjectRoot,[string]$ArchivePath='dist\Last\RE8Display-2.7.2-Final-source.zip')
$ErrorActionPreference='Stop'
if(-not $ProjectRoot){$ProjectRoot=Split-Path -Parent $PSScriptRoot}
$ProjectRoot=[IO.Path]::GetFullPath($ProjectRoot)
. (Join-Path $PSScriptRoot 'RE8Display.Common.ps1')
$receipt=Get-Content -LiteralPath (Join-Path $ProjectRoot 'artifacts\build\RE8Display-build.json') -Raw | ConvertFrom-Json
if($receipt.sourceFingerprint -ne (Get-RE8DisplayFingerprint $ProjectRoot)){throw 'Build sources changed.'}
if($receipt.binarySha256 -ne (Get-RE8Hash $receipt.binary)){throw 'Build binary changed.'}
$zip=Get-RE8ContainedPath $ProjectRoot $ArchivePath
if(Test-Path -LiteralPath $zip){throw 'Preserve the existing source delivery.'}
$stage=Get-RE8ContainedPath $ProjectRoot ('artifacts\packages\2.7.2\final-source-'+[Guid]::NewGuid().ToString('N').Substring(0,8))
[IO.Directory]::CreateDirectory($stage)|Out-Null
$roots=@('native','config','tools','tests','licenses','docs','external\reframework-sdk','external\minhook')
$inputs=@(foreach($relative in $roots){
    $folder=Get-RE8ContainedPath $ProjectRoot $relative
    Get-ChildItem -LiteralPath $folder -Recurse -Force -File | Where-Object {
        $_.FullName -notmatch '[\\/](\.git|__pycache__|bin|obj)[\\/]' -and $_.Extension -notin '.pyc','.pyo','.pdb','.dll','.exe','.nupkg','.zip'
    }
})
$inputs+=@(foreach($relative in @('CMakeLists.txt','AGENTS.md','LICENSE','README.md','.gitignore','.gitattributes','external\standalone-dependencies.json')){
    $file=Get-RE8ContainedPath $ProjectRoot $relative
    if(Test-Path -LiteralPath $file -PathType Leaf){Get-Item -LiteralPath $file -Force}
})
foreach($file in $inputs){
    $relative=$file.FullName.Substring($ProjectRoot.Length+1)
    $safe=Get-RE8ContainedPath $ProjectRoot $relative
    $target=Get-RE8ContainedPath $stage $relative
    [IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($target))|Out-Null
    Copy-Item -LiteralPath $safe -Destination $target
}
Copy-Item -LiteralPath (Join-Path $ProjectRoot 'docs\RE8Display-2.7.2-media-r2.md') -Destination (Join-Path $stage 'START-HERE.md')
Copy-Item -LiteralPath (Join-Path $ProjectRoot 'artifacts\build\RE8Display-build.json') -Destination (Join-Path $stage 'BUILD-RECEIPT.json')
if($receipt.sourceFingerprint -ne (Get-RE8DisplayFingerprint $stage)){throw 'Source copy fingerprint mismatch.'}
$files=@(Get-ChildItem -LiteralPath $stage -Recurse -Force -File | Sort-Object FullName | ForEach-Object {
    [ordered]@{path=$_.FullName.Substring($stage.Length+1).Replace('\','/');sha256=Get-RE8Hash $_.FullName}
})
Write-RE8Json (Join-Path $stage 'SOURCE-MANIFEST.json') ([ordered]@{version='2.7.2';revision='media-r2';sourceFingerprint=$receipt.sourceFingerprint;binarySha256=$receipt.binarySha256;files=$files})
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem
$pending=$stage+'.pending.zip'
$creating=[IO.Compression.ZipFile]::Open($pending,[IO.Compression.ZipArchiveMode]::Create)
try{
    foreach($file in @($files)+@(@{path='SOURCE-MANIFEST.json'})){
        [IO.Compression.ZipFileExtensions]::CreateEntryFromFile($creating,(Join-Path $stage $file.path),$file.path,[IO.Compression.CompressionLevel]::Optimal)|Out-Null
    }
}finally{$creating.Dispose()}
$archive=[IO.Compression.ZipFile]::OpenRead($pending)
try{
    if($archive.Entries.Count -ne $files.Count+1){throw 'Source archive entry count mismatch.'}
    foreach($file in $files){
        $entry=$archive.GetEntry($file.path)
        if(-not $entry){throw "Missing source entry: $($file.path)"}
        $stream=$entry.Open();$sha=[Security.Cryptography.SHA256]::Create()
        try{$hash=([BitConverter]::ToString($sha.ComputeHash($stream))).Replace('-','')}
        finally{$stream.Dispose();$sha.Dispose()}
        if($hash -ne $file.sha256){throw "Source archive hash mismatch: $($file.path)"}
    }
}finally{$archive.Dispose()}
[IO.Directory]::CreateDirectory([IO.Path]::GetDirectoryName($zip))|Out-Null
Move-Item -LiteralPath $pending -Destination $zip
Write-RE8Json (Join-Path $ProjectRoot 'artifacts\build\RE8Display-media-r2-source.json') ([ordered]@{version='2.7.2';revision='media-r2';directory=$stage;archive=$zip;archiveSha256=Get-RE8Hash $zip;sourceFingerprint=$receipt.sourceFingerprint;verifiedFiles=$files.Count})
Write-Output "Verified source files: $($files.Count)"
Write-Output "Source archive: $zip"
