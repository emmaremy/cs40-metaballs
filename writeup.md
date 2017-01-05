# 3D Metaballs
#### Rachel Diamond and Emma Remy

### Where to find our code & How to run it
Our code is available on our shared GitHub repository. It can be run by navigating into the `final` directory, creating a build directory with `mkdir build`, running `cmake ..`, running `make -j8`, then running `./final`.

### What does our project do?
We implemented 3-dimensional metaballs. You can interact with our program by using the following keypresses:
* d: increase depth
* D: decrease depth
* t: increase threshold
* T: decrease threshold
* b: add blob
* B: remove blob
* s: draw spheres instead of metaballs
* c: toggle culling
* p: toggle polygon mode
* l: change shader

Most of these interactions are for fun and amusement, but note that going into triangle-polygon mode, toggling culling, and moving between drawing spheres and metaballs clearly shows the differences between metaballs and just a bunch of overlapping spheres.

### How we did it

#### Methods
Our implementation of metaballs uses OpenGL, shaders, Phong lighting, and raytracing.

#### Overview and inspiration
A previously mentioned, we implemented 3-dimensional metaballs. Metaballs are spheres that, upon colliding, mush together to form blob shapes instead of just overlapping spheres. We were interested in this topic because we were clicking through the links of the suggested topics and found [this 2D demo](http://jamie-wong.com/2014/08/19/metaballs-and-marching-squares/). This demo had cool-looking animations and the math involved in creating the mushiness of the spheres (circles, in the case of the original 2D demo) seemed both interesting and challenging.

Originally, we considered doing 2-dimensional metaballs with cool colors but, because there was such an emphasis on lighting in this course, we decided to implement 3D metaballs. We searched and found both videos of other implementations of 3D metaballs that looked pretty cool and some writeups with advice on how to implement 3D metaballs in OpenGL. Even in 3D, we wanted to have fancy colors and various forms of interactivity, some of which we were ultimately able to implement and others of which we were not.

Our process for building our implementation of 3D metaballs followed the order of the following subsections, where we discuss different elements of our implementation.

#### Phong lighting
We used the phong lighting model to add 3D shading to the metaballs. Phong lighting incorporates ambient, diffuse, and specular light. The ambient light is the same everywhere, which gives a low level of lighting to the entire scene. The diffuse light depends on the surface normals and the light position(s), it gives shading to the scene. The specular light also depends on the surface normals and light position(s), but it is much more focused on particular regions, so it contributes bright spots. To implement phong lighting we used the same vertex shader from the w06/lighting demo, which showed how phong lighting can be used to make a sphere look 3-dimensional.

#### Bouncing blobs
The underlying geometry for 3D metaballs is a group of spheres that bounce around the screen. To store the necessary information, we defined a blob class that tracks of the center location, radius, and velocity of each underlying component. The blob class serves basically the same function that the planet class did for our model solar system. To make the blobs bounce off the sides of the screen, we implemented a move method. While the blob is within the boundaries of the screen, the move method simply increments the center location based on the velocity and the timestep. If the blob goes off the screen in any direction, the move method reverses the appropriate component of the velocity and moves the blob back onto the screen. At this point we were able to render each blob as a sphere, so we had a bunch of spheres bouncing around the screen. Due to the depth buffer, these spheres intersect in a way that looks similar to metaballs, but the geometry is fundamentally different.

#### Marching cubes (and Grid)
Our next step was to calculate the geometry specific to metaballs. We defined a grid class to handle this geometry. We calculate the geometry using the marching cubes algorithm. Marching cubes considers one grid cell at a time, and uses information about how far each of the eight vertices of the cell is from the nearest blob to generate up to five triangles on the surface of the metaball. We implemented this in our marchingcubes class using code from [this website](http://paulbourke.net/geometry/polygonise/).

The draw method within the grid class is responsible for looping though each cell in the grid, calculating the distance to the nearest blob for each vertex, calling the marchingcubes method that generates triangles based on the vertex distances, saving the generated triangles, calculating the triangle normals, loading the geometry into the VBO, and finally calling glDrawArrays.


#### Interactivity
In our original proposal, we wanted to have user interactivity to add and subtract blobs and slow down and speed up the animation. Due to the processing constraints in our actual implementation, the speed up/slow down interactivity was not feasible. However, we did add several other keypress features:

* Depth: All of our metaballs, blobs, and spheres are at the same depth. However, our code expects height, width, and depth to be equal. We were curious what would happen if we constrained the depth and made it smaller. We discovered that doing so did not change the appearance very dramatically until the depth becomes small enough to cause culling, and having the shorter depth does not require so many marching squares calculations and therefore the program runs faster. We also thought that the behavior with extremely small depth (e.g. depth = 2) was interesting (although no longer really metaballs), so we created a keypress to allow for variation in depth and easier exploration of these different behaviors.
* Threshold: As discussed above, the calculations for the triangles of our metaballs are dependent on a threshold. Initially, we were not sure what the threshold value should be, so we experimented with different numbers for a bit. We discovered that threshold should be proportional to the size of the grid, and then we decided to allow easier manipulation and exploration of the different threshold values by adding this keypress interactivity.
* Blobs: One of our original goals was to allow for the creation and removal of blobs. This feature does not necessarily serve to help the user understand metaballs or any of the details of our implementation, but is fun and can lead to more or less collision and mushing between blobs.
* Spheres: While implementing marching squares, sometimes we were uncertain if what we had created was actually metaballs or just overlapping spheres, especially with specular lighting, which emphasizes the dips between the blob-parts with shadow and highlight. We created this keypress toggle between spheres and metaballs to be able to really tell that we had implemented metaballs.
* Culling: This key binding is leftover from the phong lighting demo we started with, but we decided to keep it because, when used with the polygon toggle and the sphere/blob toggle, this makes it really clear the differences between spheres and metaballs.
* Polygon mode: This key binding is also leftover from the phong lighting demo. As with culling, we decided to keep this keypress because of how clear the differences are between spheres and metaballs when polygon mode and culling are on.
* Shader: This key binding is also leftover from the phong lighting demo. We removed specular lighting from the default shader because the mushiness of metaballs was easier to see without it. The second shader includes specular lighting, add does per-fragement rather than per-vertex lighting. We chose to keep this keypress to see the differences between lighting with and without specular lighting.

#### Reflections, Challenges & Conclusions

Overall, we are happy with what we have created - we were able to implement most of the features that we listed as project goals in our proposal.

One of the biggest challenges of this project was figuring out how to load new geometry into the VBO, since in all of our previous labs this was given to us. We also had a problem where we were allocating the wrong amount of space for the normals in the VBO which made it so that nothing rendered.

It would have been nice to optimize our program more so that it is not as slow. We did all of our computation on the CPU, without parallelization, and with more time we would have liked to parallelize our computation on the GPU.

### References

* http://jamie-wong.com/2014/08/19/metaballs-and-marching-squares/ (2D metaballs, our original inspiration)
* `/examples/w06-lighting` (phong lighting demo, the basis for our project)
* https://www.youtube.com/watch?v=BxAWDX-RboQ (example)
* https://www.youtube.com/watch?v=_dYCUaKKezs (example)
* https://www.youtube.com/watch?v=ocH_7uKTeYA (example)
* http://paulbourke.net/geometry/polygonise/ (marching cubes, the source of our external code)
* http://jamie-wong.com/2016/07/06/metaballs-and-webgl/ (inspiration)
* http://malideveloper.arm.com/downloads/deved/tutorial/SDK/linux/2.4/metaballs.html (3D metaballs in OpenGL, but did not use directly)
* http://blackpawn.com/texts/metanormals/default.html (getting normals from gradient, ultimately did not implement this)
* Many thanks to Andy for responding to our frantic Piazza posts and finding our bugs!
