#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <map>

void	fatal_error(const char* err_msg)
{
	std::cerr << err_msg << std::endl;
	exit(EXIT_FAILURE);
}

// for debugging only
void	print_letter_count(const std::vector<int>& letter_count)
{
	char	letter = 'a';

	while (letter <= 'z')
	{
		std::cout << letter << ": " << letter_count[letter - 'a'] << std::endl;
		letter++;
	}
}

// converts the second byte of a 0xc3 utf character to its ascii equivalent
inline char	utf_to_ascii(unsigned char c)
{
	static const char	utf_table[65] = "aaaaaa.ceeeeiiiidnooooo.ouuuuy..aaaaaa.ceeeeiiii.nooooo.ouuuuy.y";
	return (utf_table[c - 0x80]);
}

// counts letter occurrences in word and updates letter_count accordingly
std::vector<int>	get_letter_count(const std::string& word)
{
	std::vector<int>			letter_count(26,0);
	char						letter;
	std::string::const_iterator	i;
	
	for (i = word.begin(); i != word.end(); i++)
	{
		if (static_cast<unsigned char>(*i) == 0xc3)
		{
			i++;
			letter = utf_to_ascii(*i);
			if (letter != '.')
				letter_count[letter - 'a']++;
		}
		else if (std::isupper(*i))
			letter_count[*i - 'A']++;
		else if (std::isalpha(*i))
			letter_count[*i - 'a']++;
	}
	return (letter_count);
}

int	main(int argc, char *argv[])
{
	typedef std::multimap<std::vector<int>,std::string> mmap;

	const char*			prompt = "> ";
	std::string			word;
	std::ifstream		infile;
	std::vector<int>	letter_count;
	mmap				word_map;
	std::pair<typename mmap::const_iterator, typename mmap::const_iterator>	words_list;

	if (argc != 2)
		fatal_error("use: ./dico dico_filename");

	infile.open(argv[1]);
	if (!infile)
	{
		std::cerr << argv[1] << ": unable to open" << std::endl;
		exit(EXIT_FAILURE);
	}

	// initialize the map
	while (std::getline(infile, word))
	{
		letter_count = get_letter_count(word);
		word_map.insert(decltype(word_map)::value_type(letter_count, word));
	}
	infile.close();

	// read user input
	std::cout << prompt;
	while (std::getline(std::cin, word))
	{
		if (word == "_quit" || word == "_QUIT")
			break ;
		if (!word.empty())
		{
			letter_count = get_letter_count(word);	
			words_list = word_map.equal_range(letter_count);
			std::cout << "\t" << std::distance(words_list.first, words_list.second) << " match(es) found:" << std::endl;
			typename mmap::const_iterator	i;
			for (i = words_list.first; i != words_list.second; i++)
				std::cout << "\t- " << i->second << std::endl;
		}
		std::cout << prompt;
	}
	return (0);
}
