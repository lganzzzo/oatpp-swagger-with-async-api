//
//  main.cpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#include "./controller/MyController.hpp"
#include "./SwaggerComponent.hpp"
#include "./AppComponent.hpp"
#include "./Logger.hpp"

#include "oatpp-swagger/AsyncController.hpp"

#include "oatpp/network/server/Server.hpp"

#include <iostream>

/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
void run() {

  AppComponent components; // Create scope Environment components
  SwaggerComponent swaggerComponent;
  
  /* create ApiControllers and add endpoints to router */

  auto router = components.httpRouter.getObject();
  auto docEndpoints = oatpp::swagger::AsyncController::Endpoints::createShared();

  auto myController = MyController::createShared();
  myController->addEndpointsToRouter(router);

  docEndpoints->pushBackAll(myController->getEndpoints());

  auto swaggerController = oatpp::swagger::AsyncController::createShared(docEndpoints);
  swaggerController->addEndpointsToRouter(router);

  /* create server */
  
  oatpp::network::server::Server server(components.serverConnectionProvider.getObject(),
                                        components.serverConnectionHandler.getObject());
  
  OATPP_LOGD("Server", "Running on port %s...", components.serverConnectionProvider.getObject()->getProperty("port").toString()->c_str());
  
  server.run();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {
  
  oatpp::base::Environment::setLogger(new Logger());
  oatpp::base::Environment::init();

  run();
  
  oatpp::base::Environment::setLogger(nullptr); ///< free Logger
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
  
  return 0;
}
