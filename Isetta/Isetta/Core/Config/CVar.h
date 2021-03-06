/*
 * Copyright (c) 2018 Isetta
 */
#pragma once

#include <sstream>
#include <string>
#include "Core/Config/ICVar.h"
#include "Core/Math/Vector3.h"
#include "SID/sid.h"

namespace Isetta {
/**
 * @brief Console Variable to be used by Config and console
 *
 * @tparam T Template type must be have ostream operator>> and empty constructor
 */
template <typename T>
class CVar : public ICVar {
 public:
  /**
   * @brief Construct a new CVar object
   *
   * @param name of the CVar to put into the map, referenced in configuration
   * and console
   * @param defaultValue the value set by default (if not found in the config
   * file)
   */
  CVar(const std::string& name, const T& defaultValue)
      : ICVar(name), value{defaultValue} {
    CVarRegistry::RegisterVariable(this);
  }
  /**
   * @brief Construct a new CVar object, default value set by default
   * constructor of templated parameter
   *
   * @param name of the CVar to put into the map, referenced in configuration
   * and console
   */
  explicit CVar(const std::string& name) : ICVar(name), value{T()} {
    CVarRegistry::RegisterVariable(this);
  }
  /**
   * @brief Set the Val object
   *
   * @param strVal string value to be converted into value of type T
   */
  inline void SetVal(const std::string& strVal) override {
    std::istringstream iss(strVal);
    iss >> value;
  }
  /**
   * @brief Get the Val object
   *
   * @return T the value set of the object
   */
  inline T GetVal() const { return value; }

 private:
  T value;
};

/**
 * @brief Console Variable of type string to be used by Config and console.
 * Can use CVar<T> however when reading the string from configuration, but with
 * templated parameter the spaces are removed
 *
 */
class CVarString : public ICVar {
 public:
  /**
   * @brief Construct a new CVarString object
   *
   * @param name of the CVar to put into the map, referenced in configuration
   * and console
   * @param defaultValue the value set by default (if not found in the config
   * file)
   */
  CVarString(const std::string& name, const std::string& defaultValue);
  /**
   * @brief Construct a new CVar object, default value set by default
   * constructor of string
   *
   * @param name of the CVar to put into the map, referenced in configuration
   * and console
   */
  explicit CVarString(const std::string& name);
  /**
   * @brief Set the Val object
   *
   * @param strVal the string to set the value as
   */
  inline void SetVal(const std::string& strVal) override { sVal = strVal; }
  /**
   * @brief Get the Val object
   *
   * @return std::string the value set of the object
   */
  inline std::string GetVal() const { return sVal; }

 private:
  std::string sVal;
};

class CVarVector3 : public ICVar {
 public:
  /**
   * @brief Construct a new CVarVector3 object
   *
   * @param name of the CVar to put into the map, referenced in configuration
   * and console
   * @param defaultValue the value set by default (if not found in the config
   * file)
   */
  CVarVector3(const std::string& name, const Math::Vector3& defaultValue);
  /**
   * @brief Construct a new CVar object, default value set by default
   * constructor of vector3
   *
   * @param name of the CVar to put into the map, referenced in configuration
   * and console
   */
  explicit CVarVector3(const std::string& name);
  /**
   * @brief Set the Val object
   *
   * @param strVal string value to be converted into value of Vector3
   */
  inline void SetVal(const std::string& strVal) override {
    v3Val = Math::Vector3::FromString(strVal);
  }
  /**
   * @brief Get the Val object
   *
   * @return Math::Vector3 the value set of the object
   */
  inline Math::Vector3 GetVal() const { return v3Val; }

 private:
  Math::Vector3 v3Val;
};
}  // namespace Isetta
