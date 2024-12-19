# GraphicsSandBox

## Build
This project uses Visual Studio 2022 and requiers the following WorkLoads:
| |  |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/CPP_VS.png) | ![](GraphicsSandBox/Resources/ForREADME/DirectX_VS.png) |

After this is done only clone the repository and build it.

-------------------------------

## Controls

* A/D/S/W - Move LEFT/RIGHT/BACKWARD/FORWARD
* R/F - Move UP/DOWN
* ESC - Enable/Disable free movement

-------------------------------

## Framework Overview

![](GraphicsSandBox/Resources/ForREADME/diag2.png)

- Drawable: A base class that contains a vector of shared pointers to Bindable objects, which represent GPU resources(Vertex Buffers, Index Buffers, Constant Buffers, Shaders, etc).
- Mesh: Inherits from Drawable and represents a renderable object. It utilizes the Drawable's bindables to define its rendering behavior.
- Codex: A resource management class that maintains a map of all created Bindable objects. When a Mesh requests a Bindable, the Codex checks if it already exists, allowing for resource reuse and avoiding redundant creation.



## Features

* Geometric primitives
    - Cube
    - Pyramid
    - Tessellated Plane
    - Prism
    - Cylinder
    - UV Sphere
    - Torus
* Assimp Model Loading
* Blinn-Phong Lighting
* Normal Mapping
* Scene Graph
* Alpha Blending


## Screenshots

## 3D Primitives

### Cube
| Solid |  Wireframe |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/SolidCube.png) | ![](GraphicsSandBox/Resources/ForREADME/WireCube.png) |


### Plane
| Solid |  Wireframe |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/SolidPlane.png) | ![](GraphicsSandBox/Resources/ForREADME/WirePlane.png) |


### Prism
| Solid |  Wireframe |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/SolidPrism.png) | ![](GraphicsSandBox/Resources/ForREADME/WirePrism.png) |


### Cylinder
| Solid |  Wireframe |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/SolidCylinder.png) | ![](GraphicsSandBox/Resources/ForREADME/WireCylinder.png) |


### UV Sphere
| Solid |  Wireframe |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/SolidUVSphere.png) | ![](GraphicsSandBox/Resources/ForREADME/WireUVSphere.png) |


### Torus
| Solid |  Wireframe |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/SolidTorus.png) | ![](GraphicsSandBox/Resources/ForREADME/WireTorus.png) |


<br/>

## Assimp Model Loading

| Solid |  Wireframe |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/suzanneSolid.png) | ![](GraphicsSandBox/Resources/ForREADME/suzanneWire.png) |


<br/>


## Blinn-Phong Lighting
| |  |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/LeonFar.png) | ![](GraphicsSandBox/Resources/ForREADME/LeonClose.png) |


<br/>


## Normal Mapping
| No NMap | With NMap |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/NO_NMAP.png) | ![](GraphicsSandBox/Resources/ForREADME/WITH_NMAP.png) |

| No NMap/Zero Specular Power| With NMap/Zero Specular Power |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/NO_NMAP_NO_SPECPOW.png) | ![](GraphicsSandBox/Resources/ForREADME/WITH_NMAP_NO_SPECPOW.png) |


## Scene Graph

|Classic Sponza Scene|
|--------------------|
|![](GraphicsSandBox/Resources/ForREADME/SCENE_GRAPH.png)|

## Anisotropic filtering

| AF OFF | AF ON |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/AF_OFF.png) | ![](GraphicsSandBox/Resources/ForREADME/AF_ON.png) |


## Alpha Testing

| WITHOUT ALPHA TESTING | WITH ALPHA TESTING |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/NO_AT.png) | ![](GraphicsSandBox/Resources/ForREADME/WITH_AT.png) |

## Alpha Blending

|  |  |
|---|---|
|  ![](GraphicsSandBox/Resources/ForREADME/AB2.png) | ![](GraphicsSandBox/Resources/ForREADME/AB.png) |

-------------------------------