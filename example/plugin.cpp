#include <ldp_plugin.hpp>

#include <mutex>
#include <queue>
#include <string>

enum class FifoResult
{
    Success,
    ErrorSize,
    ErrorFull,
    ErrorEmpty
};

class MyFifo
{
public:
    MyFifo() = default;
    ~MyFifo() = default;

    void setup(size_t maxCount, size_t maxSize)
    {
        _maxCount = maxCount;
        _maxSize = maxSize;
    }

    FifoResult drain(std::ostream &output)
    {
        FifoResult result(FifoResult::Success);
        std::string data;
        
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (!_queue.empty())
            {
                data = std::move(_queue.front());
                _queue.pop();
            }
            else
            {
                result = FifoResult::ErrorEmpty;
            }
        }

        if (!data.empty())
        {
            output.write(data.data(), data.size());
        }
        
        return result;
    }

    FifoResult fill(std::istream &input)
    {
        FifoResult result(FifoResult::Success);
        std::string data;
        data.resize(_maxSize);

        input.seekg(0, std::ios::end);
        size_t size = input.tellg();
        if (size <= _maxSize)
        {
            input.seekg(0, std::ios::beg);
            data.resize(size);
            input.read(&data[0], size);
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.size() < _maxCount)
            {
                _queue.push(data);
            }
            else
            {
                result = FifoResult::ErrorFull;
            }
        }
        else
        {
            result = FifoResult::ErrorSize;
        }

        return result;
    }

private:
    size_t _maxCount;
    size_t _maxSize;
    std::mutex _mutex;
    std::queue<std::string> _queue;
};

class MyFifoSource : public LdpFifoSource
{
public:
    MyFifoSource(MyFifo *fifo) : _fifo(fifo) {}
    bool read(std::ostream &output) override
    {
        bool result(false);
        switch (_fifo->drain(output))
        {
        case FifoResult::Success:
            result = true;
            break;
        case FifoResult::ErrorEmpty:
            // Handle empty case
            break;
        };

        return result;
    }

private:
    MyFifo *_fifo;
};

class MyFifoSink : public LdpFifoSink
{
public:
    MyFifoSink(MyFifo *fifo) : _fifo(fifo) {}
    bool write(std::istream &input) override
    {
        bool result(false);
        switch (_fifo->fill(input))
        {
        case FifoResult::Success:
            result = true;
            break;
        case FifoResult::ErrorFull:
            // Handle full case
            break;
        case FifoResult::ErrorSize:
            // Handle size error case
            break;
        };
        return result;
    }

private:
    MyFifo *_fifo;
};

class MyFifoProvider : public LdpFifoProvider
{
public:
    bool configure(LdpConfig *config) override
    {
        // Configure the FIFO based on the provided configuration
        return true;
    }

    LdpFifoSource *createSourcePtr() override
    {
        return new MyFifoSource(&_fifo);
    }

    LdpFifoSink *createSinkPtr() override
    {
        return new MyFifoSink(&_fifo);
    }

private:
    MyFifo _fifo;
};

LDP_FIFO_PLUGIN(MyFifoProvider)