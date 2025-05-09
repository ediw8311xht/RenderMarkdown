## Render Markdown

Renders markdown files to image using ImageMagick (Magick++ API) or to terminal using Kitty Graphics Protocol.

## Usage:
```
RenderMarkdown: [options] [input-file] [output-file] 
     Options  
         --help    : print help  
         --test    : run tests   
         --display : display image on terminal
     Arguments  
         [input-file]  : Markdown formatted input file. 
         [output-file] : (Image file to output) or ('-') to output to terminal (must have kitty). 
     Exit Status  
         0    Good  
         1    Argument Error 
         2    File Error 
         3    Write Error
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
