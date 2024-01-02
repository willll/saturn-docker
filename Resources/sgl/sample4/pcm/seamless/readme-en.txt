About PCM seamless playback samples

All samples under this directory are seamless playback of multiple PCM files.
This is a sample based on , but there are also samples with some different elements.
The sample in the directory all is a sample that includes all elements.
Other directories only process specific elements from the samples in the all directory.
This is what I cut out. Therefore, programs with elements that are not in these samples
If necessary, please refer to the sample programs in this all directory.
For more information about the sample program, see the README file in the all directory.
Please take a look.

Structure under this directory

+--+-all Samples including seamless, pause, and forced transition to next song
   | Switch with define.
   +-basic A sample that only performs seamless playback. AIFF playback only.
   +-ff Sample that forces the next song to play seamlessly while the previous song is playing.
   +-pause (Although not directly related to seamless playback) Pause while playing a song.
             This is a sample of starting and restarting.

◎Seamless PCM playback
  A stream system is required for seamless PCM playback.
  The reason is that the PCM/ADPCM playback library is only available when using the stream system.
  Attempts to retrieve data from a CD by reading ahead.
  In general, the end determination of a PCM/ADPCM playback library is based on the actual end position.
  It will be done early. This is done by completing the termination process a little early.
  It has the meaning of preparing for the next rebirth.
  At this time, reading ahead from the CD comes into play. Library next play entry
  If there is, the sound block will not be requested to terminate the PCM, so
  The previous song and the next song are seamlessly connected.
  Playback end trigger specified by the user to determine the likely end position
  The number of samples
  The process ends when less than 100 samples remain.
  The number of playback end trigger samples is
  PCM_SetStopTrigSample
  It can be set using the function, but the default is 4096 samples.
  If you set a larger number of samples with this function, it will take more time to play the next PCM.
  Therefore, even if the data supply is delayed due to a CD read error, etc.
  This will eliminate the problem of a pause before the beginning of the next song is played.
  However, the previous song ends early, so the end of the song ends up being cut off.
  You need to be careful when changing this.

◎Special usage of seamless playback
  The PCM/ADPCM playback library performs termination processing early for seamless playback.
  Therefore, depending on how the song data is created, the end of the song may be cut off.
  To avoid this, you may want to use seamless playback.
  Always seamlessly play data with silence of 4096 samples or more at the end of a song
  This allows you to avoid such phenomena. The solution to this problem is to use the sound data
  This is an effective method regardless of the format or playback format.
  This is because the PCM/ADPCM playback library itself is designed with emphasis on seamless playback.
  This may be due to the fact that
