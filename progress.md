# First progress report
### Rachel Diamond and Emma Remy
### Metaballs

Currently we have spheres of varying sizes with Phong lighting that bounce around independently and off the sides of the window.

We started with a demo that included Phong lighting and perspective, as well as the classes included in /examples/common. With some effort, we managed to create the necessary CMakeLists and move the correct files into our final repository. From here, we wanted to make the sphere in the demo move. We created a QTimer and updateTime function, to allow for animation. We created a "blob" class, similar to the "planet" class in our solar system model, which has a radius, center, and velocity. We created a matrixstack to transform the generic sphere into a blob with the correct characteristics. Our blob move function ensures that the blobs do not go off the screen. We have also implemented a function to generate blobs with random radii, centers, and velocities. 
Our next step will be to actually implement the smushing of metaballs. We may also add a few lights and some interaction with the timer, so a keypress changes the speed of the animation or pauses everything.
