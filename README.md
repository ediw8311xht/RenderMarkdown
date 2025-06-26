## Render Markdown
Renders markdown files to image or terminal.

#### Rendering:
ImageMagick (Magick++ API)

#### Terminal display:
Kitty Graphics Protocol.

## Usage:
```
RenderMarkdown [options] [input-file] [output-file]
Options:
  -h [ --help ]               print help
  -t [ --test ]               run tests
  -W [ --width ] arg (=800)   Canvas width
  -H [ --height ] arg (=1000) Canvas height
  -c [ --config ] arg         Config File (json)
  -d [ --display ]            display to terminal
  -R [ --overwrite ]          overwrite output file
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
