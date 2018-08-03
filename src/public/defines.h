#ifndef CZ_DEFINES_H
#define CZ_DEFINES_H

namespace cz_defines
{
    const unsigned receive_buffer_size = 1024;
    const unsigned frame_header_code = 0x7E;
    const unsigned frame_max_length = 40;
    const unsigned rt_message_pool_max_size = 30; // total frame max length is 1024, single segment max length is less than 40. So if the pool is 30(1024 / 40), it won't be problem for handler.
    const unsigned channel_count_per_device = 4;
    const unsigned sample_data_max_count = 1000;
    const unsigned DST_PHYSICAL_ADDR_MASK = 0x8000;
    const unsigned SRC_PHYSICAL_ADDR_MASK = 0x4000;
    const unsigned FW_MASK = 0x2000;
    const unsigned char ACK_MASK = 0x40;
    const unsigned MORE_FRAME_MASK = 0x80;
    const unsigned FRAME_ID_MASK = 0x3F;
	const unsigned JOIN_ED_NETWORK_ADDR = 0xFFFE;
	const unsigned JOIN_SITE_NETWORK_ADDR = 0x00FE;
	const unsigned FIND_FRAME_FIRST_LEN = 12;
	const unsigned MAX_FRAME_COUNT = 12;
	const unsigned SERVER_NETWORK_ADDR = 0x01;
	const unsigned SITE_NETWORK_ADDR_MIN   = 0x02;
	const unsigned DEVICE_NETWORK_ADDR_MIN   = 0x0100;
	const unsigned RETRY_TIMEOUT   = 0x05;
	const unsigned RETRY_TIMES   = 0x03;
	const unsigned R_AP_MAX_GROUP   = 0x32;
	const unsigned R_AP_MAX_GROUP_MEMBER_SIZE   = 0x05;
};


#define HANDLER_TABLE handlerTable
#define ENTRY_TYPE boost::function1<int, cz_types::Frame*>
#define FUN_ENTRY_TYPE boost::function1<int, cz_types::RequestMessage*>
#define DISPATCH_HANDLER(slot) HANDLER_TABLE[slot]
#define ENTRY_TYPE_BIND(entry) boost::bind(entry, this, _1)
#define CREATE_HANDLER_TABLE(handler_count) ENTRY_TYPE HANDLER_TABLE[handler_count]
#define CREATE_HANDLER_ENTRY(func_name) int func_name(cz_types::Frame *frame)
#define HANDLER_CONNECTION(slot, entry) HANDLER_TABLE[slot] = entry
#define SET_TOTAL_ENTRY_COUNT(slot) enum {totalEntrySize = slot}
#define TOTAL_ENTRY_COUNT totalEntrySize

#define CREATE_READONLY_PROPERTY(typename, property_name)           \
    protected:                                                        \
    typename property_name;                                         \
    public:                                                         \
    inline typename get_##property_name() {return property_name;}

#define CREATE_READONLY_REFERENCE_PROPERTY(typename, property_name) \
    protected:                                                        \
    typename property_name;                                         \
    public:                                                         \
    inline typename &get_##property_name() {return property_name;}

#define CREATE_PROPERTY(typename, property_name)                    \
    CREATE_READONLY_PROPERTY(typename, property_name)               \
    inline void set_##property_name(typename property) {property_name = property;}

#define CREATE_REFERENCE_PROPERTY(typename, property_name)                    \
    CREATE_READONLY_REFERENCE_PROPERTY(typename, property_name)               \
    inline void set_##property_name(typename &property) {property_name = property;}

#define ACCESS_GET_PROPERTY(property_name) get_##property_name()
#define ACCESS_SET_PROPERTY(property_name, value) set_##property_name(value)

#define PLUGIN_CREATE_HANDLER(type) static CZISuite *create(void *param = 0) { \
                            return new type(param); \
                        }
#define PLUGIN_CREATE_HANDLER_ALIAS(type)   extern "C" BOOST_SYMBOL_EXPORT CZISuite *create_plugin(void *param) \
                                            { \
                                                return type::create(param); \
                                            }
#define PLUGIN_NAME(value) static std::string plugin_name = value; \
                           extern "C" BOOST_SYMBOL_EXPORT std::string &get_plugin_name() {return plugin_name;}

#endif //#ifndef CZ_DEFINES_H
