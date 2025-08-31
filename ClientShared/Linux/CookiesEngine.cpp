// credit to luna for linux!
#include "../CookiesEngine.h"
#include "../format_string.h"
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <locale>
#include <codecvt>

// uh it do something?
static std::wstring convert_s2w(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

static std::string convert_w2s(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

// aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
static std::string convert_w2a(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

class CookiesEngine {
public:
    CookiesEngine(std::wstring path);
    std::wstring getCookiesFilePath();
    void setCookiesFilePath(std::wstring &path);

    int SetValue(std::string key, std::string value);
    std::string GetValue(std::string key, int *errorCode = nullptr, bool *exists = nullptr);
    int DeleteValue(std::string key);

private:
    void ParseFileContent(std::fstream &f);
    void FlushContent(std::fstream &f);

    std::string filePath;  // utf
    std::unordered_map<std::string, std::string> cookies;
};

CookiesEngine::CookiesEngine(std::wstring path) {
    filePath = convert_w2s(path);
    std::fstream f(filePath, std::ios::in);
    if (f.is_open()) {
        ParseFileContent(f);
        f.close();
    }
}


std::wstring CookiesEngine::getCookiesFilePath() {
    return convert_s2w(filePath);
}

void CookiesEngine::setCookiesFilePath(std::wstring &path) {
    filePath = convert_w2s(path);
    // uhm!
    cookies.clear();
    std::fstream f(filePath, std::ios::in);
    if (f.is_open()) {
        ParseFileContent(f);
        f.close();
    }
}

void CookiesEngine::ParseFileContent(std::fstream &f) {
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue; // prod?
        size_t pos = line.find('=');
        if (pos == std::string::npos) continue; // prod
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        cookies[key] = value;
    }
}

void CookiesEngine::FlushContent(std::fstream &f) {
    for (const auto& kv : cookies) {
        f << kv.first << "=" << kv.second << "\n";
    }
}

int CookiesEngine::SetValue(std::string key, std::string value) {
    cookies[key] = value;
    std::fstream f(filePath, std::ios::out | std::ios::trunc);
    if (!f.is_open()) return -1;
    FlushContent(f);
    f.close();
    return 0;
}

std::string CookiesEngine::GetValue(std::string key, int *errorCode, bool *exists) {
    auto it = cookies.find(key);
    if (it == cookies.end()) {
        if (exists) *exists = false;
        // e-girls Not found
        return "";
    }
    if (exists) *exists = true;
    if (errorCode) *errorCode = 0; // wowie it work?
    return it->second;
}

int CookiesEngine::DeleteValue(std::string key) {
    auto it = cookies.find(key);
    if (it == cookies.end()) return -1;
    cookies.erase(it);
    std::fstream f(filePath, std::ios::out | std::ios::trunc);
    if (!f.is_open()) return -1;
    FlushContent(f);
    // prod!
    f.close();
    std::cont << "Cookies!" << std::endl;

    return 0;
}
