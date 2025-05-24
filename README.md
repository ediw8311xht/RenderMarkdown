## Render Markdown

Renders markdown files to image using ImageMagick (Magick++ API) or to terminal using Kitty Graphics Protocol.

## Usage:
```
RenderMarkdown [options] [input-file] [output-file]
Options:
  -h [ --help ]               print help
  -t [ --test ]               run tests
  -W [ --width ] arg (=800)   image width
  -H [ --height ] arg (=1000) image height
  -d [ --display ]            display to terminal
  -O [ --overwrite ]          overwrite output file
  -i [ --input-file ] arg     input file
  -o [ --output-file ] arg    output file
Errors:
   1  Argument Error
   2      File Error
   3     Write Error
```


## Required Libraries:
- Magick++ : https://imagemagick.org/script/magick++.php
- Boost    : Boost

## Installing/Compiling
Ensure you have installed the required libraries.

### Linux
Run `make` then execute binary `RenderMarkdown`

### Windows
If running on Windows, then you may need to add boost into makefile.
