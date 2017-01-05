# Final Project Proposal
### Rachel Diamond and Emma Remy
### Metaballs
 
 We are implementing three-dimensional metaballs. Metaballs occur when spheres intersect and merge. Our implementation will include colors, lighting, animation, and some level of interactivity (adding spheres, controlling speed, etc.). The final product will be an animation of bouncing and merging colorful metaballs.
 
 We are interested in metaballs because they are visually and mathematically compelling.

# Course Concepts Used

 Our implementation of metaballs will require the use of Phong lighting, raytracing, animation (of spheres, as in the planet lab), support for interactions via keypress, fragment and vertex shaders, OpenGL, and some of the math we've used in the course so far, such as calculating normals on the sphere.
 
 In addition to these concepts we learned in class, we will be using more complex geometry and therefore more complex math (calculating normals on blobby fused metaballs rather than just spheres), interaction between shapes upon "collision" (more interaction than orbiting each other), and the concepts and math used to "smooth" the metaballs, like marching squares.

# Other software Tools

* openGL
* raytracing
* implementation of triangle formation on 3D grid cells (includes a table of triangle edges for each orientation of cube vertices)

# Goals

### Short-term goals

* bouncing balls within a set window (potentially intersecting but not merging)
* Phong lighting
* 3D metaballs constrained to move in a plane, likely without smoothness, depending on time

### Long-term goals

* some nice colors
* interactivity (adding balls, subtract ball with keypress)
* 3D metaballs with depth (not constrained to a single plane)
* smoothness
* time/speed control with keypress

### Reach goals

* color blending
* ray tracing or marching for shadows
* add and subtract ball with click (maybe - to specify location)
* more interactivity
* pretty UI
* idea: two types of sphere that only merge with their own type

# References

* http://jamie-wong.com/2014/08/19/metaballs-and-marching-squares/ (2D metaballs)
* /examples/w06-lighting (phong lighting)
* https://www.youtube.com/watch?v=BxAWDX-RboQ (example)
* https://www.youtube.com/watch?v=_dYCUaKKezs (example)
* https://www.youtube.com/watch?v=ocH_7uKTeYA (example)
* http://paulbourke.net/geometry/polygonise/ (marching cubes)
* http://jamie-wong.com/2016/07/06/metaballs-and-webgl/ (pretty, but probably not relevant)
* http://malideveloper.arm.com/downloads/deved/tutorial/SDK/linux/2.4/metaballs.html (3D metaballs in openGL)
* http://blackpawn.com/texts/metanormals/default.html (getting normals from gradient)

# Assessment

Our project will be a success if we succeed in implementing metaballs with raytracing and phong lighting, such that the balls animate correctly, are lit correctly, merge correctly (our math calculating the shapes and lighting for the merged shapes are accurate), and the user can interact with the animation in some simple ways, like changing the speed.
