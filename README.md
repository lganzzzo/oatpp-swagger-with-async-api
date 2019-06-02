# oatpp-swagger with Async APIs

Simple example how to document Async-Controller endpoints with Swagger-UI

#### More about oat++:
- Website: [https://oatpp.io](https://oatpp.io)
- Docs: [https://oatpp.io/docs/start](https://oatpp.io/docs/start)
- Oat++ Repo: [https://github.com/oatpp/oatpp](https://github.com/oatpp/oatpp)

## Overview

### Project layout

```
|- CMakeLists.txt                        // projects CMakeLists.txt
|- src/
|    |
|    |- controller/                      // Folder containing controller where all endpoints are declared
|    |- dto/                             // DTOs are declared here
|    |- SwaggerComponent.hpp             // General info for Swagger
|    |- AppComponent.hpp                 // Service config
|    |- Logger.hpp                       // Application Logger
|    |- App.cpp                          // main() is here
|
|- test/                                 // test folder
|- utility/install-oatpp-modules.sh      // utility script to install required oatpp-modules.
    
```

---

### Build and Run

#### Using CMake

**Requires** 

- `oatpp` and `oatpp-swagger` modules installed. You may run `utility/install-oatpp-modules.sh` 
script to install required oatpp modules.

```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./example-async-api-exe       # - run application.
```

#### In Docker

```
$ docker build -t example-async-api .
$ docker run -p 8000:8000 -t example-async-api
```


### Swagger-UI

Swagger-UI endpoint - [http://localhost:8000/swagger/ui](http://localhost:8000/swagger/ui).
