# Wrap Files
.wrap files are asset files. Their purpose is to take game assets and save them into larger, more compressed file formats.

## Format

Header
Files

### Header Format

| Name | Bytes | Description |
| ID | 4 | Identification to ensure this is indeed a wrap file. |
| Wrap Version | 4 | The wrap version. |
| Content Version | 4 | The content version. |
| Path | 100 | Base path for the folder. |
| Name | 50 | Name of this wrap file. |
