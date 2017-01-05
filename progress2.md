# Second progress report
### Rachel Diamond and Emma Remy
### Metaballs

At our last progress report, we had successfully created bouncing 3D spheres with Phong lighting. Since then, we have been working on implementing the actual mushiness of metaballs.

We took code for marching cubes from [this website](http://paulbourke.net/geometry/polygonise/). We also created a grid object to help us implement marching cubes. Currently, the grid constructor calculates the locations of the necessary triangles based on nearby blob-spheres. We are calling the draw method for grid in our main program. However, currently nothing actually draws.

We need to de-bug our triangles that are currently not drawing, and we need to move the code calculating triangle locations to the grid's draw method so that the grid changes with time increments and the metaballs will actually animate.
