# RE8Display 2.7.0 release artwork

- `RE8Display-2.7.0-overview.jpg`: lightweight upload version, 2400 × 1800 pixels.
- `RE8Display-2.7.0-overview.png`: lossless master at the same dimensions.

The four original screenshots are arranged in a 2 × 2 grid using uniform scaling, without cropping, stretching, retouching or generative imagery. Captions use Segoe UI with white text and restrained gold accents, outside the screenshots.

Rebuild from the project root:

```powershell
pwsh -NoProfile -File tools/RE8Release.Thumbnail.ps1
```

Use `-ScreenshotDirectory` to specify a different folder containing the four original screenshot filenames listed in the script. Source screenshots remain unchanged. The script uses Windows System.Drawing and installed Windows fonts; no additional imaging packages are required.
