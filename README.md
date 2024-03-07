# Minty Engine
A 3D game engine.

NOTE: This project is in development. More info on how to use, etc. will come in due time. Please check out the development branch for the most recent features that have been added.

## About

### Motivation
Given the news of [Unity changing their pricing model](https://www.theverge.com/2023/9/12/23870547/unit-price-change-game-development), I decided that using a game engine backed by a large company was risky. Given my background and tendancy to work on random hobby projects in the field of game development, I decided that I would create my own game engine for any future games I wanted to develop long term. That said, I present the Minty game engine.

The goals of this project are as follows:

#### Performance
Performace, in terms of the CPU and the GPU. On the CPU, libraries such as [EnTT](https://github.com/skypjack/entt) will be used to allow for many moving parts. An entity component system was chosen to help take advantage of powerful CPUs. Although classic OOP architecture is much easier to understand, ECS offers many advantages, especially in terms of performance, that you could never achieve with OOP. Some OOP is still used within the engine, but it is not the core.

#### Portability
The engine itself will be portable to many devices. This is to allow for all sorts of people to use the engine. All libraries and dependencies of this project will also be portable.

#### Networking
Who doesn't love playing with your friends? The Minty engine will have built-in networking capabilities to make it easy (easier- its not easy) to implement multiplayer games. This might not be the first feature, but I hope to implement it one day...

#### Ease of Use
After the base of the engine has been designed, a user interface will be created to help advance the creation of games using the engine, as well as support for a scripting language (likely Lua, but it is still TBD).

### Inspiration


### Technology
The Minty Engine will use a multitude of advanced and new technologies to make the most of the computer being used. This includes, but is not limited to:

* [Vulkan](https://www.vulkan.org), for graphics.
* [GLFW](https://www.glfw.org), for windows.
* [GLM](https://github.com/g-truc/glm), for linear mathematics.
* [EnTT](https://github.com/skypjack/entt), for the entity component system.
* [stb](https://github.com/nothings/stb), for loading images from the disk.
* [SoLoud](https://github.com/jarikomppa/soloud), for loading and playing audio.
* [Dear ImGui](https://github.com/ocornut/imgui), for the editor user iterface.