# Graphics Assessment

[Website](https://bennybroseph.github.io/OpenGL_Graphics_Assessment/)

This repository is for the Graphics assessment for Year 2. It contains multiple compiled programs for each required part of the assessment. Check the releases tab for an easy way to find a compiled and zipped version of each program.

Each is implemented as an application class derived from <b>BaseApplication</b>. To run a certain example simply change the application being created within <b>main.cpp</b>.

Current working examples are:

- [<b>Intro To OpenGL</b>](https://github.com/bennybroseph/Year-2/releases/tag/1.0) [ReadMe](https://docs.google.com/document/d/1IJtbUiv4CHTNCCFirTcbIrJLG3M_SDe69oCyLwc9VXA/edit?usp=sharing) (Demonstrates setting up and OpenGL context window using GLFW as the wrangler)
- [<b>Rendering Geometry</b>](https://github.com/bennybroseph/Year-2/releases/tag/2.0) [ReadMe](https://docs.google.com/document/d/1CiJHh5O7DcQRhr8fZIBz0Rn27rjLUC5KNMHLDtJHbd0/edit?usp=sharing) (Demonstrates drawing a plane using a custom vertex and fragment shader)
- [<b>Cameras and Projections </b>](https://github.com/bennybroseph/Year-2/releases/tag/3.0) [ReadMe](https://docs.google.com/document/d/1a_Q5JioL1uQwTnFrGCcOtHEKER8AlBrvRzfOq5VKiBY/edit?usp=sharing) (Demonstrates creating a base camera class, and then deriving a Fly Camera from that)
- [<b>Direct Lighting</b>](https://github.com/bennybroseph/Year-2/releases/tag/4.0) [ReadMe](https://docs.google.com/document/d/1psDq-5ZGS4HmRJM-wTsZh7sAoNPN571RmVKBRjApkb8/edit?usp=sharing) (Demonstrates Phong lighting on a generated sphere)
- [<b>Texturing</b>](https://github.com/bennybroseph/Year-2/releases/tag/5.0) [ReadMe](https://docs.google.com/document/d/1hLMMU4T2_JenmmOphJ3VL_8IbrAM5NkgmmPMXekbGME/edit?usp=sharing) (Texture applied across a quad using GL VAO, VBO and IBO)
- [<b>Procedural Generation</b>](https://github.com/bennybroseph/Year-2/releases/tag/6.0) [ReadMe](https://docs.google.com/document/d/1psDq-5ZGS4HmRJM-wTsZh7sAoNPN571RmVKBRjApkb8/edit?usp=sharing) (Demonstrates using Perlin Noise to populate a texture and displacement mapping in a vertex shader)

## [<b>Putting it all Together</b>](https://github.com/bennybroseph/Year-2/releases/tag/Final)
This is the final product. After putting all of the pieces together using a component based archetecture, we get this as the result. It contains a basic scene editor modeled after Unity complete with a transform hierachy and inspector. The first .exe provides a basic solar system of a sun, earth and moon to show transform parenting. The second one provides an example of texturing.

### Different Material Ambient Color
This Scene shows three spheres of decreasing sizes which are parented from largest to smallest. This scene is representing a sort of Sun, Earth,  and Moon. When the Sun rotates, the Earth rotates. When the Earth rotates, the Moon rotates. This is represented in the Hierarchy window on the left. The Sun is the root, the Earth is a child of the sun, the Earth is a parent of the Moon, and the Moon is a child of the Earth. On the right, the inspector window show the current values for the selected object(highlighted in a light purple color). The green, blue, and red lines are the Sun's up, forward, and right directions based on it's transform's orientation respectivly.
![Alt text](Images/SolarSystemSun.gif?raw=true)
### Different Color Directional Light Diffuse Color + Material Ambient Color
This time the directional light is selected. In the inspector, the diffuse color was changed to a bright purple color. This is represented by the directional light's gizmo at the top center of the scene. The line starts out bright purple and fades into the specular color which is green.
![Alt text](Images/SolarSystemDirectionalLight.gif?raw=true)
### Texture Shader Options
This Scene shows off the multiple different options for shaders which can be chagned at runtime. Each shader has it's own paramerters which are defined in the shader itself using a comment and read in by my custom parser for it. The options for a texture shader are shown in the screenshot. Clicking on the '...' button will open up a standard windows file open dialogue and the new image file can be selected from there. The object will then update it's texture immediatly upon selection.
![Alt text](Images/ShaderTexture.gif?raw=true)
