#pragma once

#include <string>
#include <string_view>
#include <vector>

class Args
{
  public:
    class FilterArg
    {
      public:
        FilterArg(std::string_view name, std::vector<std::string>&& parameters);

        const std::string& GetName() const;
        const std::vector<std::string>& GetParameters() const;

        void AddParameter(const std::string& param);

      private:
        std::string name_;
        std::vector<std::string> parameters_;
    };

  public:
    Args();

    Args(int argc, const char* argv[]);

    const std::string& GetInputFile() const;
    const std::string& GetOutputFile() const;
    const std::vector<FilterArg>& GetFilters() const;

  private:
    static bool IsValidFilterName(std::string_view name);

    std::string input_file_;

    std::string output_file_;

    std::vector<FilterArg> filters_;
};
