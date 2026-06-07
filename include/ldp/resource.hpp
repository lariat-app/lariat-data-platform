#ifndef LDP_RESOURCE_HPP
#define LDP_RESOURCE_HPP

#include <ldp/resource/blob_storage.hpp>
#include <ldp/resource/message_queue.hpp>

namespace ldp
{
    class resource_manager
    {
    public:
        virtual ~resource_manager() = default;
        virtual blob_storage *create_blob_storage(const std::string &name) = 0;
        virtual message_queue *create_message_queue(const std::string &name) = 0;
    };
}

#endif