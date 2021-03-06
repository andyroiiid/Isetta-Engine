#include "RenderSystem/RenderSystem.h"
#include "InputSystem/InputSystem.h"

bool running{ false };

void windowCloseListener(GLFWwindow* win) {
    running = false;
}

void keyEventListener(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    running = false;
}

int main(int argc, char** argv) {
    RenderSystem renderSystem{ "Sample" };
    InputSystem inputSystem{};

    running = renderSystem.StartUp();
    inputSystem.RegisterWindowCloseCallback(renderSystem.winHandle, windowCloseListener);
    inputSystem.RegisterKeyCallback(renderSystem.winHandle, keyEventListener);

    // Potential scene graph part
    H3DRes envRes = h3dAddResource(H3DResTypes::SceneGraph, "test/Low-Poly-Racing-Car.scene.xml", 0);

    H3DRes lightMatRes = h3dAddResource(H3DResTypes::Material, "materials/light.material.xml", 0);

    if (!h3dutLoadResourcesFromDisk(renderSystem.getResourcePath().c_str())) {
        h3dutDumpMessages();
        return false;
    }

    H3DNode env = h3dAddNodes(H3DRootNode, envRes);
    h3dSetNodeTransform(env, 0, -20, 0, 0, 0, 0, 1, 1, 1);

    H3DNode light = h3dAddLightNode(H3DRootNode, "Light1", lightMatRes, "LIGHTING", "SHADOWMAP");
    h3dSetNodeTransform(light, 0, 200, 600, 0, 0, 0, 1, 1, 1);
    h3dSetNodeParamF(light, H3DLight::RadiusF, 0, 2500);
    h3dSetNodeParamF(light, H3DLight::FovF, 0, 90);
    h3dSetNodeParamI(light, H3DLight::ShadowMapCountI, 1);
    h3dSetNodeParamF(light, H3DLight::ShadowMapBiasF, 0, 0.01f);
    h3dSetNodeParamF(light, H3DLight::ColorF3, 0, 1.0f);
    h3dSetNodeParamF(light, H3DLight::ColorF3, 1, 1.0f);
    h3dSetNodeParamF(light, H3DLight::ColorF3, 2, 1.0f);
    h3dSetNodeParamF(light, H3DLight::ColorMultiplierF, 0, 1.0f);


    while (running) {
        renderSystem.Update();
    }
    renderSystem.ShutDown();

    return 0;
    //KnightSample app(argc, argv);
    //return app.run();
}
