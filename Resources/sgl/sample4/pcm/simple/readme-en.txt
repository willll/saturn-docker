◎About PCM samples

☆Directory structure
  +-+ALL: Samples compatible with all data formats and all playback methods.
    |
    +MEM: Sample using memory playback (AIFF only playback)
    |
    +GFS: Sample using file system playback (AIFF only playback)
    |
    +STM: Sample using stream system (plays only AIFF)
    |
    +XA: (ADPCM) playback sample by CDROM-XA (plays only XA-AUDIO)
    |
    +TESTDAT: AIFF data and
              ADPCM data (exhaled by Audio Stack)

  Basically, all samples other than ALL use conditional compilation of ALL.
  This is what was removed. In each sample, only AIFF is used, but by referring to ALL
  Playing other formats is also easy.
  Also, in the XA directory, there is also a script for CDBUILD to create XA tracks.
  It contains.
  You can use this script to create a CD image with XBLD.
  The TESTDAT directory contains sound data in COFF format.
  This COFF file has a start address of 200000H (the beginning of Low RAM),
  This can be used to check memory playback, etc.
  The format is AIFF, ADPCM (AIFF-like file produced by Audio Stack).
  (for playback in MODE1).
  For playback from CDROM, create a CDROM or use Virtual CD System or Mirage.
  I need to create a CD image, but the data for that is not available.

Furthermore, the README in the ALL directory contains more detailed information.
Please be sure to refer to them as well.
