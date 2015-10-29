Simple plugin to export Static Meshes in Unreal to itpmesh2 format

To use this plugin, follow these steps:
1. In the C++ Unreal 4 project directory, create a "Plugins" directory
2. Place the "ITPMeshExporter" directory inside "Plugins"
3. Right click on your .uproject and regenerate the project files
4. Now in the editor, you can right click on a static mesh and select
   "Asset Actions>Export...". From the file dropdown, select
   .itpmesh2, and you should be set.

A Note on Mesh Orientation:
The plug-in does not fix-up or transform the asset in any way. You
should make sure your asset is facing down the X-axis with Z-up
before you export it.

A Note on Textures:
Right now, the exporter will select the FIRST texture associated with
the "BaseColor" of the FIRST material assigned to the mesh. Multiple
materials/textures will be ignored.

Also, it assumes that you will follow the convention where the texture
file is in Textures/NameOfTexture. So when it exports, you'll get both
an .itpmesh2 and a .tga file in the same directory, but you need
to place the .itpmesh2 in Assets/Meshes and the .tga in Assets/Textures.