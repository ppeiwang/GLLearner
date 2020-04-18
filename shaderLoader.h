#include <fstream>
#include <string>

class ShaderLoader
{
public:
	explicit ShaderLoader(const std::string& file_name);

	std::string DumpShader() const;

private:
	std::string file_name_;
};