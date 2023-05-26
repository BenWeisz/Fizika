### May 25, 2023
-   Start work on data pipeline design (no implemention)

# Todo List
- [✔] Implement system for rendering / substeping
- [✘] Build data pipeline 
- [✘] Simulation / engine class
- [✘] Possibly add engine that you can register entities to and have it automatically apply uniforms and call draw / update
- [✘] Figure out a better way to do the glfw / glad imports
- [✘] Fix issue with having to place \n at end of obj file
- [✘] Migrate to assimp for model loading
- [✘] Add a define function for resource paths

---

## Data Pipeline needs:
-   have preprocessing pipeline before load time for model
    -   Can apply initial rotations / translations
    -   Can compute normals if missing via compute shader
    -   Can pack into buffer format based on material if needed
    -   These should be packed into a single using zip
        -   Should have option for static geo, dynamic geo   
-   Should remove heavy data loading from mesh.cpp
    -   Mesh should unzip the model file, read the descriptor, load the data into the buffers


### Proposed zip layout
-   model.xml --> describes the model
-   vertex.bin --> vertex data binary form
-   index.bin --> index data binary form
-   <material_file>.xml --> The material file
-   ~Assorted list of texture files~

model.xml
-   property: static / dynamic geo
-   property: num vertices
-   property: num primitives
-   property: primitive type: tri / lines
-   property: material name
<Model>
    <Geometry type="STATIC|DYNAMIC">
        <Vertices count="1000"/>
        <Primitives count="1000" type="TRIANGLES|LINES"/>
    <Geometry/>
    <Material path="./some/path.mat"/>
</Model>

*.mat
This format will specify EXACTLY what is in the shader files and the bin files. The pipeline will have recepe that it follows and it will pack the model with the exact data, no more.
<Material>
    <Shaders>
        <Source type="vertex"></Source>
        <Source type="fragment"></Source>
    </Shaders>
    <Layout>
        <LayoutElement name="iPosition" attribSize="3"/>
        <LayoutElement name="iNormal" attribSize="3"/>
        <LayoutElement name="iUV" attribSize="2"/>
    </Layout>
    <Textures>
        <Texture path="../some/path.png" index="0"/>
    </Textures>
</Material>

### ModelName.xml
<Recipe>
    <ModelData path="./some/path.obj"/>
    <Material path="./some/path.mat"/>
    <Transform>
        <Scale x="1.0" y="1.0" z="1.0"/>
        <Rotate x1="1.0" z="1.0" x2="1.0"/>
        <Translate x="1.0" y="1.0" z="1.0"/>
        <Unitize/> --> makes everything fit in 1x1x1 box
    </Transform>
</Recipe>