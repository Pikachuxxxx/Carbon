#include "CarbonException.h"
#include <sstream>

CarbonException::CarbonException(int line, const char* file)
	: m_Line(line), m_File(file)
{}

const char* CarbonException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* CarbonException::GetType() const noexcept
{
	return "Exception";
}

int CarbonException::GetLine() const noexcept
{
	return m_Line;
}

const std::string& CarbonException::GetFile() const noexcept
{
	return m_File;
}

std::string CarbonException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_File << std::endl
		<< "[Line] " << m_Line;
	return oss.str();
}
