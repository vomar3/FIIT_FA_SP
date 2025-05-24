//
// Created by Des Caldnd on 3/27/2024.
//

#include "server.h"
#include <logger_builder.h>
#include <fstream>
#include <iostream>


server::server(uint16_t port)
{
    CROW_ROUTE(app, "/init")([&](const crow::request &req) {
        std::string pid_str = req.url_params.get("pid");
        std::string sev_str = req.url_params.get("sev");
        std::string path_str = req.url_params.get("path");
        std::string console_str = req.url_params.get("console");
        std::cout << "INIT PID: " << pid_str << " SEVERITY: " << sev_str << " PATH: " << path_str << " CONSOLE: " << console_str << std::endl;

        int pid = stoi(pid_str);
        logger::severity sev = logger_builder::string_to_severity(sev_str);

        bool console;
        if (console_str == "1") {
          console = true;
        } else {
          console = false;
        }

        std::lock_guard lock(_mut);
        // Ищем по айдишнику процесса
        auto id = _streams.find(pid);

        if (id == _streams.end()) {
            // Не нашли и вставили в мапу
            id = _streams.emplace(pid, std::unordered_map<logger::severity, std::pair<std::string, bool>>()).first;
        }

        auto inner_id = id->second.find(sev);

        if (inner_id == id->second.end()) {
            // Не нашли и создали внутренний словарь для северити с путями
            inner_id = id->second.emplace(sev, std::make_pair(std::string(), false)).first;
        }

        inner_id->second.first = std::move(path_str);
        inner_id->second.second = console;

        return crow::response(200);
    });

    CROW_ROUTE(app, "/destroy")([&](const crow::request &req) {
        std::string pid_str = req.url_params.get("pid");
        std::cout << "DESTROY PID: " << pid_str << std::endl;

        int pid = stoi(pid_str);

        std::lock_guard lock(_mut);
        _streams.erase(pid);

        return crow::response(200);
    });

    CROW_ROUTE(app, "/log")([&](const crow::request &req) {
        std::string pid_str = req.url_params.get("pid");
        std::string sev_str = req.url_params.get("sev");
        std::string message_str = req.url_params.get("message");
        std::cout << "LOG PID: " << pid_str << " SEVERITY: " << sev_str << " MESSAGE: " << message_str << std::endl;

        int pid = stoi(pid_str);
        logger::severity sev = logger_builder::string_to_severity(sev_str);

        std::shared_lock lock(_mut);

        auto id = _streams.find(pid);
        if (id != _streams.end()) {
            auto inner_id = id->second.find(sev);
            if (inner_id != id->second.end()) {
                const std::string &path = inner_id->second.first;

                if (!path.empty()) {
                    std::ofstream stream(path, std::ios_base::app);
                    if (stream.is_open()) {
                        stream << message_str << std::endl;
                    }

                    if (inner_id->second.second) {
                        std::cout << message_str << std::endl;
                    }
                }
            }
        }

        return crow::response(200);

    });

    app.port(port).loglevel(crow::LogLevel::Warning).multithreaded();
    app.run();

}
