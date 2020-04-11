#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include <mutex>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include "game.grpc.pb.h"
#include "EventQueue.h"
#include "spinlock.h"

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

class GameServerImpl final : public Network::Service {
public:
    GameServerImpl() : events_(4), currentPlayerNumber_(0) { }

    bool RefreshGame() {
        std::lock_guard<utility::spinlock> guard(spin_);
        if (isRun_.load()) {
            Event endgame;
            endgame.set_type(EventType::ENDGAME);
            for (int k = 0; k < 3; ++k) {
                endgame.set_playerid(k + 1);
                events_[k].clear();
                events_[k].push(endgame);
            }
            isRun_.store(false);
            return true;
        }
        return false;
    }
private:
    Status Register(::grpc::ServerContext* context, const Void* request, OrderInfo* response) override {
        std::lock_guard<utility::spinlock> guard(spin_);
        if (!isRun_.load()) {
            for (int k = 0; k < 3; ++k) {
                events_[k].clear();
            }
            currentPlayerNumber_ = 0;
            // std::thread ref(refresher, this);
            // ref.detach();
        }
        isRun_.store(true);
        int id = currentPlayerNumber_++;
        if (id > 2) {
            id %= 3;
            currentPlayerNumber_ %= 3;
            events_[id].clear();
        }
        id %= 3;
        response->set_id(id);
        response->set_numberofplayers(3);

        return Status::OK;
    }

    Status SendEvent(::grpc::ServerContext* context, const Event* request, Void* response) override {
        Event event = *request;
        int playerid = event.playerid();
        
        for (int k = 0; k < 3; ++k) {
            if (k == playerid) continue;
            events_[k].push(event);
        }
        if (event.type() == EventType::ENDGAME) {
            events_[playerid].push(event);
            isRun_.store(false);
        }
        std::cout << playerid << ' ' << event.type() << std::endl;
        return Status::OK;
    }

    Status GetEvent(ServerContext* context, const Player* request, Event* response) override { 
        int playerid = request->playerid();
        Event event = events_[playerid].front();
        *response = std::move(event);

        return Status::OK;
    }

private:
    std::vector<utility::EventQueue> events_;
    int currentPlayerNumber_;
    std::atomic_bool isRun_ { false };
    utility::spinlock spin_;
};


void refresher(GameServerImpl* server) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        if (server->RefreshGame()) {
            return;
        }
    }
}

 
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