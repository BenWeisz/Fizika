### June 1, 2023
-   Set up loading for material file in data build

### May 31, 2023
-   Redesign plan databuild system
-   Implement loading of the model file

### May 28, 2023
-   Start work on the DataBuild system, laying foundation for
    Pipeline class and setting up data build target

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
    <Geometry mode="STATIC|DYNAMIC" type="TRIANGLES|LINES">
        <Vertices count="1000"/>
        <Primitives count="1000"/>
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
<Model>
    <Geometry path="./some/path.obj" mode="STATIC|DYNAMIC"/>
    <Material path="./some/path.mat"/>
    <Pipeline>
        <Scale x="1.0" y="1.0" z="1.0"/>
        <Rotate x1="1.0" z="1.0" x2="1.0"/>
        <Translate x="1.0" y="1.0" z="1.0"/>
        <Unitize type="VERTEX|NORMAL"/> --> makes everything fit in 1x1x1 box
    </Pipeline>
</Model>

Steps:
1. Load Model file for reading
2. Load Material file for reading
   1. Record shader sources paths
   2. Record names of attributes and sizes
   3. paths of the images
3. Load model data file for reading
   1. Record kind of geometry is to be used (static / dynamic)
   2. load only the data required from the geometry file
      1. For now we only handle static geos
         1. Process data into index and vertex buff style vectors using unordered maps
      2. record primitive type, length of each of the arrays
4. Run the pipeline runner
   1. Loops through tags in order and kicks off specific pipes
5. create output model file
6. write out index and vertex buffers
7. package files into zip file along with materials, shaders and image files 

Implementation:
- Variables: have struct for data build state and all its variables
- Functions: 
  - DataBuild(const std::string& modelFilePath); 
  - static tinyxml2::XMLDocument* LoadXMLFile(const std::string& path)
  - void InitDataBuild();
  - void LoadMaterialFile(const std::string& materialFilePath);
  - void LoadGeometryFile(const std::string& geometryFilePath);
  - Pipeline* LoadPipeline(tinyxml2::XMLElement* pipelineElements);
  - bool WriteData();
  - bool PackData(); 