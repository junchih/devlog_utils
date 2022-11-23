#!env -S deno run --allow-all

import ffmpeg from "npm:fluent-ffmpeg@^2.*";

var inFile: string = Deno.args[0];
var outFile: string = Deno.args[1];
console.log(`from: ${inFile}, to: ${outFile}`);

ffmpeg(inFile)
  .audioCodec("aac")
  .audioBitrate("128k")
  .videoCodec("libx264")
  .videoBitrate("2500k")
  .videoFilters("scale=-2:720")
  .save(outFile);

/*
#!/bin/sh

CRF=${3:-23}

ffmpeg \
        -hide_banner \
        -i "$1" \
        \
        -vf scale=-2:720 \
        -codec:v h264 \
        -b:v 2500k \
        -minrate 1500k \
        -maxrate 4000k \
        -bufsize 5000k \
        \
        -codec:a aac \
        -b:a 128k \
        \
        -crf "$CRF" \
        -preset veryslow \
        \
        "$2"
*/
