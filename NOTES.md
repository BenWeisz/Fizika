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

### Camera / Windowing System