# DearImGui Demo/Tutorial
### realizado por Fernando Arciga
### para la materia Computación Gráfica e Interacción Humano Computadora
> Antes de ejecutar se debe hacer un git clone https://github.com/ocornut/imgui.git en la raiz del proyecto

## Configuración
- Dentro de visual studio se debe añadir imgui;imgui\backends; en C/C++ > General
- Se debe añadir todos los .cpp y .h de la carpeta imgui y de la carpeta backends dentro de imgui sólo se deben añadir imgui_impl_glfw.h y imgui_impl_opengl3.h y sus respectivos cpp
- En imgui_impl_glfw.cpp se debe quitar GLFW/ de las líneas 97 y 104
