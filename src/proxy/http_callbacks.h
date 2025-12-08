#include <algorithm>

struct custom_data_t {
    bool done = false;

    std::unordered_map<std::string, std::string> headers;
    std::string next_header_value;

    std::string url;
};

int on_url_callback(http_parser *parser, const char *at, size_t length) {
    auto *parse_data = static_cast<custom_data_t *>(parser->data);
    parse_data->url = std::string(at, at + length);
    return 0;
}

int on_header_field(http_parser *parser, const char *at, size_t length) {
    auto *parse_data = static_cast<custom_data_t *>(parser->data);

    std::string field_name(at, at + length);

    std::transform(field_name.begin(), field_name.end(), field_name.begin(), ::tolower);

    parse_data->next_header_value = field_name;
    return 0;
}

int on_header_value(http_parser *parser, const char *at, size_t length) {
    auto *parse_data = static_cast<custom_data_t *>(parser->data);

    parse_data->headers[parse_data->next_header_value] = std::string(at, at + length);

    return 0;
}

int on_headers_complete(http_parser *parser) {
    auto *parse_data = static_cast<custom_data_t *>(parser->data);

    parse_data->done = true;

    return 0;
}

int on_message_complete(http_parser *parser) {
    auto *parse_data = static_cast<custom_data_t *>(parser->data);

    parse_data->done = true;

    return 0;
}
