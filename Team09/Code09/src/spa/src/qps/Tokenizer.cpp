#include "Tokenizer.h"

std::vector<std::string> Tokenizer::tokenize(std::string inputQueryString)
{
	std::vector<std::string> inputQueryTokens = { };
	std::string delimiter = ";";

	std::size_t nextDelimiterIndex = inputQueryString.find(delimiter);
	std::size_t offset = 0;
	std::size_t substringSize;

	while (nextDelimiterIndex != std::string::npos)
	{
		substringSize = nextDelimiterIndex - offset + 1;
		inputQueryTokens.push_back(inputQueryString.substr(offset, substringSize));

		offset = nextDelimiterIndex + 1;
		nextDelimiterIndex = inputQueryString.find(delimiter, offset);
	}

	if (offset != inputQueryString.length())
	{
		substringSize = inputQueryString.length() - offset + 1;
		inputQueryTokens.push_back(inputQueryString.substr(offset, substringSize));
	}

	return inputQueryTokens;
}
