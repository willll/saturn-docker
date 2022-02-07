#!/bin/bash
set -e

echo "Generating ISO ------------------>  $1"

echo "$SATURN_CD/ABS.TXT" >> "$6/CD/FILES.txt"
echo "$SATURN_CD/BIB.TXT" >> "$6/CD/FILES.txt"
echo "$SATURN_CD/CPY.TXT" >> "$6/CD/FILES.txt"
echo "$6/CD/$4" >> "$6/CD/FILES.txt"

if [ ! -f "$6/CD/EXCLUDES.txt" ]; then
  touch "$6/CD/EXCLUDES.txt"
fi

$SATURN_CD/mkisofs -v -sysid "SEGA SEGASATURN" -volid "$1" -volset "$1"  -publisher "$2" \
 -preparer "$3" -appid "$1" -sectype 2352 \
 -abstract "ABS.TXT" -copyright "CPY.TXT" -biblio "BIB.TXT" \
 -generic-boot "$6CD/$4" \
 -full-iso9660-filenames -o "$6/$5" \
 -exclude-list "$6/CD/EXCLUDES.txt" \
 -path-list "$6/CD/FILES.txt"

exit
