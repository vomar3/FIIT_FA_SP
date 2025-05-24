#include <not_implemented.h>
#include "../include/server_logger_builder.h"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

logger_builder& server_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity) &
{
    // Ищем по severity + создание пары / закидывание файла в мапу
    auto check_stream = _output_streams.find(severity);
    if (check_stream == _output_streams.end()){
        check_stream = _output_streams.emplace(severity, std::make_pair(std::string(), false)).first;
    }

    check_stream->second.first = stream_file_path;

    return *this;
}

logger_builder& server_logger_builder::add_console_stream(
    logger::severity severity) &
{
    // Типа что и выше, но теперь буля в тру переводим для вывода
    auto check_stream = _output_streams.find(severity);
    if (check_stream == _output_streams.end()){
        check_stream = _output_streams.emplace(severity, std::make_pair(std::string(), false)).first;
    }

    check_stream->second.second = true;

    return *this;
}

logger_builder& server_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path) &
{
    std::ifstream stream(configuration_file_path);

    if (!stream.is_open()) {
        return *this;
    }

    json full_config = json::parse(stream);
    stream.close();

    // Ключа в виде пути нет = печаль
    if (!full_config.contains(configuration_path)) {
        return *this;
    }

    json js = full_config[configuration_path];

    // Обработка ключа format
    if (js.contains("format") && js["format"].is_string()) {
        // установили значение в format
        set_format(js["format"].get<std::string>());
    }

    // Обработка ключа streams
    if (js.contains("streams") && js["streams"].is_array()) {
        for (auto& stream_item : js["streams"]) {
            if (!stream_item.contains("type") || !stream_item["type"].is_string()) {
                continue;
            }

            std::string type = stream_item["type"].get<std::string>();

            // Если type не файл или нет ключа пути или severities / никакие поля не под нужным форматом, то увы
            // + тут запись именно для файла
            if (type == "file" && stream_item.contains("path") && stream_item["path"].is_string() &&
                stream_item.contains("severities") && stream_item["severities"].is_array()) {

                std::string path = stream_item["path"].get<std::string>();

                for (auto& sev : stream_item["severities"]) {
                    if (sev.is_string()) {
                        add_file_stream(path, string_to_severity(sev.get<std::string>()));
                    }
                }

                // Проверка для консоли и подготовка для вывода туада
            } else if (type == "console" && stream_item.contains("severities") && stream_item["severities"].is_array()) {
                for (auto& sev : stream_item["severities"]) {
                    if (sev.is_string()) {
                        add_console_stream(string_to_severity(sev.get<std::string>()));
                    }
                }
            }
        }
    }

    return *this;
}

logger_builder& server_logger_builder::clear() & {
    _output_streams.clear();
    _destination = "http://127.0.0.1:9200";
    return *this;
}

logger *server_logger_builder::build() const
{
    return new server_logger(_destination, _output_streams, _format);
}

logger_builder& server_logger_builder::set_destination(const std::string& dest) &
{
    _destination = dest;
    return *this;
}

logger_builder& server_logger_builder::set_format(const std::string &format) &
{
    _format = format;
    return *this;
}
