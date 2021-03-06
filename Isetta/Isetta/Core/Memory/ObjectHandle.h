/*
 * Copyright (c) 2018 Isetta
 */
#pragma once
#include "Core/Debug/Logger.h"
#include "Core/IsettaAlias.h"
#include "Core/Memory/MemoryManager.h"
#include "Utilities.h"

namespace Isetta {
class HandleEntry {
 private:
  HandleEntry() = default;
  ~HandleEntry() = default;

  void Set(U32 uniqueID, void* ptr, bool isEmpty);

  U32 uniqueID{};
  void* ptr{};
  bool isEmpty{true};

  template <typename T>
  friend class ObjectHandle;
  friend class MemoryManager;
};

template <typename T>
class ObjectHandle {
 public:
  ~ObjectHandle() = default;
  T* operator->() const;
  T& operator*();

  friend bool operator<(const ObjectHandle& a, const ObjectHandle& b) {
    // TODO(YIDI): Optimize this after debugging
    PtrInt aAddress = reinterpret_cast<PtrInt>(a.GetObjectPtr());
    PtrInt bAddress = reinterpret_cast<PtrInt>(b.GetObjectPtr());
    bool isASmaller = aAddress < bAddress;
    return isASmaller;
  }

 private:
  ObjectHandle();
  T* GetObjectPtr() const;
  void EraseObject() const;
  U32 uniqueID;
  U32 index;

  friend class MemoryManager;
};

template <typename T>
ObjectHandle<T>::ObjectHandle() {
  uniqueID = MemoryManager::nextUniqueID++;
  HandleEntry* entry = nullptr;

  for (U32 i = 0; i < MemoryManager::maxTableSize; i++) {
    if (MemoryManager::handleEntryTable[i].isEmpty) {
      index = i;
      entry = &MemoryManager::handleEntryTable[index];
      break;
    }
  }

  if (entry == nullptr) {
    throw std::out_of_range{
        "ObjectHandle::ObjectHandle => No empty slot in handle table"};
  }

  T* t = new (MemoryManager::AllocDynamic(sizeof(T))) T{};
  entry->Set(uniqueID, static_cast<void*>(t), false);
}

template <typename T>
T* ObjectHandle<T>::operator->() const {
  return GetObjectPtr();
}

template <typename T>
T& ObjectHandle<T>::operator*() {
  return *GetObjectPtr();
}

template <typename T>
T* ObjectHandle<T>::GetObjectPtr() const {
  HandleEntry& entry = MemoryManager::handleEntryTable[index];

  if (entry.isEmpty) {
    throw std::exception{
        "ObjectHandle::GetObjectPtr => Object already deleted"};
  }

  // prevent the problem of "stale pointer"
  if (uniqueID == entry.uniqueID) {
    return static_cast<T*>(entry.ptr);
  }

  throw std::exception{
      "ObjectHandle::GetObjectPtr => Object you are trying to access was "
      "replaced by a new object"};
}

template <typename T>
void ObjectHandle<T>::EraseObject() const {
  HandleEntry& entry = MemoryManager::handleEntryTable[index];

  if (entry.isEmpty) {
    throw std::exception{Utilities::Msg(
        "ObjectHandle::EraseObject => ObjectHandle::DeleteObject => Double "
        "deleting handle!")};
  }

  if (uniqueID != entry.uniqueID) {
    throw std::exception{Utilities::Msg(
        "ObjectHandle::EraseObject => ObjectHandle::DeleteObject => You are "
        "trying to delete an "
        "object you don't own!")};
  }

  static_cast<T*>(entry.ptr)->~T();
  MemoryManager::FreeDynamic(entry.ptr);
  entry.isEmpty = true;
}

}  // namespace Isetta
