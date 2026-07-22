#include <ldp_service.hpp>

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/listener.h>

#include <yyjson.h>

#include <sstream>

static bool readMessage(evhttp_request *req, std::string &message)
{
    evbuffer *inputBuffer = evhttp_request_get_input_buffer(req);
    if (inputBuffer)
    {
        size_t length = evbuffer_get_length(inputBuffer);
        message.resize(length);
        evbuffer_copyout(inputBuffer, message.data(), length);
        return true;
    }
    return false;
}

static bool writeMessage(evhttp_request *req, const std::string &message)
{
    evbuffer *buf = evbuffer_new();
    if (buf)
    {
        evbuffer_add_printf(buf, "%s", message.c_str());
        evhttp_send_reply(req, HTTP_OK, "OK", buf);
        evbuffer_free(buf);
        return true;
    }
    return false;
}

class MyExampleService : public ldp::Service
{
public:
    bool connect(LdpConnector &connector, LdpLogger &logger) override
    {
        bool result(false);

        _eventBase.reset(event_base_new());
        _httpServer.reset(evhttp_new(_eventBase.get()));

        if (_httpServer && (evhttp_bind_socket(_httpServer.get(), "0.0.0.0", 8080) == 0))
        {
            evhttp_set_cb(_httpServer.get(), "/messages", requestCallback, this);
            _fifo = connector.acquireFifo("myfifo_resource");
            result = (_fifo != nullptr);
        }

        return result;
    }

    int launch() override
    {
        event_base_dispatch(_eventBase.get());
        return 0;
    }

    void shutdown() override
    {
        event_base_loopbreak(_eventBase.get());
    }

private:
    static void requestCallback(evhttp_request *req, void *arg)
    {
        switch (evhttp_request_get_command(req))
        {
        case EVHTTP_REQ_GET:
            static_cast<MyExampleService *>(arg)->handleGet(req);
            break;
        case EVHTTP_REQ_POST:
            static_cast<MyExampleService *>(arg)->handlePost(req);
            break;
        default:
            evhttp_send_error(req, HTTP_BADMETHOD, "Unsupported request method");
            break;
        }
    }

    void handleGet(evhttp_request *req)
    {
        
    }

    void handlePost(evhttp_request *req)
    {
        
    }

    LdpFifo::Ptr _fifo;
    std::unique_ptr<event_base, decltype(&event_base_free)> _eventBase{nullptr, &event_base_free};
    std::unique_ptr<evhttp, decltype(&evhttp_free)> _httpServer{nullptr, &evhttp_free};
};

LDP_EXPORT_SERVICE(MyExampleService)