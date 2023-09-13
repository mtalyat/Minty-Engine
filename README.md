# Minty Engine
A dynamic and advanced voxel engine.

NOTE: This project is in development. More info on how to use, etc. will come in due time.

## About

### Motivation
Voxels are becoming more and more relevant in video games today. With newer and better CPUs and GPUs, many, many voxels can be handled on a screen, using the correct rendering techniques. Better looking games can be created, with realistic lighting, physics, and more. Another motivation is to make the engine, as well as the games created with it, very portable and efficient.

The goals of this project are as follows:

#### Performance
Performace, in terms of the CPU and the GPU. On the CPU, libraries such as [EnTT](https://github.com/skypjack/entt) will be used to allow for many moving parts. An entity component system was chosen to help take advantage of powerful CPUs. Although classic OOP architecture is much easier to understand, ECS offers many advantages, especially in terms of performance, that you could never achieve with OOP.

#### Portability
The engine itself will be portable to many devices. This is to allow for all sorts of people to use the engine. All libraries and dependencies of this project will also be portable.

#### Networking
Who doesn't love playing with your friends? The Minty engine will have built-in networking capabilities to make it easy (easier- its not easy) to implement multiplayer games.

#### Ease of Use
After the base of the engine has been designed, a user interface will be created to help advance the creation of games using the engine.

### Inspiration
There are multiple sources that inspired this project.
* [John Lin's Sandbox](https://www.youtube.com/@johnlin9665), a stunningly beautfil voxel tech demo.
* [Teardown](https://teardowngame.com), a well made voxel game about destroying the environment.

### Technology
The Minty Engine will use a multitude of advanced and new technologies to make the most of the computer being used. This includes, but is not limited to:

* [Vulkan](https://www.vulkan.org), for graphics.
* [GLFW](https://www.glfw.org), for windows.
* [GLM](https://github.com/g-truc/glm), for linear mathematics.
* [EnTT](https://github.com/skypjack/entt), for the entity component system.