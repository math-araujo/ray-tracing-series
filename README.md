# Ray Tracing Series


Implementation of [Peter Shirley's Ray Tracing Book Series](https://raytracing.github.io/)

> - [Scenes](#Scenes)
> - [Building](#Building)
> - [TODO](#TODO)

<!-- -->
## Scenes
Some renders done throught the books:

| | |
| :---: | :---: |
| ![In One Weekend Final Scene](images/20c.final_scene_500samples.png?raw=True) <br/> Random spheres with different materials (lambertian, metal and dielectric) | ![Emissive Material with Perlin Noise Texture](images/32e.other_viewpoint_light_rectangle_and_sphere_400samples.png?raw=True) <br/> Emissive material and a sphere with noise texture (made using Perlin noise) |

| | |
| :---: | :---: |
| ![Classic Cornell Box](images/35.classic_cornell_box.png?raw=True) <br/> Classic Cornell Box | ![Cornell Box with Smoke](images/36b.smoke_cornell_box_yellowblue_1.png?raw=True) <br/> Cornell Box with colored smoke |

| | |
| :---: | :---: |
| ![Next Week Final Scene with 50 samples per pixel](images/37c.next_week_50samples.png?raw=True) <br/> *Next Week* final scene with 50 samples per pixel | ![Next Week Final Scene with 1000 samples per pixel](images/37b.next_week_1000samples.png?raw=True) <br/> *Next Week* final scene with 1000 samples per pixel |


Some custom made scenes:


![Wikipedia Path Tracing with Ambient Light](images/wikipedia_ambientlight.png?raw=True)

![Wikipedia Path Tracing with Diffuse Light](images/wikipedia_diffuselight.png?raw=True)

![Recursives Spheres](images/recursive_glass.png?raw=True)

![Bunny Point Cloud](images/bunny_point_cloud.png?raw=True)
Note: the model used in this scene is the famous Stanford Bunny, which can be downloaded
in [this link](https://graphics.stanford.edu/~mdfisher/Data/Meshes/bunny.obj), and is part of the
[Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep/).

![Diffuse Light Bunny Point Cloud](images/night_bunny_point_cloud.png?raw=True)

<!-- -->
## Building

Requirements: CMake >= 3.12

Download [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) and move it into `src/external/`

`cd build`

`cmake ..`

`cmake --build .` (add other CMake options as desired e.g. `cmake --build . --config Release`)

<!-- -->
## TODO

* Implement [The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)

* Refactor the code to replace `std::shared_ptr` with const-references whenever it's possible.

* Refactor the code to accept a command-line argument to choose the scene, instead of hard-coding the choosen scene in a variable.

* Check [v4.0](https://github.com/RayTracing/raytracing.github.io/milestone/16) when it comes out, then refactor or add any substantial changes.