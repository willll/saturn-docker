#!/bin/bash

$SATURN_CD/mkisofs -v -sysid "SEGA SEGASATURN" -volid $1 -volset $1 -publisher $2 \
 -preparer $3 -appid "CD0001" \
 -abstract "ABS.TXT" -copyright "CPY.TXT" -biblio "BIB.TXT" \
 -generic-boot $4 \
 -full-iso9660-filenames -o $5 \
 -exclude-list EXCLUDES.txt \
 -path-list FILES.txt
