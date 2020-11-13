#include "RoastyServer.hpp"
#include "../Roasty.hpp"
#include "../Serialisation.hpp"
#include "../Storage/DiskStorage.hpp"
#include "../Storage/MemoryStorage.hpp"
#include "RoastyServerException.hpp"
#include "httplib.h"
#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using namespace httplib;
using json = nlohmann::json;

void handleRequestWithErrorHandling(Response& res, const std::function<void(void)>& handler) {
  try {
    handler();
  } catch(std::string& error) {
    res.status = Roasty<void>::errorCode;
    res.set_content(error, "text/plain");
  } catch(RoastyServerException& e) {
    res.status = e.status;
    res.set_content(e.message, "text/plain");
  }
}

template <typename RoastyImplementation> void RoastyServer<RoastyImplementation>::startServer() {

  // =============== Bean ==================

  // Get all beans
  srv.Get("/beans", [this](const Request& /*req*/, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto allBeans = requestHandler->allBeans();

      json beans;
      for(auto& bean : allBeans) {
        beans.push_back(bean.getName());
      }

      json j;
      j["beans"] = beans;

      res.set_content(j.dump(), "application/json");
    });
  });

  // Add a bean type
  // Body expects: {"beans": "newBeanNameToAdd"}
  srv.Post("/beans", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto j = json::parse(req.body);
      Bean b{j["beans"].get<std::string>()};

      requestHandler->addBean(b);
    });
  });

  // Delete a bean type
  // Restriction: No cascade to Roast on delete
  srv.Delete(R"(/beans/(.+))", [this](const Request& req, Response& res) {
    auto b = Bean{req.matches[1]};
    handleRequestWithErrorHandling(res, [&] { requestHandler->deleteBean(b); });
  });

  srv.Put(R"(/beans/(.+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto j = json::parse(req.body);
      auto b = Bean{req.matches[1]};
      auto newName = j["beans"].get<std::string>();

      requestHandler->renameBean(b, newName);
    });
  });

  // ================== Roasts ===============
  srv.Get("/roasts", [this](const Request& /*req*/, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto allRoasts = requestHandler->allRoasts();
      json j;

      for(auto& roast : allRoasts) {
        j.push_back(roastToJson(roast));
      }

      res.set_content(j.dump(), "application/json");
    });
  });

  srv.Post("/roasts", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto j = json::parse(req.body);
      auto roast = jsonToRoast(j);

      std::stringstream newPath{};
      newPath << "/roasts/" << roast.getId();
      requestHandler->addRoast(roast);
      res.set_header("Location", newPath.str().c_str());
      res.status = 201;
      res.set_content(newPath.str(), "application/json");
    });
  });

  srv.Put(R"(/roasts/(\d+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto id = std::stol(req.matches[1]);
      auto j = json::parse(req.body);
      auto roast = jsonToRoast(j);

      requestHandler->replaceRoast(id, roast);
    });
  });

  srv.Delete(R"(/roasts/(\d+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto id = std::stol(req.matches[1]);

      requestHandler->deleteRoast(id);
    });
  });

  srv.Get(R"(/roasts/(\d+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto id = std::stol(req.matches[1]);

      auto roast = roastToJson(requestHandler->getRoast(id));
      if(req.get_header_value("Accept").find("text/html") != std::string::npos) {
        static std::string indexFile;
        if(indexFile.empty()) {
          std::ifstream input("../www/addRoast.html");
          indexFile = {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
        }
        res.set_content(indexFile, "text/html");
      } else {
        res.set_content(roast.dump(), "application/json");
      }
    });
  });

  // ====================== Events ======================
  srv.Get(R"(/roasts/(\d+)/events/(\d+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto roastId = std::stol(req.matches[1]);
      auto eventId = std::stol(req.matches[2]);

      auto event = requestHandler->getEventById(roastId, eventId);
      auto json = eventToJson(event);
      res.set_content(json.dump(), "application/json");
    });
  });

  srv.Put(R"(/roasts/(\d+)/events/(\d+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto roastId = std::stol(req.matches[1]);
      auto eventId = std::stol(req.matches[2]);
      auto j = json::parse(req.body);

      requestHandler->replaceEventInRoast(roastId, eventId, *jsonToEvent(j));
    });
  });

  srv.Post(R"(/roasts/(\d+)/events)", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto id = std::stol(req.matches[1]);
      auto j = json::parse(req.body);

      requestHandler->addEventToRoast(id, *jsonToEvent(j));
    });
  });

  srv.Delete(R"(/roasts/(\d+)/events/(\d+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto roastId = std::stol(req.matches[1]);
      auto eventId = std::stol(req.matches[2]);

      requestHandler->removeEventFromRoast(roastId, eventId);
    });
  });

  // ====================== Blends ======================
  srv.Get(R"(/roasts/(\d+)/blends/(.+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto roastId = std::stol(req.matches[1]);
      auto beanName = req.matches[2];

      auto blend = requestHandler->getIngredientByBeanName(roastId, beanName);
      auto json = ingredientToJson(blend);
      res.set_content(json.dump(), "application/json");
    });
  });

  srv.Patch(R"(/roasts/(\d+)/blends/(.+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto roastId = std::stol(req.matches[1]);
      auto beanName = req.matches[2];
      auto j = json::parse(req.body);

      if(j["newAmount"].empty()) {
        throw RoastyServerException{"No field newAmount", Roasty<void>::errorCode};
      }

      auto newAmount = j["newAmount"].get<int>();

      requestHandler->updateIngredient(roastId, beanName, newAmount);
    });
  });

  srv.Post(R"(/roasts/(\d+)/blends)", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto id = std::stol(req.matches[1]);
      auto j = json::parse(req.body);

      requestHandler->addIngredientToRoast(id, *jsonToIngredient(j));
    });
  });

  srv.Delete(R"(/roasts/(\d+)/blends/(.+))", [this](const Request& req, Response& res) {
    handleRequestWithErrorHandling(res, [&] {
      auto roastId = std::stol(req.matches[1]);
      auto beanName = req.matches[2];

      requestHandler->removeIngredientFromRoast(roastId, beanName);
    });
  });

  srv.Get(R"(/)", [this](auto const& request, Response& response) {
    static std::string indexFile;
    if(indexFile.empty()) {
      std::ifstream input("../www/index.html");
      indexFile = {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
    }
    response.set_content(indexFile, "text/html");
  });

  srv.listen("localhost", getPort());
}

template class RoastyServer<Roasty<MemoryStorage>>;
template class RoastyServer<Roasty<DiskStorage>>;
