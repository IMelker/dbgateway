//
// Created by imelker on 10.01.2021.
//

#include "common/Config.h"

#include "Gateway.h"

#include "common/HttpUtils.h"
using HttpRequest = httplib::Request;
using HttpResponse =httplib::Response;


Gateway::Gateway(Config *config) : config(config) {
    // inits http server with common threads

    if (!server.is_valid()) {
        printf("server has an error...\n");
    }

    this->server.Post("/multipart", [](const HttpRequest &req, HttpResponse &res) {
        auto body = dump_headers(req.headers) + dump_multipart_files(req.files);

        res.set_content(body, "text/plain");
    });

    this->server.Get("/", [=](const HttpRequest & /*req*/, HttpResponse &res) {
        res.set_redirect("/hi");
    });

    this->server.Get("/hi", [](const HttpRequest & /*req*/, HttpResponse &res) {
        res.set_content("Hello World!\n", "text/plain");
    });

    this->server.Get("/slow", [](const HttpRequest & /*req*/, HttpResponse &res) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        res.set_content("Slow...\n", "text/plain");
    });

    this->server.Get("/dump", [](const HttpRequest &req, HttpResponse &res) {
        res.set_content(dump_headers(req.headers), "text/plain");
    });

    this->server.Get("/stop",
            [&](const HttpRequest & /*req*/, HttpResponse & /*res*/) { server.stop(); });

    this->server.set_error_handler([](const HttpRequest & /*req*/, HttpResponse &res) {
        const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
        char buf[BUFSIZ];
        snprintf(buf, sizeof(buf), fmt, res.status);
        res.set_content(buf, "text/html");
    });

    this->server.set_logger([](const HttpRequest &req, const HttpResponse &res) {
        printf("%s", log(req, res).c_str());
    });

    this->serverThread = std::thread{[&] {
        this->server.listen("localhost", 8080);
    }};
}

Gateway::~Gateway() {
    this->server.stop();
    this->serverThread.join();
};
