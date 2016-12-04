
Description:

LePlatz is a level editor for the Platz toolset (supporting Platz v1.0). LePlatz eliminates the tedious task of converting images of your game world into textual data structures. LePlatz provides a gui with a set of tools corresponding to the logical structure of the format expected by the Platz toolset. You simply need to draw shapes on your game world canvas and LePlatz does the rest.

Created with Qt 4.6.

/***** Change Log *****/

LePlatz 1.1.0
- Added update tool to easily distribute future versions.
- Added LePatch tool to patch downloaded updates.
- All valid slice sizes now supported.
- Slice canvas background color configurable through "Edit->Canvas Bg Color" menu.
- Compatible with v1.0 and v1.1 project files. v1.0 files will be converted to v1.1 files upon saving.
- Added find/replace dialog. This tool allows you to update previously set
  triggers/platform clear tiles/mutable classes/mutable bgs (src code defines) that
  may have changed their name in your code. v1.0 stored these as indexes into the source
  code defines, but this caused problems when defines were removed or reordered. The new
  LePlatz project file format stores the defines as strings and so it not affected by updates
  in the source code. However, name changes can be easily filtered throughout your LePlatz
  world through the use of this tool in the "Edit->Find/Replace..." menu.
- Fixed an instance where you could not drop a tile onto an appropriate Inner bg to replace its
  tile index.
- Added support for Mutable Bg Classes (see Platz v1.1 for details).
- Added support for video mode 2.
- Added mode 2 example project.
- Numerous UI tweaks
- Removed need for SliceSize setting due to changes in Platz v1.1 collision handling.
- JPEG no longer an image format option.
- TileStudio sample now contains correct relative path to tiles.png.

LePlatz 1.0.0
- Initial release.
