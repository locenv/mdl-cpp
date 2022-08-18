#include <locenv/context.hpp>

using std::filesystem::path;

namespace locenv {
	context::context(const bootstrap_context &bc) :
		module_name(bc.name),
		working_directory(bc.working_directory, path::format::native_format)
	{
	}

	context::~context()
	{
	}
}
