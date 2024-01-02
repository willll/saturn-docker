◎About CDDA playback

☆How to play CDDA
There are several ways to familiarize CDDA with an SGL-based development environment.
1) Use SGL's CD function.
    In this case, you need to assign a file name to the CDDA track.
    I need to access CDDA tracks by file name.
    File names are assigned to CDDA tracks when building a CD image.
    Specify in a script file.

2) How to use CDC
    If you use the CD communication library, you can use the track number or frame address to
    You can play CDDA.
    With this method, it is possible to play CDDA tracks without file names like those on ordinary CDs.
    can.

3) How to use GFS and CDC
    This method is the same as 1), and accesses CDDA like a file.

This sample uses methods 2) and 3). Method (1) is
This is done with sample/sample_cd4. Please take a look there. )

Method 3) obtains the frame address from the file name, and then
The address processing method is the same as in 2), so first, from the file name,
We will explain how to obtain the frame address, and then explain method 2).
I will explain in detail.

☆Obtain frame address
In the sample program, the frame address is obtained from the file name.
GetFrameAddress function.
This function gets the file ID from the file name and from that file ID
It goes through the process of getting the frame address.
Use GFS_NameToId( filename ) to get the file ID from the file name.
The function that calculates the frame address from the file ID obtained with this function is:
GFS_GetDirInfo( Sint32 file_id, GfsDirId *directory_id_table )
is. This "directory_id_table" contains the physical information on the CDROM for that file.
Contains information, including the frame address.
An access macro exists to obtain the frame address from this structure.
GFS_DIR_FAD( GfsDirId *directory_id_table )
That's it.
Now you can happily get the frame address of the CDDA track from the file name.
it is complete.

☆CDDA track/frame address playback
CDDA playback using CDC uses the "CdcPos" structure for both track and frame addresses.
This is done by initializing the necessary member variables and calling the "CDC_CdPlay" function.
For track number playback and frame address playback, the “CdcPos” structure that should be filled
Only the member variables are different.
less than. I will give you a list of the members you need to fill in for each.
Basically, instead of accessing member variables directly, use access macros.
We will change it, so here we will give you the access macro.
Track number playback
  CDC_PLY_STYPE: Type at playback start (specify CDC_PTYPE_TNO)
  CDC_PLY_STNO: Start track number
  CDC_PLY_SIDX: Start index number
Frame address playback
  CDC_PLY_STYPE: Playback type at playback start (specify CDC_PTYPE_FAD)
  CDC_PLY_SFAD: Start frame address

Other common elements
  CDC_PLY_ETYPE: Type at end of playback (specify CDC_PTYPE_TNO)
  CDC_PLY_PMODE: Play mode (repeat, move pick, etc.)
etc., and it is now possible to specify different types at the start and end of playback.
To specify when to end,
  CDC_PLY_ETNO: Stop track number
  CDC_PLY_EIDX: Stop index number
  CDC_PLY_EFAD: Stop frame address
To use each specification, use the "CDC_PLY_ETYPE" macro mentioned above.
is needed.

☆About CD block status
If you want to control the CD in detail, you can check the state of the CD block, such as whether the pick is moving.
Sometimes you just need to know. The CDC function supports such detailed requests.
There are functions available to do this. (Conversely, in SGL functions, such detailed operations are
Can not do. )
The CDC_GetPeriStat function returns the status of a CD block.
This function sends the status response periodically returned by CD Block to the user.
By using this function, the command request to the CD block will be
Does not occur.
There are very strict restrictions on command requests to the CD block, so please avoid using the CD block as much as possible.
You must refrain from requesting commands to.
In that sense, the CDC_GetCurStat function, which has a similar function, is a command to the CD block.
You should use the CDC_GetPeriStat function as much as possible since it requires a request.
*These CD block status acquisition functions can detect CD tray opening, but
This tray open status is not complete.
To check the tray open, be sure to
Please use the CDC_GetHirqReq function.
