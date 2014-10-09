smooth_trianglerasterization_flatshading_gourandshading
=======================================================
CAP 5705 – Homework 2 Start Early!
Due date: October 10, 2014
Graphics Pipeline – Rasterization

Smooth1 is an example of an OpenGL program that loads a mesh (given in an .OBJ file) and renders it with various features. Learn how smooth works so that you can modify the code. You can find meshes in .OBJ format from various online sources (that need to be properly acknowledged).

You are to modify the smooth program to implement a basic graphics pipeline for ren- dering triangular meshes. You may use all the functions in smooth for loading an .OBJ file and the data structures for maintaining and accessing vertices and faces of a triangular mesh.

Rasterization
Implement a rasterization algorithm that takes the coordinates of a 2-D triangle and gen- erates fragments using the scan-line algorithm. You can use the glDrawPixels method for rendering the generated fragments. 30pt
  Modify the smooth program so that it will use your rasterization algorithm to draw 2-D triangles that are projected from the   loaded 3-D model (have a look at gluProject).
  Use “data/cube.obj” as a starting object to test your implementation.
  Keep the previous functionality of the smooth program; allow the user to switch to your pipeline by pressing the key ’y’. This key will toggle onto and off of your implementation of the pipeline.



Z-Buffer and Shading
In this section you will complete the implementation of the graphics pipeline by incorporat- ing the Z-buffer and shading steps into the rasterization framework that you developed.
  As you recall gluProject returns the depth values of projected vertices (i.e., z′). To determine the depth values for each fragment, that the scan-line algorithm draws, you will implement the barycentric interpolation on the depth values associated to each of the projected vertices.The interpolated depth values will provide a Z-buffer for each triangle which is being rendered. The final stage of the Z-buffer algorithm can be implemented as you blend each rendered triangle to the final image (i.e., frame-buffer); simply over a pixel value only if it is further to the viewer from the new fragment you are currently drawing. 20pt



  Implement flat shading for your pipeline. Query each triangle for its normal, and perform the local illumination (ambient, diffuse, specular) per triangle. Simply use the current lighting setup (i.e., direction, intensity, ...) that is available in smooth for your shading calculations. 20pt


  Modify your pipeline to include Gouraud shading – vertex shader and interpolate RGB’s produced by the shader across the triangle. 10pt


  Use “data/cube.obj” as a starting object to test your implementation. Once tested, examine your pipeline using more sophisticated models.


Submit a report (PDF document) that, along with images, documents the results for each step of the homework. Moreover, in the report explain any differences between the images rendered in your pipeline and the image produced in smooth. Also comment on why your implementation is slower than the original implementation. 20pt



Acknowledgments
This is a single-person project. What you submit needs to be your own work. If you are using resources for any part of the homework (including ideas, source code and images), makes sure to include proper citation and clear acknowledgments in your report.
