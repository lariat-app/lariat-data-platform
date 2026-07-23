#include <ldp_plugin.hpp>

#include <mutex>
#include <queue>
#include <string>

class MyFifo : public ldp::Fifo
{
private:
    class Source : public LdpFifoSource
    {
    private:
        MyFifo *_fifo{nullptr};
    public:
        Source(MyFifo *fifo) : _fifo(fifo) {}
        bool read(std::ostream& output) override
        {
            return _fifo->drain(output);
        }
    };

    class Sink : public LdpFifoSink
    {
    private:
        MyFifo *_fifo{nullptr};
    public:
        Sink(MyFifo *fifo) : _fifo(fifo) {}
        bool write(std::istream& input) override
        {
            return _fifo->fill(input);
        }
    };

    bool drain(std::ostream& output)
    {
        _logger->debug("Draining FIFO to output stream");
        bool result(false);
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (!_queue.empty())
            {
                const std::string &message = _queue.front();
                _logger->trace("Draining message: %s", message.c_str());
                output << message;
                _queue.pop();
                result = true;
            }
            else
            {
                _logger->error("Queue is empty, cannot read message");
            }
        }
        return result;
    }

    bool fill(std::istream& input)
    {
        _logger->debug("Filling FIFO with new message");
        bool result(false);
        input.seekg(0, std::ios::end);
        size_t size = input.tellg();
        if (size <= _maxSize)
        {
            input.seekg(0, std::ios::beg);
            std::string message(size, '\0');
            input.read(&message[0], size);
            if (input)
            {
                _logger->trace("New message read from input stream: %s", message.c_str());
                std::lock_guard<std::mutex> lock(_mutex);
                if (_queue.size() < _maxCount)
                {
                    _queue.push(message);
                    result = true;
                }
                else
                {
                    _logger->error("Queue is full, cannot accept new message");
                }
            }
        }
        else
        {
            _logger->error("Message size exceeds maximum allowed size");
        }
        return result;
    }
    
    LdpLogger *_logger{nullptr}; 
    size_t _maxCount;
    size_t _maxSize;
    std::mutex _mutex;
    std::queue<std::string> _queue;
public:
    bool configure(const LdpConfig &config, LdpLogger &logger) override
    {
        bool result(false);
        LdpConfig::Object configObject;
        if (config.getObject(configObject))
        {
            int maxSize, maxCount;
            LdpConfig::ObjectReader reader(configObject);
            if (reader.getInt("maxSize", maxSize))
            {
                logger.debug("Configured maxSize: %d", maxSize);
                _maxSize = static_cast<size_t>(maxSize);
                if (reader.getInt("maxCount", maxCount))
                {
                    logger.debug("Configured maxCount: %d", maxCount);
                    _maxCount = static_cast<size_t>(maxCount);
                    _logger = &logger;
                    result = true;
                }
                else
                {
                    logger.error("Missing 'maxCount' configuration");
                }
            }
            else
            {
                logger.error("Missing 'maxSize' configuration");
            }
        }
        else
        {
            logger.error("Configuration is not an object");
        }
        return result;
    }

    LdpFifoSource* createSource() override
    {
        return new Source(this);
    }

    LdpFifoSink* createSink() override
    {
        return new Sink(this);
    }

};

LDP_EXPORT_FIFO(MyFifo)