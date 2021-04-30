#include "log.h"
	Logger::Logger() : current_level(INFO) {}

	Logger::~Logger() {}

	std::string Logger::GetCurrentLvl() const {
		if (current_level == INFO)
			return "INFO";
		else if (current_level == DEBUG)
			return "DEBUG";
		else if (current_level == WARNING)
			return "WARNING";
		else
			return "ERROR";
	}

	LogLevel Logger::GetLvl() const {
		return current_level;
	}

	void Logger::output() {
		std::cout << boost::posix_time::second_clock::local_time() << "; " << GetCurrentLvl() << "; " << "(" << std::this_thread::get_id() << "): ";
	}

	void Logger::ChangeLogLvl(const LogLevel& new_lvl) {
		current_level = new_lvl;
	}
	ConsoleLogger::ConsoleLogger() : Logger() {}

	std::ostream& ConsoleLogger::operator()(const LogLevel& new_level) {
		ChangeLogLvl(new_level);
		output();
		return std::cout;
	}

	void FileLogger::output() {
		if (!output_file.is_open()) {
			output_file.open(name_of_file, std::ios_base::app);
		}
		output_file << "\n"<< boost::posix_time::second_clock::local_time() << "; " << GetCurrentLvl() << "; " << prefix << "(" << std::this_thread::get_id() << "): ";
	}
		 
	FileLogger::FileLogger(const std::string& new_name, const std::string& new_prefix) : name_of_file(new_name), prefix(new_prefix), Logger() {}

	FileLogger::~FileLogger() {
		output_file.close();
	}

	std::ofstream& FileLogger::operator()(const LogLevel& new_level) {
		ChangeLogLvl(new_level);
		mtx.lock();
		output();
		mtx.unlock();
		return output_file;
	}

	FileLogger::FileLogger(const FileLogger& fl) : name_of_file(fl.name_of_file), prefix(fl.prefix), Logger() {}

ConsoleLogger getLogger() {
	ConsoleLogger l;
	return l;
}

FileLogger getLogger(const std::string& filename,const std::string& prefix) {
	FileLogger fl(filename, prefix);
	return fl;
}
