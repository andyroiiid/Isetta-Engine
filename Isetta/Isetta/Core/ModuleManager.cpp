/*
 * Copyright (c) 2018 Isetta
 */
#include "ModuleManager.h"
#include "Audio/AudioModule.h"
#include "Core/Graphics/RenderModule.h"
#include "Core/Input/InputModule.h"
#include "Core/Window.h"

namespace Isetta {
ModuleManager::ModuleManager() {
  audioModule = new AudioModule{};
  windowModule = new WindowModule{};
  renderModule = new RenderModule{};
  inputModule = new InputModule{};
}
ModuleManager::~ModuleManager() {
  delete windowModule;
  delete audioModule;
  delete renderModule;
  delete inputModule;
}
void ModuleManager::StartUp() {
  audioModule->StartUp();
  windowModule->StartUp();
  renderModule->StartUp(windowModule->winHandle);
  inputModule->StartUp(windowModule->winHandle);
}

void ModuleManager::Update() {
  audioModule->Update();
  inputModule->Update();
  renderModule->Update();
  windowModule->Update();
}

void ModuleManager::ShutDown() {
  audioModule->ShutDown();
  renderModule->ShutDown();
  inputModule->ShutDown();
  windowModule->ShutDown();
}
}  // namespace Isetta