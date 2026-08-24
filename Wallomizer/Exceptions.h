#pragma once

#include <exception>

class StartupCancelled final : public std::exception
{
};
