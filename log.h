#pragma once
#include <thread>
#include <iostream>
#include <fstream>
#include <string>
#include<thread>
#include<mutex>
#include"boost/date_time/posix_time/posix_time.hpp"

enum LogLevel {
	INFO,
	DEBUG,
	WARNING,
	ERROR
};
class Logger {
public:
	Logger();
	virtual ~Logger();
	std::string GetCurrentLvl() const;
	LogLevel GetLvl() const;
	virtual void output();
	void ChangeLogLvl(const LogLevel& new_lvl);
private:
	LogLevel current_level;
};
class ConsoleLogger : public Logger {
public:
	ConsoleLogger();
	template<typename T>
	void operator <<(const T& t) {
		output();
		std::cout << t << std::endl;
	}
	std::ostream& operator()(const LogLevel& new_level);
};
class FileLogger : public Logger {
public:
	void output() override;
	FileLogger(const std::string& new_name, const std::string& new_prefix);
	~FileLogger();
	template<typename T>
	void operator <<(const T& t) {
		mtx.lock();
		output();
		output_file << t;
		output_file.close();
		mtx.unlock();
	}
	std::ofstream& operator()(const LogLevel& new_level);
	FileLogger(const FileLogger& fl);
private:
	std::ofstream output_file;
	std::string name_of_file;
	std::string prefix;
	std::mutex mtx;
};
ConsoleLogger getLogger();
FileLogger getLogger(const std::string& filename, const std::string& prefix ="");
