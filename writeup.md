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

#### Overview and inspiration
A previously mentioned, we implemented 3-dimensional metaballs. Metaballs are spheres that, upon colliding, mush together to form blob shapes instead of just overlapping spheres. We were interested in this topic because we were clicking through the links of the suggested topics and found [this](link to Jamie Wong 2D demo). This demo had cool-looking animations and the math involved in creating the mushiness of the spheres (circles, in the case of the original 2D demo) seemed both interesting and challenging. We continued to research and found some videos of 3-dimensional metaballs.

#### Stuff we stole from that lighting demo

#### Making spheres bounce

#### Marching cubes
Link to outside code

#### Interactivity

#### Other notes and challenges (e.g. things we changed from our original proposal)

### References

* Thanks Andy!
