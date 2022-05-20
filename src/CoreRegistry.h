/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "HandlerModulePlugin.h"
#include "InputModulePlugin.h"
#include <map>
#include <yaml-cpp/yaml.h>

namespace visor {

class InputStreamManager;
class HandlerManager;
class LabelManager;
class TapManager;
class PolicyManager;

/**
 * The "registry" of core data structures such as plugins, modules, taps and policies
 */
class CoreRegistry
{
public:
    typedef std::map<std::string, std::unique_ptr<InputModulePlugin>> InputPluginMap;
    typedef std::map<std::string, std::unique_ptr<HandlerModulePlugin>> HandlerPluginMap;

private:
    // this is the interface to load/instantiate/unload Corrade plugins (Corrade::PluginManager::Manager)
    InputPluginRegistry _input_registry;
    HandlerPluginRegistry _handler_registry;

    // these hold instantiated Corrade plugin instances (Corrade::PluginManager::AbstractPlugin->visor::AbstractPlugin instances)
    // they know how to instantiate visor::AbstractModule derived instances (stored in managers below) via HTTP admin API (through setup_routes) or Tap instantiation
    // *only one* per plugin type exists at a time, but they can instantiate many visor::AbstractModules (see managers below)
    // keyed by plugin alias name
    InputPluginMap _input_plugins;
    HandlerPluginMap _handler_plugins;

    // these hold instances of active visor::AbstractModule derived modules (the main event processors) which are created from the plugins above
    // any number can exist per plugin type at a time, each with their own life cycle
    std::unique_ptr<InputStreamManager> _input_manager;
    std::unique_ptr<HandlerManager> _handler_manager;

    // labels, taps and policies
    std::unique_ptr<LabelManager> _label_manager;
    std::unique_ptr<TapManager> _tap_manager;
    std::unique_ptr<PolicyManager> _policy_manager;

    std::shared_ptr<spdlog::logger> _logger;

public:
    CoreRegistry();
    ~CoreRegistry();

    void start(HttpServer *svr);
    void stop();

    // yaml based configuration
    void configure_from_file(const std::string &filename);
    void configure_from_str(const std::string &str);
    void configure_from_yaml(YAML::Node &node);

    [[nodiscard]] const InputPluginMap &input_plugins() const
    {
        return _input_plugins;
    }
    [[nodiscard]] const HandlerPluginMap &handler_plugins() const
    {
        return _handler_plugins;
    }
    [[nodiscard]] const InputStreamManager *input_manager() const
    {
        return _input_manager.get();
    }
    [[nodiscard]] const HandlerManager *handler_manager() const
    {
        return _handler_manager.get();
    }
    [[nodiscard]] const LabelManager *label_manager() const
    {
        return _label_manager.get();
    }
    [[nodiscard]] const TapManager *tap_manager() const
    {
        return _tap_manager.get();
    }
    [[nodiscard]] const PolicyManager *policy_manager() const
    {
        return _policy_manager.get();
    }
    [[nodiscard]] const HandlerPluginRegistry *handler_plugin_registry() const
    {
        return &_handler_registry;
    }
    [[nodiscard]] const InputPluginRegistry *input_plugin_registry() const
    {
        return &_input_registry;
    }
    [[nodiscard]] HandlerPluginRegistry *handler_plugin_registry()
    {
        return &_handler_registry;
    }
    [[nodiscard]] InputPluginRegistry *input_plugin_registry()
    {
        return &_input_registry;
    }

    [[nodiscard]] InputPluginMap &input_plugins()
    {
        return _input_plugins;
    }
    [[nodiscard]] HandlerPluginMap &handler_plugins()
    {
        return _handler_plugins;
    }

    [[nodiscard]] InputStreamManager *input_manager()
    {
        return _input_manager.get();
    }

    [[nodiscard]] HandlerManager *handler_manager()
    {
        return _handler_manager.get();
    }

    [[nodiscard]] LabelManager *label_manager()
    {
        return _label_manager.get();
    }

    [[nodiscard]] TapManager *tap_manager()
    {
        return _tap_manager.get();
    }

    [[nodiscard]] PolicyManager *policy_manager()
    {
        return _policy_manager.get();
    }
};

}