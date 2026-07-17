#include <ldp_service.hpp>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/listener.h>

#include <yyjson.h>

#include <sstream>

static void encodeMessage(evhttp_request *req, std::ostringstream &stream)
{
    
}

static bool decodeMessage(evhttp_request *req, std::istringstream &stream);

class MyExampleService : public LdpService
{
public:
    MyExampleService() = default;
    ~MyExampleService() = default;

    bool onInitialize(LdpConnector &connector) override
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

    bool onDispatch() override
    {
        event_base_dispatch(_eventBase.get());
        return true;
    }

    void onShutdown() override
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
        std::ostringstream output;
        LdpFifoSource::Ptr source;

        source = _fifo->acquireSource();
        if (source->read(output))
        {
            encodeMessage(req, output);
        }
        else
        {
            evhttp_send_error(req, HTTP_INTERNAL, "Failed to read from FIFO");
        }
    }

    void handlePost(evhttp_request *req)
    {
        std::istringstream input;
        LdpFifoSink::Ptr sink;

        if (decodeMessage(req, input))
        {
            sink = _fifo->acquireSink();
            if (sink->write(input))
            {
                evhttp_send_reply(req, HTTP_OK, "OK", nullptr);
            }
            else
            {
                evhttp_send_error(req, HTTP_INTERNAL, "Failed to write to FIFO");
            }
        }
        else
        {
            evhttp_send_error(req, HTTP_BADREQUEST, "Invalid request body");
        }
    }

    LdpFifo::Ptr _fifo;
    std::unique_ptr<event_base, decltype(&event_base_free)> _eventBase{nullptr, &event_base_free};
    std::unique_ptr<evhttp, decltype(&evhttp_free)> _httpServer{nullptr, &evhttp_free};
};

LDP_SERVICE(MyExampleService)