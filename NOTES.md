# Renderer

### Requirements
-   Way to draw geometries while swaping out the material
-   Models are array of mesh-material pairs
-   Way to store material
-   Render engine that can render a mesh-material pair
-   Has Camera System
-   When you render a model youre rendering all its mesh-material pairs
-   Be able to draw a model thats a cloth with lines plus points on the verts
-   Want mesh factory for simple shapes
-   Want simple environment lighting
-   Want built in suite of basic shaders
-   Want window class to encapsulate glad / glfw
    -   Need to have control to add key events
    -   Want this to control imgui set up

### Implementation
-   Model: class holding array of:
    -   Pairs of Mesh and Material structs
    -   When pair is added, mesh gets generated a vao, vbo
-   Mesh:
    -   initially only holds the only the data you need for the geometry
    -   Also holds info on primitive type
    -   has function to regenerate vao, vbo. only to be called if material changes
    -   [ ] Function to read obj files for mesh, lines, points
-   Material
    -   [x] Holds shader reference 
    -   [x] names and values of uniforms
    -   [ ] functions to set uniforms
    -   [x] the vertex layout
    -   [x] Functions to get layout
    -   [x] Functions to bind, unbind
    -   [x] Function to get shader
-   VertexBufferLayout
    -   [ ] Get vertex size function so that we can check the data size of the vertex and compare it to the layout before even attempting to draw a model
-   Renderer
    -   Since model is just data, render is responsible for complete render


# The ECS

### Requirements
-   Each entity should have render and integrate function
-   components should have mechanism for talking to each other
    -   Getting data for a ball when a cloth drapes over it
-   Want a replay system where we can simulate once and serialize to file and then reload and render
    -   This is useful if the simulation is really slow
-   Should be able to draw from an input handler for object controls

/// New Plan
- Have a mesh class that only holds and loads the raw data for a mesh
- Have a model class hold a reference to the shader, it gets from the outside
    -   Should create and hold the vao, vbo and the vertex layout