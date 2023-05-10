# HoloDemtect
## A mixed reality framework for cognitive stimulation through interaction with objects

[![UnrealEngine](https://img.shields.io/badge/UnrealEngine-4.27-green?style=flat-square)](https://img.shields.io/badge/UnrealEngine-4.27-green?style=flat-square)
![GitHub repo size](https://img.shields.io/github/repo-size/3dperceptionlab/HoloDemtect?style=flat-square)
![GitHub last commit](https://img.shields.io/github/last-commit/3dperceptionlab/HoloDemtect?color=orange&style=flat-square)

In this work, we propose a system for the evaluation of potential signs of dementia. To this end, we have defined three groups of tasks that stimulate mental and motor abilities of the users. The application is divided into two phases, in the first one the supervisor of the system sets up the environment by selecting the task and spawning the necessary objects with the help of QR codes. In the second one, the user performs the tasks while an evaluation is carried out by the supervisor and the system, by actively collecting data from the user's actions.

![HoloDemtect Pipeline](img/Pipeline.jpg)

We have distinguished three groups of tasks based on how they are completed: 
- Sequential tasks, which evaluate the user's memory and motor skills are evaluated by assessing their ability to keep track of a list of objects. An example of such a task is do the shopping given a list.
- Semantic pair matching tasks, in which the available objects have to be grouped into pairs, taking into account the semantic and contextual relationship that the objects have with each other.
- Logical organisation tasks, in which the user has to place the objects according to a given set of instructions or social conventions. For example, the user has to arrange the table with the appropriate virtual objects.



## How to use  

First, install Unreal Engine 4, version 2.27 or upper.

```bash
# clone project   
git clone https://github.com/3dperceptionlab/HoloDemtect.git
```

There are two ways to run the AR application:

- **Run the application on our PC**: Open the UE4 project and connect the HoloLens to Unreal using the Holographic Remoting Player app on the HoloLens. This requires the Hololens plugin to be installed in Unreal. Then go to Project Settings > Platforms > Windows Mixed Reality and connect the device by setting its IP.

- **Deploy the application to the HoloLens headset**: To deploy to the device, build and upload the package (.appxbundle and select "Allow me to select framework packages" and upload the .appx file) to the Mixed Reality Portal.
If this is your first time, you will need to generate a key. To do this in UE4, go to Edit>Project Settings>Platform>HoloLens, in Packaging/Signing Certificate: Generate New.

## Posible building errors

> #### Not found some asset while building
>
> - Check the files in `ProjectFolder > Saved > Cooked` 
>

> #### Failed to build "Saved\UATTempProj.proj":
>
> - Recompile (not just compile) AutomationTool Visual Studio project
> 



## Authors

This project has been carried out by:
- David Mulero-Perez ([dmulero@dtic.ua.es](mailto:dmulero@dtic.ua.es))
- Manuel Benavent-Lledo ([mbenavent@dtic.ua.es](mailto:mbenavent@dtic.ua.es))
