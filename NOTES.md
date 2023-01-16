# Renderer

### Requirements
-   Has Camera System
-   Want simple environment lighting
-   Want built in suite of basic shaders
-   Want window class to encapsulate glad / glfw
    -   Need to have control to add key events
    -   Want this to control imgui set up
-   Each entity should have render and integrate function
-   components should have mechanism for talking to each other
    -   Getting data for a ball when a cloth drapes over it
-   Want a replay system where we can simulate once and serialize to file and then reload and render
    -   This is useful if the simulation is really slow
-   Should be able to draw from an input handler for object controls

# TODO:
- [ x ] Implement Camera System
- [ x ] Add Orientation unit coordinate
- [ x ] Implement Normal Transformation
- [ x ] Add some basic phong shading
- [ x ] Ensure that you can perform updates on mesh data
- [ x ] Integrate ImGUI + IMPlot
- [ x ] Integrate stb image
- [  ] Implement system for rendering / substeping 
- [ x ] Add energy plot

# Texture Class
### Requirements
- Want to be able able to resuse the texture
- Want bind / unbind capability
- Want to be able to load images with stb_image
- Needs to have interactivity with shader / material

# Input Class
### Requirements
- If possible I don't want to have input update function
- Want to be able to register keys to string
- Want to have a press and held event at least

# Extras needed
- [ ] Simulation / engine class
- [ ] move materials into separate folder
- [ ] Add line cube object
- [ ] Add folder for models used by all sims
- [ ] Print register keys like in libigl during input init
- [ ] Possibly add engine that you can register entities to and have it automatically apply uniforms and call draw / update
- [ ] Figure out a better way to do the glfw / glad imports
