/*
 *  mafResourcesSingletons.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesSingletons.h"


using namespace mafResources;

void mafResourcesSingletons::mafSingletonsInitialize() {
    // Register all classes instantiable in mafResources module.
    mafResourcesRegistration::registerResourcesObjects();
    mafPluginManager::instance();
    mafViewManager::instance();
    mafVMEManager::instance();
    mafOperationManager::instance();
    mafInteractionManager::instance();
}

void mafResourcesSingletons::mafSingletonsShutdown() {
    // Un-register objects from the factory.
    mafViewManager::instance()->shutdown();
    mafVMEManager::instance()->shutdown();
    mafOperationManager::instance()->shutdown();
    mafInteractionManager::instance()->shutdown();
    mafPluginManager::instance()->shutdown();
    mafResourcesRegistration::unregisterResourcesObjects();
}

void mafResources::initializeModule() {
    mafResources::mafResourcesSingletons::mafSingletonsInitialize();
}

void mafResources::shutdownModule() {
    mafResources::mafResourcesSingletons::mafSingletonsShutdown();
}
