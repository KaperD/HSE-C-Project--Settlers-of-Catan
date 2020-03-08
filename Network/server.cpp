#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <exception>
#include <condition_variable>
#include <atomic>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "game.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using game::EventType;
using game::Void;
using game::OrderInfo;
using game::Event;
using game::Card;
using game::Dice;
using game::Market;
using game::Build;
using game::Player;
using game::Network;

class EventQueue final {
public:
    EventQueue() : hasElement(false) { }

    void push(Event event) {
        std::unique_lock<std::mutex> locker(lockqueue);
        events.push(std::move(event));
        hasElement.store(true);
        queuecheck.notify_all();
    }

    Event front() {
        std::unique_lock<std::mutex> locker(lockqueue);
        while (!hasElement.load()) {
            queuecheck.wait(locker);
        }
        if (events.size() > 0) {
            Event event = events.front();
            events.pop();
            if (events.size() == 0) {
                hasElement.store(false);
            }
            return event;
        } else {
            std::cerr << "front()" << std::endl;
            throw (std::logic_error("front()"));
        }
    }




private:
    std::queue<Event> events;
    std::mutex lockqueue;
    std::condition_variable queuecheck;
    std::atomic_bool hasElement;
};
 

class GameServerImpl final : public Network::Service {
public:
    GameServerImpl() : events_(4) { }
private:
    Status Register(::grpc::ServerContext* context, const Void* request,
                    OrderInfo* response) override {
        response->set_id(1);
        response->set_numberofplayers(3);
        return Status::OK;
    }

    Status SendEvent(::grpc::ServerContext* context, const Event* request, Void* response) override {
        Event event = *request;
        if (event.has_player()) {
            int playerid = event.mutable_player()->playerid();
            for (int k = 0; k < 3; ++k) {
                if (k == playerid) continue;
                events_[k].push(event);
            }
        } else {
            std::cerr << "No playerid" << std::endl;
        }

        return Status::OK;
    }

    Status GetEvent(ServerContext* context, const Player* request, Event* response) override { 
        int playerid = request->playerid();
        Event event = events_[playerid].front();
        *response = std::move(event);
        return Status::OK;
    }
  
private:
    std::vector<EventQueue> events_;
};

 
void RunServer() {
  std::string server_address("68.183.30.230:50051");
  GameServerImpl service;

  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();
}

int main() {
    RunServer();
    
     
    return 0;
}