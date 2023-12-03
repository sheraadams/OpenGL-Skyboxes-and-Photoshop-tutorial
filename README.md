## About the Project

This is a repository resource loaded with high-resolution skyboxes for use with OpenGL (C++). Most of the skyboxes here are open source but are cited below for reference. Tutorials are linked below to demonstrate Photoshop's best practices for preserving detail and resolution when working with the images. 

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

Using OpenGL and Photoshop to make 20000 x 15000, 300 dpi resolution skyboxes in Photoshop in less [than two minutes](https://www.youtube.com/watch?v=7LiPo5eH600).

Other videos: 

Also check out how to [configure your opengl project](https://youtu.be/ZFx30Zmo1yI) and how to [create game assets](https://youtu.be/WE-DJ-A5yTY).

## Tutorial Previews

![Screenshot 2023-01-26 112904](https://user-images.githubusercontent.com/110789514/214892358-50fb78fa-2a3d-4673-b759-62a78f34f659.png)

## Function

The skyboxes use the following C++ function to create the skybox with OpenGL. Note that left and right are horizontally reflected.
Also, front and back are horizontally reflected.

```cpp
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
```

## Image and Media References
Check out my [references](https://github.com/sheraadams/OpenGL-Skyboxes-and-Photoshop-tutorial/edit/main/REFERENCES.md) here.

<div style="text-align: center;">
  <p><strong>Proudly crafted with ❤️ by <a href="https://github.com/sheraadams" target="_blank">Shera Adams</a>.</strong></p>
</div>

