# ComputerGraphics
Example code for the Computer Graphics subject

You will need to set the <b>Working Directory</b> to the <b>./bin/</b> folder in Visual Studio, i.e. <b><i>"$(ProjectDir)bin\"</i></b> for some of the examples to work correctly.

The aim is for every tutorial within this subject to contain a working example here, along with any other useful code samples that may help teachers best deliver this subject.

Each is implemented as an application class derived from <b>BaseApplication</b>. To run a certain example simply change the application being created within <b>main.cpp</b>.

Current working examples are:

- <b>SolarSystemApplication</b> (uses Gizmos to demonstrate matrix parenting)
- <b>RenderingGeomApplication</b> (rendered quad using GL VAO, VBO, IBO and hard-coded shader program)
  - <b>RenderingOBJApplication</b> (example using the Mesh class and Shader class)
- <b>TexturingApplication</b> (texture applied across a quad using GL VAO, VBO and IBO)
  - <b>TexturedOBJApplication</b> (example using the Mesh class and Shader class to demonstrate textured models)
  - <b>ApplyTextureOBJApplication</b> (example attaching a texture to an already loaded OBJ)
- <b>ProceduralGenerationApplication</b> (demonstrates using Perlin Noise to populate a texture and displacement mapping in a vertex shader)
- <b>GUIApplication</b> (demonstrates using IMGUI for UI elements)
- <b>LightingOBJApplication</b> (demonstrates Phong lighting on an imported OBJ untextured)
  - <b>LightingSphereApplication</b> (demonstrates Phong lighting on a generated sphere)
