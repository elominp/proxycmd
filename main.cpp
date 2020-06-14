#include <process.h>
#include <Windows.h>
#include <string>
#include <string.h>
#include <iostream>

constexpr size_t max_environment_value_length = 32768;
constexpr size_t max_filename_length = 32768;

inline const char * getExecutableName(const char * str) {
	int executable_name_start;
	
	for (executable_name_start = strlen(str); executable_name_start >= 0 && str[executable_name_start] != '\\'; executable_name_start--);
	return str + executable_name_start + 1;
}

inline const char * getExecutableFolderPath() {
	static char filepath[max_filename_length + 1] = { '\0' };

	if (filepath[0] == '\0')
	{
		GetModuleFileNameA(nullptr, filepath, max_filename_length);
		filepath[max_filename_length] = '\0';
		*(const_cast<char *>(getExecutableName(filepath))) = '\0';
	}
	return filepath;
}

int main(int argc, char * argv[]) {
	char current_path[max_environment_value_length + 1];
	const char ** args = reinterpret_cast<const char **>(_alloca(sizeof(char *) * (argc + 1)));
	const char * bin_name = getExecutableName(argv[0]);
	std::string new_folder_path = getExecutableFolderPath();
	std::string bin_path = "demo\\bin";
	std::string new_command = bin_path + "\\" + bin_name;
	std::string new_path;
	
	new_path.reserve(max_environment_value_length + 1);
	GetEnvironmentVariableA("Path", current_path, max_environment_value_length);
	current_path[max_environment_value_length] = '\0';
	new_folder_path += bin_path;
	new_path = new_folder_path + ";" + current_path;
	SetEnvironmentVariableA("Path", new_path.c_str());
	args[0] = new_command.c_str();
	for (int i = 1; i < argc; i++)
	{
		args[i] = argv[i];
	}
	args[argc] = nullptr;
	_execvp(args[0], args);
	abort();
	return 1;
}