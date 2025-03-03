# OpenGLEngine
![Rendu  Engine Open GL Opti 1](https://github.com/user-attachments/assets/f1829ef2-7fd6-4b4e-ae62-5f0bcfb82ea7)

## How to Use
### Librairies
- ASSIMP
- GLEW
- GLFW 
- GLM
🙌 You don't need to download external librairies

⚠️ Only work on Window OS
- Clone the repository
- Launch the Visual Studio Solution
- Build and Play in Debug

## Features
- Display 3D Object (Model or simple primitive).
- Material for displaying texture or interacting with the Lighting
- Lighting : Directionnal Light, Point Light, Spot Light who create Shadow Map
- Skybox
- Simple Camera Movement

## Controls
| **Category**       | **Key**        | **Action**         |
|---------------------|---------------|--------------------|
| **Movement**        | Z & UpArrow   | Move Forward       |
|                     | S & DownArrow | Move Backward      |
|                     | Q & LeftArrow | Move Left          |
|                     | D & RightArrow| Move Right         |
|                     | A             | Move Up            |
|                     | E             | Move Down          |
|                     | Shift Left    | Run                |
|                     | Ctrl Left     | Accelerate         |
| **Camera**          | Mouse Movement| Deccelerate        |
| **Light Torch**     | L             | Toggle Light on camera On/Off |
|---------------------| **Modifying Rendering** | -----------------------------------------|
| **Switch Skybox**   | 1             | Change Skybox to Stylized Cloud |
|                     | 2             | Change Skybox to Stylized Cloud Night |
|                     | 3             | Change Skybox to Cupertin Skybox |
|                     | 0             | Change Skybox to Minecraft Skybox |

### Remapping the Input
- Go to the KeyMappings.h and add or change the GLFW Key

## Modify the Engine
### Add Meshes
⚠️ OpenGLEngine only support .obj file. For the FBX File, you need to install the FBX SDK from Autodesk and implement the logic yourself.
- Add the Model in the Models Folder
- Open the ResourcePaths.h and add the path to your(s) model(s) in the namespace Models
- Open the main.cpp, Add a new variable of the name you want who is a model
- Go to the InitializeModels() and add the Model. 

✔️ For now the model and his resource are loaded but now you need to display him ! 
- Go to the main and search for the RenderScene()
- Call the Render3DModelObject and set has parameters the name of your model and the properties you want (Position, RotationAngle, Axis, Scale)
✅ If you follow all this step, now you have the Model display in the Engine.

### Add Skybox Textures
- Add the Skybox Texture in the Textures/Skybox Folder (Create a new folder for your Skybox)
- Open the ResourcePaths.h and add the path to your(s) Skybox Texture(s) in the namespace Skybox (create a new one for simplicity)
#### Order of Skybox Textures
1. Right or pz
2. Left or nz
3. Top or py
4. Down or ny
5. Back or nx
6. Front or px

- Now, go to Skybox.h and add a new name in the Skybox enum class then go to GetSkyboxPaths and add a new case with your skybox 


✔️ For now you be ready to use the Skybox you add !
#### Two Options :
1. Go to the main.cpp, search for the InitializeSkybox() and change the namespace to your skybox
2. Go to the main.cpp, search for the SwitchSkybox(), add a new declaration and a new input for changing to your skybox
✅ If you follow all this step, now you have the Model display in the Engine.


## More Pictures
![Rendu Open GL 1](https://github.com/user-attachments/assets/e4b7210e-df01-4b39-9d02-e7ddc823bc6d)
![Rendu Engine OpenGL 2](https://github.com/user-attachments/assets/d714c463-1de5-4174-8b19-8ecadf9f8690)
![Rendu Engine OpenGL 3](https://github.com/user-attachments/assets/79046f46-e7fc-4393-98b5-75f7ddd3c452)


