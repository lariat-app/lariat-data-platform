#include <ldp_service.hpp>

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/listener.h>

#include <yyjson.h>

#include <cstdlib>
#include <sstream>

static bool readMessage(evhttp_request *req, std::string &message)
{
    bool result(false);
    evbuffer *inputBuffer = evhttp_request_get_input_buffer(req);
    if (inputBuffer)
    {
        std::string buffer;
        size_t length = evbuffer_get_length(inputBuffer);
        message.resize(length);
        evbuffer_copyout(inputBuffer, message.data(), length);
        std::unique_ptr<yyjson_doc, decltype(&yyjson_doc_free)> jsonDoc(yyjson_read(message.data(), message.size(), 0), &yyjson_doc_free);
        if (jsonDoc)
        {
            yyjson_val *root = yyjson_doc_get_root(jsonDoc.get());
            if (root && yyjson_is_obj(root))
            {
                yyjson_val *messageVal = yyjson_obj_get(root, "message");
                if (messageVal && yyjson_is_str(messageVal))
                {
                    message = std::string(yyjson_get_str(messageVal), yyjson_get_len(messageVal));
                    result = true;
                }
            }
        }
    }
    return result;
}

static bool writeMessage(evhttp_request *req, const std::string &message)
{
    bool result(false);
    const char *messageBuffer = (message.empty() ? "" : message.c_str());
    std::unique_ptr<evbuffer, decltype(&evbuffer_free)> buf(evbuffer_new(), &evbuffer_free);
    std::unique_ptr<yyjson_mut_doc, decltype(&yyjson_mut_doc_free)> jsonDoc(yyjson_mut_doc_new(nullptr), &yyjson_mut_doc_free);
    if (buf && jsonDoc)
    {
        yyjson_mut_val *root = yyjson_mut_obj(jsonDoc.get());
        if (root && yyjson_mut_obj_add_strcpy(jsonDoc.get(), root, "message", messageBuffer))
        {
            yyjson_mut_doc_set_root(jsonDoc.get(), root);
            size_t jsonLen = 0;
            std::unique_ptr<char, decltype(&free)> jsonStr(yyjson_mut_write_opts(jsonDoc.get(), 0, nullptr, &jsonLen, nullptr), &free);
            if (jsonStr && jsonLen > 0)
            {
                evbuffer_add(buf.get(), jsonStr.get(), jsonLen);
                evhttp_send_reply(req, HTTP_OK, "OK", buf.get());
                result = true;
            }
        }
    }
    return result;
}

class MyExampleService : public ldp::Service
{
public:
    bool connect(LdpConnector &connector, LdpLogger &logger) override
    {
        bool result(false);

        _eventBase.reset(event_base_new());
        _httpServer.reset(evhttp_new(_eventBase.get()));

        if (_httpServer && (evhttp_bind_socket(_httpServer.get(), "0.0.0.0", 12345) == 0))
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
        LdpFifoSource::Ptr source = _fifo->acquireSource();
        std::ostringstream outputSink;
        if (source && source->read(outputSink))
        {
            if (!writeMessage(req, outputSink.str()))
            {
                evhttp_send_error(req, HTTP_INTERNAL, "Failed to encode message");
            }
        }
        else
        {
            evhttp_send_error(req, HTTP_INTERNAL, "Failed to read message");
        }
    }

    void handlePost(evhttp_request *req)
    {
        LdpFifoSink::Ptr sink = _fifo->acquireSink();
        std::string message;
        if (sink && readMessage(req, message))
        {
            std::istringstream inputSource(message);
            if (sink->write(inputSource))
            {
                evhttp_send_reply(req, HTTP_OK, "OK", nullptr);
            }
            else
            {
                evhttp_send_error(req, HTTP_INTERNAL, "Failed to write message");
            }
        }
        else
        {
            evhttp_send_error(req, HTTP_INTERNAL, "Failed to decode message");
        }
    }

    LdpFifo::Ptr _fifo;
    std::unique_ptr<event_base, decltype(&event_base_free)> _eventBase{nullptr, &event_base_free};
    std::unique_ptr<evhttp, decltype(&evhttp_free)> _httpServer{nullptr, &evhttp_free};
};

LDP_EXPORT_SERVICE(MyExampleService)