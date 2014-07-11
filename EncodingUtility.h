#pragma once

#include <string>

std::string GBKToUTF8(const std::string& src);
std::string UTF8ToGBK(const std::string& src);

std::string hash_sha1(const std::string& src);
