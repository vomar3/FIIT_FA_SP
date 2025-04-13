#include <filesystem>
#include <utility>
#include <not_implemented.h>
#include "../include/client_logger_builder.h"
#include <not_implemented.h>

using namespace nlohmann;

logger_builder& client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity) &
{
    auto check_stream = _output_streams.find(severity);
    if (check_stream == _output_streams.end()) {
        // iterator
        check_stream = _output_streams.emplace(severity, std::make_pair(std::forward_list<client_logger::refcounted_stream>(), false)).first;
    }

    // std::filesystem::weakly_canonical: абсолютный путь в относительный
    // делаем строчку и добавляем в начало списка
    check_stream->second.first.emplace_front(std::filesystem::weakly_canonical(stream_file_path).string());
    return *this;
}

logger_builder& client_logger_builder::add_console_stream(
    logger::severity severity) &
{
    auto check_stream = _output_streams.find(severity);
    // Не нашли = вставили, нашли = сделали флаг true
    if (check_stream == _output_streams.end()) {
        _output_streams.emplace(severity, std::make_pair(std::forward_list<client_logger::refcounted_stream>(), true));
    } else {
        check_stream->second.second = true;
    }

    return *this;
}

// 1 - json, 2 file
logger_builder& client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path) &
{
    std::ifstream file;
    file.open(configuration_file_path);

    if (!file.is_open()) {
        throw std::ios_base::failure("Can't open file " + configuration_file_path);
    }

    // Парсим
    json data = json::parse(file);
    file.close();

    // Ищем запись
    auto check_stream = data.find(configuration_path);
    // Запись не нашли или (json = словарь в словаре) не найден внутренний словарь
    if (check_stream == data.end() || !check_stream->is_object()) {
        return *this;
    }

    parse_severity(logger::severity::information, (*check_stream)["information"]);
    parse_severity(logger::severity::critical, (*check_stream)["critical"]);
    parse_severity(logger::severity::warning, (*check_stream)["warning"]);
    parse_severity(logger::severity::trace, (*check_stream)["trace"]);
    parse_severity(logger::severity::debug, (*check_stream)["debug"]);
    parse_severity(logger::severity::error, (*check_stream)["error"]);

    // Ищем поле format (%)
    auto format = check_stream->find("format");
    // Если найдено + это именно строка
    if (format != check_stream->end() && format->is_string()) {
        // Записываем данные в переменную
        _format = format.value();
    }

    return *this;
}

// Ну клир и клир
logger_builder& client_logger_builder::clear() &
{
    _output_streams.clear();
    _format = "%m";
    return *this;
}

// Создание нового объекта + вызов конструктора
logger *client_logger_builder::build() const
{
    return new client_logger(_output_streams, _format);
}

// Установка формата нашего лога
logger_builder& client_logger_builder::set_format(const std::string &format) &
{
    _format = format;
    return *this;
}

void client_logger_builder::parse_severity(logger::severity sev, nlohmann::json& j)
{
    if (j.empty() || !j.is_object()) {
        return;
    }

    // По хэш таблице ищем наш severity, чтобы был доступ к списку
    auto check_stream = _output_streams.find(sev);
    // Ищем пути + чекаем, что найдено, и пути записаны в json в виде массива
    auto data_paths = j.find("paths");
    if (data_paths != j.end() && data_paths->is_array()) {
        // Копия json
        json data = *data_paths;
        for (const json& js: data) {
            if (js.empty() || !js.is_string()) {
                continue;
            }

            // js-элемент -> строка
            const std::string& path = js;
            // Если еще нет записи - делаем
            if (check_stream == _output_streams.end()) {
                check_stream = _output_streams.emplace(sev, std::make_pair(std::forward_list<client_logger::refcounted_stream>(), false)).first;
            }

            // Делаем относительный путь
            check_stream->second.first.emplace_front(std::filesystem::weakly_canonical(path).string());
        }
    }

    auto console = j.find("console");
    if (console != j.end() && console->is_boolean()) {
        if (check_stream == _output_streams.end()) {
            check_stream = _output_streams.emplace(sev, std::make_pair(std::forward_list<client_logger::refcounted_stream>(), false)).first;
        }
        check_stream->second.second = console->get<bool>();
    }
}

logger_builder& client_logger_builder::set_destination(const std::string &format) &
{
    return *this;
}
