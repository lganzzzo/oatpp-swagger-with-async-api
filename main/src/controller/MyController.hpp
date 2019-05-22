//
//  MyController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyController_hpp
#define MyController_hpp

#include "dto/MyDTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class MyController : public oatpp::web::server::api::ApiController {
protected:
  MyController(const std::shared_ptr<ObjectMapper>& objectMapper)
  : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<MyController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                    objectMapper)){
    return std::shared_ptr<MyController>(new MyController(objectMapper));
  }
  
  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)
  
  /**
   *  Hello World endpoint Coroutine mapped to the "/" (root)
   */
  ENDPOINT_INFO(Root) {
    info->summary = "Example 'Root' endpoint. Without any params";
    info->addResponse<HelloDto::ObjectWrapper>(Status::CODE_200, "application/json");
  }
  ENDPOINT_ASYNC("GET", "/", Root) {
    
    ENDPOINT_ASYNC_INIT(Root)
    
    /**
     *  Coroutine entrypoint act()
     *  returns Action (what to do next)
     */
    Action act() override {
      auto dto = HelloDto::createShared();
      dto->message = "Hello Async!";
      dto->server = Header::Value::SERVER;
      dto->userAgent = request->getHeader(Header::USER_AGENT);
      return _return(controller->createDtoResponse(Status::CODE_200, dto));
    }
    
  };
  
  /**
   *  Echo body endpoint Coroutine. Mapped to "/body/string".
   *  Returns body received in the request
   */
  ENDPOINT_INFO(EchoStringBody) {
    info->summary = "Echo endpoint. Echo body content";
    info->addConsumes<String>("text/plain");
    info->addResponse<String>(Status::CODE_200, "text/plain");
  }
  ENDPOINT_ASYNC("POST", "/body/string", EchoStringBody) {
    
    ENDPOINT_ASYNC_INIT(EchoStringBody)
    
    Action act() override {
      /* return Action to start child coroutine to read body */
      return request->readBodyToStringAsync().callbackTo(&EchoStringBody::returnResponse);
    }
    
    Action returnResponse(const oatpp::String& body){
      /* return Action to return created OutgoingResponse */
      return _return(controller->createResponse(Status::CODE_200, body));
    }
    
  };
  
  /**
   *  Echo body endpoint Coroutine. Mapped to "/body/dto".
   *  Deserialize DTO reveived, and return same DTO
   *  Returns body as MessageDto received in the request
   */
  ENDPOINT_INFO(EchoDtoBody) {
    info->summary = "Echo endpoint. Echo body content Serialized/Deserialized as DTO";
    info->addConsumes<MessageDto::ObjectWrapper>("application/json");
    info->addResponse<MessageDto::ObjectWrapper>(Status::CODE_200, "application/json");
  }
  ENDPOINT_ASYNC("POST", "/body/dto", EchoDtoBody) {
    
    ENDPOINT_ASYNC_INIT(EchoDtoBody)
    
    Action act() override {
      return request->readBodyToDtoAsync<MessageDto>(controller->getDefaultObjectMapper()).callbackTo(&EchoDtoBody::returnResponse);
    }
    
    Action returnResponse(const MessageDto::ObjectWrapper& body){
      return _return(controller->createDtoResponse(Status::CODE_200, body));
    }
    
  };

  /**
   *  Echo body endpoint Coroutine. Mapped to "/body/string".
   *  Returns body received in the request
   */
  ENDPOINT_INFO(ConcatParams) {
    info->summary = "Example. Documenting path params for async APIs.";
    info->addResponse<String>(Status::CODE_200, "text/plain");
    info->pathParams.add<String>("param1").description = "just the first parameter";
    info->pathParams.add<String>("param2").description = "just the second parameter";
  }
  ENDPOINT_ASYNC("GET", "/params/{param1}/{param2}", ConcatParams) {

    ENDPOINT_ASYNC_INIT(ConcatParams)

    Action act() override {
      auto p1 = request->getPathVariable("param1");
      auto p2 = request->getPathVariable("param2");
      OATPP_ASSERT_HTTP(p1 && p2, Status::CODE_400, "param1 and param2 should not be null");
      return _return(controller->createResponse(Status::CODE_200, "param1 + param2 = '" + p1 + p2 + "'"));
    }

  };
  
  /**
   *  Finish ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_END(ApiController)
  
};

#endif /* MyController_hpp */
