/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/ModuleManager.h"
#include "Audio/AudioModule.h"
#include "Core/Memory/MemoryManager.h"
#include "Graphics/RenderModule.h"
#include "Graphics/Window.h"
#include "Input/InputModule.h"
#include "Networking/NetworkingModule.h"

namespace Isetta {
ModuleManager::ModuleManager() {
  memoryManager = new MemoryManager{};
  audioModule = new AudioModule{};
  windowModule = new WindowModule{};
  renderModule = new RenderModule{};
  inputModule = new InputModule{};
  networkingModule = new NetworkingModule{};
}
ModuleManager::~ModuleManager() {
  delete windowModule;
  delete audioModule;
  delete renderModule;
  delete inputModule;
  delete memoryManager;
  delete networkingModule;
}

void ModuleManager::StartUp() {
  memoryManager->StartUp();
  audioModule->StartUp();
  windowModule->StartUp();
  renderModule->StartUp(windowModule->winHandle);
  inputModule->StartUp(windowModule->winHandle);
  networkingModule->StartUp();
}

void ModuleManager::SimulationUpdate(float deltaTime) {
  audioModule->Update(deltaTime);
  inputModule->Update(deltaTime);
  networkingModule->Update(deltaTime);
  memoryManager->Update();
}

void ModuleManager::RenderUpdate(float deltaTime) {
  renderModule->Update(deltaTime);
  windowModule->Update(deltaTime);
}

void ModuleManager::ShutDown() {
  networkingModule->ShutDown();
  audioModule->ShutDown();
  renderModule->ShutDown();
  inputModule->ShutDown();
  windowModule->ShutDown();
  memoryManager->ShutDown();
}
}  // namespace Isetta
