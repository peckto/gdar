#!/bin/sh

for file in `ls *.?pp` ; do
    mv "$file" "$file.bak"
    sed -r 's/libdar\:\:/libdar5\:\:/g' < "$file.bak" > "$file"
done
