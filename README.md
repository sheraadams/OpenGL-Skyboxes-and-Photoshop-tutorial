## Update

March 5, 2023: Added new skyboxes and an hdr environment map for use in OpenGL. 

## Skybox Previews

<p align="left">
   <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/222976468-0fd8be7a-dd3b-4b68-b954-1cfbc1ee831b.jpg">
  <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/222979976-0e9b8081-8140-4181-98ab-be2b08e0ff1c.jpg">
  <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/222980028-f52fa496-55f4-4886-90e3-24d874ef9300.png">
    <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/222980038-1075378d-cfc5-4bf1-93e7-a70c83567097.jpg">
    <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/222980136-d84374da-6472-4f80-a141-40c44daa2441.jpg">
    <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/222980406-c7539bd3-875b-4eb6-80fe-58cbdf1ac17a.png">
    <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/225405846-fda5fff0-f221-4aa1-a683-de20d40eaa4c.jpg">
    <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/225406623-71113728-c3e5-4a38-8866-9a7c08e8b602.jpg"> 
    <img width="300" height="300" src="https://user-images.githubusercontent.com/110789514/225407354-6fb0423c-bf3f-409b-972c-ad60aa50f761.jpg">    
</p>

## Photoshop Tutorial Links

Using OpenGL and Photoshop to make skyboxes and textures for 3d objects.

How to make amazing skyboxes 20000 x 15000, 300dpi resolution in Photoshop in under 2 minutes:https://www.youtube.com/watch?v=7LiPo5eH600 
Other videos: https://youtu.be/ZFx30Zmo1yI, https://youtu.be/WE-DJ-A5yTY

## Tutorial Previews

![Screenshot 2023-01-26 112904](https://user-images.githubusercontent.com/110789514/214892358-50fb78fa-2a3d-4673-b759-62a78f34f659.png)

## Function
    vector<std::string> faces
    {
        "resources/textures/space/right.jpg", // right 
        "resources/textures/space/left.jpg", // left 
        "resources/textures/space/top.jpg", // top  
        "resources/textures/space/bottom.jpg", // bottom  
        "resources/textures/space/front.jpg", // front
        "resources/textures/space/back.jpg", // back
    };
    unsigned int cubemapTexture = texture.loadCubemap(faces) 
  
left and right are horizontally reflected
front and back are horizontally reflected

## Image and Media References


DeVries, J. (n.d.). LearnOpenGL/LICENSE.md at master · JoeyDeVries/LearnOpenGL. GitHub. https://github.com/JoeyDeVries/LearnOpenGL/blob/master/LICENSE.md

DM7(n.d.). master spaceship in white background top view. https://as1.ftcdn.net/v2/jpg/04/16/15/36/1000_F_416153690_zcwO683IOB5otBAdR901AGXMqMqIvFkk.jpg

Persson, Emil (2010).Heart in the sand. http://www.humus.name/index.php?page=Textures&ID=82

Persson, Emil (2010). Måskonåive. http://www.humus.name/index.php?page=Cubemap&item=Maskonaive

Persson, Emil (2007). Meadow. http://www.humus.name/index.php?page=Textures&ID=39

Persson, Emil (2012). Nissi beach 2. http://www.humus.name/index.php?page=Textures&ID=100

Samserius. (n.d.). Abstract circular geometric background. https://as1.ftcdn.net/v2/jpg/04/07/63/62/1000_F_407636260_DK9Cn5SCJcuMO1981KhxfNylW4U0NCMQ.jpg

Solar System Scope. (n.d.). 8k Stars. [Image]. Retrieved March 4, 2023, from https://www.solarsystemscope.com/textures/

Unknown (n.d) Clouds. [Image] https://hdrmaps.com/above-the-clouds/

Unknown. (n.d.). Iceberg. [Image] https://learnopengl.com/Advanced-OpenGL/Cubemaps

unknown (n.d.). Loft. [Image] https://learnopengl.com/PBR/IBL/Specular-IBL

Westbeam (2013). Space-Skybox. https://opengameart.org/content/space-skybox-0

